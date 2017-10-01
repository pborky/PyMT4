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


#include "pymt4_client_iosession.h"
#include "pymt4_client_ioconnector.h"
namespace PyMT4
{


    IOSession::IOSession(io_service& ioService) : 
        IOSessionCommon(ioService)
    {
#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK -> registerHandler(MessageResultType)" << std::endl;
#endif
        registerHandler(MessageResultType,boost::bind(&IOSession::messageResultHandler,this,_1,_2,_3,_4,_5));

#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK -> registerHandler(MessageEventType)" << std::endl;
#endif
        registerHandler(MessageEventType,boost::bind(&IOSession::messageEventHandler,this,_1,_2,_3,_4,_5));
    }

    IOSession::~IOSession()
    {
        if (IOConnector::m_instance)
        {
            boost::mutex::scoped_lock sessionListLock(IOConnector::m_instance->_connectorMutex);
            BOOST_FOREACH(IOSessionMap::value_type& mapItem, IOConnector::m_instance->m_sessions)
            {
                if (mapItem.second == this) {
#ifdef _DEBUG
                    std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK. Instance()" << std::endl;
#endif
                    IOConnector::Instance()->m_sessions.erase(mapItem.first);
                    break;
                }
            }
        }
    }

    bool IOSession::messageResultHandler(const MessageTypeIdentifier& messageId, const MessageUID& messageuid, BufferCPos& begin, BufferCPos& end ,const boost::system::error_code& error)
    {
#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
        PendingResultPtr pendingResult = _pendingResultQueue.front();
        _pendingResultQueue.pop_front();

        MessageTypeIdentifier replyTo;
        size_t msguid_size=0;
        Serializer<MessageTypeIdentifier>::deserializeItem(&replyTo,&begin);
        Serializer<size_t>::deserializeItem(&msguid_size,&begin);

        MessageUID replyTomessageuid;

        for(uint8_t index=0;index<MessageUID::static_size();++index)
            Serializer<uint8_t>::deserializeItem(&replyTomessageuid.data[index],&begin);    

        int32_t lastErrorValue = 0;
        Serializer<int32_t>::deserializeItem(&lastErrorValue ,&begin);

#ifdef _DEBUG
        if (lastErrorValue != 0)
        {
            std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " ERROR: lastErrorValue: " << lastErrorValue << std::endl;
        }
#endif
        pendingResult->setError(lastErrorValue);
        pendingResult->setResult(begin,end);

        return true;
    }


    bool IOSession::messageEventHandler(const MessageTypeIdentifier& messageId, const MessageUID& messageuid, BufferCPos& begin, BufferCPos& end ,const boost::system::error_code& error)
    {
#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK" << std::endl;
#endif
        EventIdentifier eventType;

        Serializer<EventIdentifier>::deserializeItem(&eventType, &begin);
    
        switch(eventType)
        {
            case EvtId_OnTick:
            {
                std::string symbol;
                double bid,ask;
                int counter;
                Serializer<std::string>::deserializeItem(&symbol, &begin);
                Serializer<double>::deserializeItem(&bid, &begin);
                Serializer<double>::deserializeItem(&ask, &begin);
                Serializer<int>::deserializeItem(&counter, &begin);

                _sessionMutex.unlock();
#ifdef _DEBUG
                std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK. EvtId_OnTick -> Instance()" << std::endl;
#endif
                IOConnector::Instance()->notifyOnTick(static_pointer_cast<IOSession>(shared_from_this()), messageuid, symbol, bid, ask, counter);
                _sessionMutex.lock();
                break;
            }

            default:
                break;
        }

        return true;
    }


    PendingResultPtr IOSession::dispatchMessage(MessageHeaderPtr message)
    {
        PendingResultPtr pendingResult = PendingResult::Create();
        {
        boost::mutex::scoped_lock scopedLock(_sessionMutex);
        _pendingResultQueue.push_back(pendingResult);

        }
#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " dispatchMessage -> writeMessage(), message: " << message << std::endl;
#endif
        writeMessage(message);

        return pendingResult;

    }

    void IOSession::initialize()
    {
#ifdef _DEBUG
        std::cout << __FILE__ << "," << __FUNCTION__ << ",L:" << __LINE__ << " OK. keep_alive, enable_connection_aborted, no_delay. readHeader()" << std::endl;
#endif

        m_socket.set_option(boost::asio::socket_base::keep_alive(true));
        m_socket.set_option(boost::asio::socket_base::enable_connection_aborted(true));
        m_socket.set_option(boost::asio::ip::tcp::no_delay(true));
        readHeader();
    }
}