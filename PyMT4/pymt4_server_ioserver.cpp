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


#include "pymt4_server_ioserver.h"

using namespace boost;

namespace PyMT4
{

	#define PENDING_CMD_WAIT_TIMEOUT 50

	/* ///////////////////////////// *
	 * 
	 * Static Variable Definition 
	 *
	 */

	IOServerPtr PyMT4::IOServer::m_instance;


	/* ///////////////////////////// *
	 * 
	 * IOServer Class Definition
	 *
	 */

	int32_t PyMT4::IOServer::WindowUpdateMsg = RegisterWindowMessage(L"MetaTrader4_Internal_Message");

	PyMT4::IOServer::~IOServer()
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK -> shutdown()" << std::endl;
#endif
		shutdown();
	}

	PyMT4::IOServer::IOServer() :
	m_iosWork(m_ioService),
	m_acceptor(m_ioService)
	{	
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK. m_iosThread.reset(new thread), m_acceptor.open and bind port, acceptSession()" << std::endl;
#endif
		m_iosThread.reset(new thread( boost::bind(&io_service::run, &m_ioService)));

		m_acceptor.open(tcp::v4());
		m_acceptor.bind(tcp::endpoint(tcp::v4(),DEFAULT_PORT));
		m_acceptor.listen();

		acceptSession();
	}


	void PyMT4::IOServer::acceptSession()
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
		IOSessionPtr pendingSession(new IOSession(m_ioService));
		m_acceptor.async_accept(pendingSession->socket(),bind(&PyMT4::IOServer::acceptHandler,this,pendingSession,asio::placeholders::error));
	}

	void PyMT4::IOServer::acceptHandler(IOSessionPtr session, const system::error_code& error)
	{
		if(!error)
		{
			boost::mutex::scoped_lock scopedlock(_servermutex);
			_sessionList.insert(make_pair(session->id(),session.get()));
			session->initialize();
			acceptSession();
		}
	}


	void PyMT4::IOServer::registerChartWindow(const std::string& chartName, HWND chartHandle)
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
		_registeredWindowList[chartName.c_str()] = chartHandle;
	}

	void PyMT4::IOServer::requestChartsUpdate()
	{
		BOOST_FOREACH(ChartWindowList::value_type chartWindowData, _registeredWindowList)
		{
			PostMessageA(chartWindowData.second, PyMT4::IOServer::WindowUpdateMsg, 2, 1);
		}

	}


	void PyMT4::IOServer::queueCommand(PendingCommandPtr command)
	{
		boost::mutex::scoped_lock scopedlock(_servermutex);

		_pendingCommandList.push_back(command);
		_processCondition.notify_all();
	}


	int32_t PyMT4::IOServer::pendingCommand()
	{
		boost::mutex::scoped_lock scopedLock(_servermutex);

		if (!_pendingCommandList.size())
		{
			_processCondition.timed_wait(scopedLock,boost::posix_time::milliseconds(PENDING_CMD_WAIT_TIMEOUT));
		}

		CommandIdentifier commandId = CheckShutdownCondition;

		if (_pendingCommandList.size())
		{
			_currentCommand = _pendingCommandList.front();
			_pendingCommandList.pop_front();
			commandId = _currentCommand->commandId;
		}
		return (int)commandId;
	}


	const std::string& PyMT4::IOServer::getStringArgument(std::string& stringbuffer)
	{
		std::string result;
		Serializer<std::string>::deserializeItem(&result, &_currentCommand->dataPos);
		std::copy(result.begin(), result.end(), std::back_inserter(stringbuffer));
		stringbuffer[result.size()] = NULL;

		// Todo: Not yet working
		//char result[1024] = { 0 };
		//Serializer<char>::deserializeItem(result, &_currentCommand->dataPos);
		//
		//std::string resultString = result;
		//std::copy(resultString.begin(), resultString.end(), std::back_inserter(stringbuffer));
		//stringbuffer[resultString.size()] = NULL;
		////ioserver->completeCommand<std::string>(resultString.c_str(), error);

		return stringbuffer;
	}

	void PyMT4::IOServer::shutdown()
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
		if (m_instance)
		{
#ifdef _DEBUG
			std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " session->disconnect(), ioService.stop(), iosThread->join()" << std::endl;
#endif
			{
				boost::mutex::scoped_lock scopedlock(_servermutex);

				BOOST_FOREACH(IOSessionMap::value_type& sessionData,_sessionList)
				{
					IOSession* session = sessionData.second;
					session->disconnect();
				}

				m_ioService.stop();
			}


			m_iosThread->join();
			PyMT4::IOServer::m_instance.reset();
		}
	}

	void PyMT4::IOServer::notifyResult(const MessageTypeIdentifier&,const MessageUID& replyToUid)
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
		boost::mutex::scoped_lock scopedlock(_servermutex);
		Buffer& buffer = Buffer();
		_onTickResults[replyToUid]->setResult(buffer.begin(), buffer.end());
	}

	bool PyMT4::IOServer::dispatchOnTick(const char *symbol, const double& bid, const double& ask, const int& counter)
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK -> writeMessage()" << std::endl;
#endif
		{
			boost::mutex::scoped_lock scopedlock(_servermutex);

			_onTickResults.clear();
			BOOST_FOREACH(IOSessionMap::value_type& sessionData,_sessionList)
			{

				MessageEventPtr messageEvent = MessageEvent::Create(EvtId_OnTick);

				Buffer& buffer = messageEvent->messageBuffer();

				std::string resultString = symbol;
				Serializer<std::string>::serializeItem(const_cast<std::string*>(&resultString), &buffer);
				Serializer<double>::serializeItem(const_cast<double*>(&bid), &buffer);
				Serializer<double>::serializeItem(const_cast<double*>(&ask), &buffer);
				Serializer<int>::serializeItem(const_cast<int*>(&counter), &buffer);


				IOSession* session = sessionData.second;
				session->writeMessage(messageEvent);

				_onTickResults[messageEvent->uid()] = PendingResult::Create();

			}

		}
	
		BOOST_FOREACH(OnTickResultList::value_type& resultItem, _onTickResults)
		{
			resultItem.second->waitForResult<bool>();
		}


		return true;
	}


	IOServerPtr PyMT4::IOServer::Instance()
	{
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
		if (!m_instance)
		{
#ifdef _DEBUG
			std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " m_instance.reset(new IOServer)" << std::endl;
#endif
			m_instance.reset(new IOServer);
		}
#ifdef _DEBUG
		std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " return m_instance" << std::endl;
#endif
		return m_instance;
	}

}