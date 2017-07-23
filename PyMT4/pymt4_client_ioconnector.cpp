/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */


#include "pymt4_client_ioconnector.h"

#include <boost/pointer_cast.hpp>
using namespace PyMT4;

IOConnectorPtr PyMT4::IOConnector::m_instance;

IOConnectorPtr PyMT4::IOConnector::Instance()
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << "\n";
#endif
	if (!m_instance)
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK, m_instance.reset -> new IOConnector" << "\n";
#endif
		m_instance.reset(new IOConnector());
	}

#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " return m_instance.reset" << "\n";
#endif
	return m_instance;
}


IOSessionMap& PyMT4::IOConnector::sessionMap()
{
	return m_sessions;
}

PyMT4::IOConnector::IOConnector() :
	m_iosWork(m_ioService),
	m_resolver(m_ioService),
	_poolWork(_poolIOService)
{
	for (size_t index = 0; index < 16; ++index)
	{
		_poolThreadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &_poolIOService));
	}
}

bool PyMT4::IOConnector::connect(const string& address, const uint32_t& port)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK -> m_iosThread.reset" << "\n";
#endif

	m_iosThread.reset(new thread( boost::bind(&io_service::run, &m_ioService)));

	boost::mutex waitMutex;
	boost::condition_variable waitCondition;
	boost::unique_lock<mutex> uniqueLock(waitMutex);
	bool result = false;

	ConnectCallback callback( [&result, &waitMutex, &waitCondition](const bool& connectResult)
		{	
			boost::unique_lock<mutex> uniqueLock(waitMutex);
			result=connectResult;
			waitCondition.notify_all();
			
		}
	);

	m_resolver.async_resolve(
		tcp::resolver::query(address,boost::lexical_cast<std::string>(port)),
		boost::bind(&PyMT4::IOConnector::resolveHandler, this, callback, asio::placeholders::error, boost::asio::placeholders::iterator));

	waitCondition.wait(uniqueLock);
	return result;
}


void PyMT4::IOConnector::connect(ConnectCallback connectCallback,const string& address,const uint32_t& port)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	m_resolver.async_resolve(
		tcp::resolver::query(address,boost::lexical_cast<std::string>(port)),
		boost::bind(&PyMT4::IOConnector::resolveHandler,this,connectCallback,asio::placeholders::error,boost::asio::placeholders::iterator));
}

bool PyMT4::IOConnector::disconnect()
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	{
		boost::mutex::scoped_lock connectorLock(_connectorMutex);
		BOOST_FOREACH(IOSessionMap::value_type& sessionData, m_sessions)
		{
			IOSession* session = sessionData.second;
			session->disconnect();
		}

		m_ioService.stop();
		_poolIOService.stop();

	}
	if (m_iosThread)
	{
		m_iosThread->join();
	}

	_poolThreadGroup.join_all();

	PyMT4::IOConnector::m_instance.reset();
	return true;
}

PyMT4::IOConnector::~IOConnector()
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif
	disconnect();

}


void PyMT4::IOConnector::resolveHandler(ConnectCallback connectCallback, const boost::system::error_code& error, tcp::resolver::iterator iterator)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	if (error || iterator == tcp::resolver::iterator())
	{
		if (error)
		{
			std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Error: " << error.message() << "\n";
		}
		return connectCallback(false);
	}

	IOSessionPtr connectSession(new IOSession(m_ioService));

	connectSession->socket().async_connect(iterator->endpoint(),
		boost::bind(&PyMT4::IOConnector::connectHandler, this, connectSession, connectCallback, asio::placeholders::error));
}


OnTickHandlerId PyMT4::IOConnector::registerOnTickHandler(const OnTickHandlerFunc& newHandler)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	OnTickHandlerId newId = boost::uuids::random_generator()();
	_onTickHandlerList[newId] = make_pair(false,newHandler);
	return newId;
};


void PyMT4::IOConnector::onTickDispatcher(const OnTickHandlerFunc& handlerFunc,IOSessionPtr session,const MessageUID& messageuid,const OnTickHandlerId& id,const std::string& symbol, const double& bid, const double& ask)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	handlerFunc(symbol, bid, ask);

	MessageResultPtr result = MessageResult::Create(MessageEventType,messageuid);

	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	if (_onTickHandlerList.find(id) != _onTickHandlerList.end())
	{
		_onTickHandlerList[id].first = false;
	}

	session->writeMessage(result);
	
}



void PyMT4::IOConnector::notifyOnTick(IOSessionPtr session,const MessageUID& messageuid, const std::string& symbol, const double& bid, const double& ask)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	BOOST_FOREACH(OnTickHandlerList::value_type& tickHandlerDetail,_onTickHandlerList)
	{
		boost::mutex::scoped_lock connectorLock(_connectorMutex);
		if (!tickHandlerDetail.second.first)
		{	
			tickHandlerDetail.second.first = true;
			_poolIOService.post(boost::bind(&PyMT4::IOConnector::onTickDispatcher,shared_from_this(),tickHandlerDetail.second.second,session,messageuid,tickHandlerDetail.first,symbol,bid,ask));
		}
	}

};


void PyMT4::IOConnector::connectHandler(IOSessionPtr connectSession, ConnectCallback connectCallback, const boost::system::error_code& error)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	if (error)
	{
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Error: " << error.message() << "\n";
		return connectCallback(false);
	}

	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	tcp::endpoint& remoteEndpoint = connectSession->socket().remote_endpoint();
	m_sessions.insert(make_pair(make_pair(remoteEndpoint.address().to_v4().to_string(), remoteEndpoint.port()), connectSession.get()));

#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK. remoteEndpoint address:" << remoteEndpoint.address().to_v4().to_string() << "\n";
#endif

	connectSession->initialize();
	connectCallback(true);
}


PendingResultPtr PyMT4::IOConnector::dispatchMessage(const MessageHeaderPtr message)
{
#ifdef _DEBUG
	std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " Info: OK" << "\n";
#endif

	boost::mutex::scoped_lock connectorLock(_connectorMutex);
	
	if (!m_sessions.size())
	{
		PendingResultPtr invalidResult = PendingResult::Create();
		invalidResult->setError(ERR_PYMT4_NOT_CONNECTED);
		return invalidResult;
	}

	IOSession* iosession = m_sessions.begin()->second;

	return iosession->dispatchMessage(message);

}