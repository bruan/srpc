// TestBaseLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "message_queue.h"
#include "rpc_context.h"
#include "rpc_client.h"
#include "rpc_service.h"
#include "when_all.h"
#include "protobuf_codec.h"
#include "request_msg0.pb.h"
#include "request_msg.pb.h"
#include "response_msg.pb.h"
#include "request_msg1.pb.h"
#include "response_msg1.pb.h"
#include "request_msg2.pb.h"
#include "response_msg2.pb.h"
#include <chrono>
#include <thread>
#include <sstream>

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#pragma pack(push,1)
// 消息头
struct message_header
{
	uint16_t	nMessageSize;	// 包括消息头的
	uint16_t	nMessageID;
};

struct rpc_request : public message_header
{
	uint32_t	nID;
	uint64_t	nSessionID;
};

struct rpc_response : public message_header
{
	uint32_t	nErrorCode;
	uint64_t	nSessionID;
	uint8_t		nNameLen;
	char		szName[1];
};

#pragma pack(pop)

#define RPC_REQUEST_ID 1
#define RPC_RESPONSE_ID 2

enum EMessageQueueType
{
	eMQT_C2P = 0,
	eMQT_P2C,
	eMQT_P2SA,
	eMQT_SA2P,
	eMQT_P2SB,
	eMQT_SB2P,
	eMQT_COMMAND,
	eMQT_COUNT,
};

CMessageQueue	g_zMessageQueue[eMQT_COUNT];

class CServiceA :
	public rpc::CRpcChannel
{
public:
	CServiceA(rpc::CRpcService<rpc::codec::CProtobufCodec>& service) : rpc::CRpcChannel(service.getRpcChannelGroup()), m_service(service) { }

	void init()
	{
		m_service.registerHandler<request_msg1, response_msg1>("request_msg1", [](request_msg1* pRequest, response_msg1& msg, const rpc::SSessionInfo& sSessionInfo)
		{
			msg.set_info(pRequest->info() + "<request_msg1>");
			return true;
		});

		this->m_thread = std::thread([this]()
		{
			coroutine::init(1024 * 1024);
			while (true)
			{
				this->dispatch();
			}
		});
	}

	void dispatch()
	{
		std::vector<SMessagePacket> vecMessagePacket;
		g_zMessageQueue[eMQT_P2SA].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);
			if (pHeader->nMessageID == RPC_REQUEST_ID)
			{
				const rpc_request* pRequest = reinterpret_cast<const rpc_request*>(pHeader);
				//std::cout << "CServiceA::dispatch: " << pRequest->nID << " " << pRequest->nSessionID << std::endl;
				rpc::SSessionInfo sSessionInfo;
				sSessionInfo.nChannelID = this->getChannelID();
				sSessionInfo.nSessionID = pRequest->nSessionID;
				m_service.dispatch(pRequest->nID, reinterpret_cast<const char*>(pRequest + 1), (uint32_t)(pRequest->nMessageSize - sizeof(rpc_request)), sSessionInfo);
			}

			delete[](char*)vecMessagePacket[i].pData;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	bool writeResponse(const rpc::SResponse& sResponse)
	{
		char* szBuf = new char[sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize];
		rpc_response* msg = reinterpret_cast<rpc_response*>(szBuf);
		msg->nMessageID = RPC_RESPONSE_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize);
		msg->nErrorCode = sResponse.nErrorCode;
		msg->nSessionID = sResponse.nSessionID;
		msg->nNameLen = (uint8_t)sResponse.szName.size();
		strncpy(msg->szName, sResponse.szName.c_str(), msg->nNameLen);
		msg->szName[msg->nNameLen] = 0;
		memcpy(szBuf + sizeof(rpc_response) + msg->nNameLen, sResponse.szData, sResponse.nDataSize);

		SMessagePacket sMessagePacket;
		sMessagePacket.nDataSize = msg->nMessageSize;
		sMessagePacket.pData = szBuf;
		g_zMessageQueue[eMQT_SA2P].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceA::writeResponse: " << " " << msg->nSessionID << std::endl;

		return true;
	}

	bool writeRequest(const rpc::SRequest& sRequest)
	{
		char* szBuf = new char[sizeof(rpc_request) + sRequest.nDataSize];
		rpc_request* msg = reinterpret_cast<rpc_request*>(szBuf);
		msg->nMessageID = RPC_REQUEST_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_request) + sRequest.nDataSize);
		msg->nID = sRequest.nID;
		msg->nSessionID = sRequest.nSessionID;
		memcpy(szBuf + sizeof(rpc_request), sRequest.szData, sRequest.nDataSize);
		SMessagePacket sMessagePacket;
		sMessagePacket.pData = szBuf;
		sMessagePacket.nDataSize = msg->nMessageSize;

		g_zMessageQueue[eMQT_P2SA].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceA::writeRequest: " << msg->nID << " " << msg->nSessionID << std::endl;

		return true;
	}

protected:
	rpc::CRpcService<rpc::codec::CProtobufCodec>& m_service;
	std::thread	m_thread;
};

class CServiceB :
	public rpc::CRpcChannel
{
public:
	CServiceB(rpc::CRpcService<rpc::codec::CProtobufCodec>& service) : rpc::CRpcChannel(service.getRpcChannelGroup()), m_service(service) { }

	void init()
	{
		m_service.registerHandler<request_msg2, response_msg2>("request_msg2", [](request_msg2* pRequest, response_msg2& msg, const rpc::SSessionInfo& sSessionInfo)
		{
			msg.set_info(pRequest->info() + "<request_msg2>");
			return true;
		});

		this->m_thread = std::thread([this]()
		{
			coroutine::init(1024 * 1024);
			while (true)
			{
				this->dispatch();
			}
		});
	}

	void dispatch()
	{
		std::vector<SMessagePacket> vecMessagePacket;
		g_zMessageQueue[eMQT_P2SB].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);
			if (pHeader->nMessageID == RPC_REQUEST_ID)
			{
				const rpc_request* pRequest = reinterpret_cast<const rpc_request*>(pHeader);
				//std::cout << "CServiceB::dispatch: " << pRequest->nID << " " << pRequest->nSessionID << std::endl;
				rpc::SSessionInfo sSessionInfo;
				sSessionInfo.nChannelID = this->getChannelID();
				sSessionInfo.nSessionID = pRequest->nSessionID;
				m_service.dispatch(pRequest->nID, reinterpret_cast<const char*>(pRequest + 1), (uint32_t)(pRequest->nMessageSize - sizeof(rpc_request)), sSessionInfo);
			}

			delete[](char*)vecMessagePacket[i].pData;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	bool writeResponse(const rpc::SResponse& sResponse)
	{
		char* szBuf = new char[sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize];
		rpc_response* msg = reinterpret_cast<rpc_response*>(szBuf);
		msg->nMessageID = RPC_RESPONSE_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize);
		msg->nErrorCode = sResponse.nErrorCode;
		msg->nSessionID = sResponse.nSessionID;
		msg->nNameLen = (uint8_t)sResponse.szName.size();
		strncpy(msg->szName, sResponse.szName.c_str(), msg->nNameLen);
		msg->szName[msg->nNameLen] = 0;
		memcpy(szBuf + sizeof(rpc_response) + msg->nNameLen, sResponse.szData, sResponse.nDataSize);

		SMessagePacket sMessagePacket;
		sMessagePacket.nDataSize = msg->nMessageSize;
		sMessagePacket.pData = szBuf;
		g_zMessageQueue[eMQT_SB2P].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceB::writeResponse: " << " " << msg->nSessionID << std::endl;

		return true;
	}

	bool writeRequest(const rpc::SRequest& sRequest)
	{
		char* szBuf = new char[sizeof(rpc_request) + sRequest.nDataSize];
		rpc_request* msg = reinterpret_cast<rpc_request*>(szBuf);
		msg->nMessageID = RPC_REQUEST_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_request) + sRequest.nDataSize);
		msg->nID = sRequest.nID;
		msg->nSessionID = sRequest.nSessionID;
		memcpy(szBuf + sizeof(rpc_request), sRequest.szData, sRequest.nDataSize);
		SMessagePacket sMessagePacket;
		sMessagePacket.pData = szBuf;
		sMessagePacket.nDataSize = msg->nMessageSize;

		g_zMessageQueue[eMQT_P2SB].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceB::writeRequest: " << msg->nID << " " << msg->nSessionID << std::endl;

		return true;
	}

protected:
	rpc::CRpcService<rpc::codec::CProtobufCodec>& m_service;
	std::thread	m_thread;
};

class CServiceProxy :
	public rpc::CRpcChannel
{
public:
	CServiceProxy(CServiceA& serviceA, CServiceB& serviceB, rpc::CRpcService<rpc::codec::CProtobufCodec>& service) : m_client1(&m_rpcContext), m_client2(&m_rpcContext), m_serviceA(serviceA), m_serviceB(serviceB), CRpcChannel(service.getRpcChannelGroup()), m_service(service) {}

	void init()
	{
		m_service.registerHandler<request_msg0>("request_msg0", [](request_msg0* pRequest)
		{
			std::cout << "request_msg0: " << pRequest->info() << std::endl;
		});

		m_service.registerHandler<request_msg, response_msg>("request_msg", std::bind(&CServiceProxy::msgHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

		this->m_client1.setChannel(&m_serviceA);
		this->m_client2.setChannel(&m_serviceB);

		this->m_thread = std::thread([this]()
		{
			coroutine::init(1024 * 1024);

			while (true)
			{
				this->dispatch();
			}
		});
	}

	bool msgHandler(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		switch(pRequest->tt())
		{
		case 1:
			return fun1(pRequest, msg, sSessionInfo);
			break;

		case 2:
			return fun2(pRequest, msg, sSessionInfo);
			break;

		case 3:
			return fun3(pRequest, msg, sSessionInfo);
			break;

		case 4:
			return fun4(pRequest, msg, sSessionInfo);
			break;

		case 5:
			return fun5(pRequest, msg, sSessionInfo);
			break;

		case 6:
			return fun6(pRequest, msg, sSessionInfo);
			break;
		}

		return false;
	}

	// 异步串行
	bool fun1(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture, 10);
		sFuture.then_r([this](response_msg1* pMsg, uint32_t nErrorCode)
		{
			rpc::CFuture<response_msg2> sFuture;
			request_msg2 msg2;
			msg2.set_info(pMsg->info());
			m_client2.async_call(&msg2, sFuture, 10);

			return sFuture;
		}).then([this, sSessionInfo](response_msg2* pMsg, uint32_t nErrorCode)
		{
			response_msg rmsg;
			rmsg.set_info(pMsg->info() + "[fun1]");
			this->m_service.response(sSessionInfo, &rmsg);
		});

		return false;
	}

	// 异步转同步串行
	bool fun2(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);
		sFuture1.wait();
		auto pMsg1 = sFuture1.getValue();
		if (nullptr == pMsg1)
			return false;
		
		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);
		sFuture2.wait();
		auto pMsg2 = sFuture2.getValue();
		if (nullptr == pMsg2)
			return false;

		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun2]");

		return true;
	}

	// 同步串行
	bool fun3(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		request_msg1 msg1;
		std::shared_ptr<response_msg1> pMsg1 = nullptr;
		msg1.set_info(pRequest->info());
		m_client1.sync_call(&msg1, pMsg1, 10);
		if (nullptr == pMsg1)
			return false;

		request_msg2 msg2;
		std::shared_ptr<response_msg2> pMsg2 = nullptr;
		msg2.set_info(pRequest->info());
		m_client2.sync_call(&msg2, pMsg2, 10);
		if (nullptr == pMsg2)
			return false;
		
		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun3]");

		return true;
	}

	// 异步并行
	bool fun4(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);

		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);

		auto sFuture = rpc::whenAll(sFuture1, sFuture2);

		sFuture.then([sSessionInfo, this](std::tuple<rpc::CFuture<response_msg1>, rpc::CFuture<response_msg2>>* all, uint32_t nErrorCode)
		{
			auto pMsg1 = std::get<0>(*all).getValue();
			auto pMsg2 = std::get<1>(*all).getValue();
			if (nullptr == pMsg1 || nullptr == pMsg2)
				return;

			response_msg rmsg;
			rmsg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun4]");
			this->m_service.response(sSessionInfo, &rmsg);
		});

		return false;
	}

	// 异步转同步并行
	bool fun5(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);

		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);

		sFuture1.wait();
		sFuture2.wait();

		auto pMsg1 = sFuture1.getValue();
		auto pMsg2 = sFuture2.getValue();
		if (nullptr == pMsg1 || nullptr == pMsg2)
			return false;

		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun5]");

		return true;
	}

	// 异步串行
	bool fun6(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);
		sFuture1
			.then_r(std::bind(&CServiceProxy::ff1, this, std::placeholders::_1, std::placeholders::_2))
			.then(std::bind(&CServiceProxy::ff2, this, sSessionInfo, std::placeholders::_1, std::placeholders::_2));

		return false;
	}

	rpc::CFuture<response_msg2> ff1(response_msg1* pMsg, uint32_t nErrorCode)
	{
		rpc::CFuture<response_msg2> sFuture;
		request_msg2 msg2;
		msg2.set_info(pMsg->info());
		m_client2.async_call(&msg2, sFuture, 10);

		return sFuture;
	}

	void ff2(const rpc::SSessionInfo& sSessionInfo, response_msg2* pMsg, uint32_t nErrorCode)
	{
		response_msg rmsg;
		rmsg.set_info(pMsg->info() + "[fun6]");
		this->m_service.response(sSessionInfo, &rmsg);
	}

	virtual void dispatch()
	{
		std::vector<SMessagePacket> vecMessagePacket;
		g_zMessageQueue[eMQT_C2P].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);
			if (pHeader->nMessageID == RPC_REQUEST_ID)
			{
				const rpc_request* pRequest = reinterpret_cast<const rpc_request*>(pHeader);
				//std::cout << "CServiceProxy::dispatch: " << pRequest->nID << " " << pRequest->nSessionID << std::endl;
				rpc::SSessionInfo sSessionInfo;
				sSessionInfo.nChannelID = this->getChannelID();
				sSessionInfo.nSessionID = pRequest->nSessionID;
				m_service.dispatch(pRequest->nID, reinterpret_cast<const char*>(pRequest + 1), (uint32_t)(pRequest->nMessageSize - sizeof(rpc_request)), sSessionInfo);
			}

			delete[](char*)vecMessagePacket[i].pData;
		}

		vecMessagePacket.clear();
		g_zMessageQueue[eMQT_SA2P].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);

			if (pHeader->nMessageID == RPC_RESPONSE_ID)
			{
				const rpc_response* pResponse = reinterpret_cast<const rpc_response*>(pHeader);
				//std::cout << "CServiceProxy::dispatch: " << pResponse->nSessionID << std::endl;
				m_rpcContext.dispatch(pResponse->nSessionID, pResponse->nErrorCode, pResponse->szName, reinterpret_cast<const char*>(pResponse + 1) + pResponse->nNameLen, (uint32_t)(pResponse->nMessageSize - sizeof(rpc_response) - pResponse->nNameLen));
			}

			delete[](char*)vecMessagePacket[i].pData;
		}

		vecMessagePacket.clear();
		g_zMessageQueue[eMQT_SB2P].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);

			if (pHeader->nMessageID == RPC_RESPONSE_ID)
			{
				const rpc_response* pResponse = reinterpret_cast<const rpc_response*>(pHeader);
				//std::cout << "CServiceProxy::dispatch: " << pResponse->nSessionID << std::endl;
				m_rpcContext.dispatch(pResponse->nSessionID, pResponse->nErrorCode, pResponse->szName, reinterpret_cast<const char*>(pResponse + 1) + pResponse->nNameLen, (uint32_t)(pResponse->nMessageSize - sizeof(rpc_response) - pResponse->nNameLen));
			}

			delete[](char*)vecMessagePacket[i].pData;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	bool writeResponse(const rpc::SResponse& sResponse)
	{
		char* szBuf = new char[sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize];
		rpc_response* msg = reinterpret_cast<rpc_response*>(szBuf);
		msg->nMessageID = RPC_RESPONSE_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_response) + sResponse.szName.size() + sResponse.nDataSize);
		msg->nErrorCode = sResponse.nErrorCode;
		msg->nSessionID = sResponse.nSessionID;
		msg->nNameLen = (uint8_t)sResponse.szName.size();
		strncpy(msg->szName, sResponse.szName.c_str(), msg->nNameLen);
		msg->szName[msg->nNameLen] = 0;
		memcpy(szBuf + sizeof(rpc_response) + msg->nNameLen, sResponse.szData, sResponse.nDataSize);

		SMessagePacket sMessagePacket;
		sMessagePacket.nDataSize = msg->nMessageSize;
		sMessagePacket.pData = szBuf;
		g_zMessageQueue[eMQT_P2C].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceProxy::writeResponse: " << " " << msg->nSessionID << std::endl;

		return true;
	}

	bool writeRequest(const rpc::SRequest& sRequest)
	{
		char* szBuf = new char[sizeof(rpc_request) + sRequest.nDataSize];
		rpc_request* msg = reinterpret_cast<rpc_request*>(szBuf);
		msg->nMessageID = RPC_REQUEST_ID;
		msg->nMessageSize = (uint16_t)(sizeof(rpc_request) + sRequest.nDataSize);
		msg->nID = sRequest.nID;
		msg->nSessionID = sRequest.nSessionID;
		memcpy(szBuf + sizeof(rpc_request), sRequest.szData, sRequest.nDataSize);
		SMessagePacket sMessagePacket;
		sMessagePacket.pData = szBuf;
		sMessagePacket.nDataSize = msg->nMessageSize;

		g_zMessageQueue[eMQT_C2P].addMessagePacket(sMessagePacket);

		//std::cout << "CServiceB::writeRequest: " << msg->nID << " " << msg->nSessionID << std::endl;

		return true;
	}

private:
	rpc::CRpcService<rpc::codec::CProtobufCodec>& m_service;
	std::thread	m_thread;
	rpc::CRpcContext m_rpcContext;
	rpc::CRpcClient<rpc::codec::CProtobufCodec> m_client1;
	rpc::CRpcClient<rpc::codec::CProtobufCodec> m_client2;
	CServiceA& m_serviceA;
	CServiceB& m_serviceB;
};

class CClient :
	public rpc::CRpcChannel
{
public:
	CClient(CServiceProxy& serviceProxy) : m_client(&m_rpcContext), m_serviceProxy(serviceProxy) { }

	void init()
	{
		m_client.setChannel(&m_serviceProxy);

		this->m_thread = std::thread([this]()
		{
			coroutine::init(1024 * 1024);
			while (true)
			{
				this->dispatch();
			}
		});
	}

	void dispatch()
	{
		std::vector<SMessagePacket> vecMessagePacket;
		g_zMessageQueue[eMQT_P2C].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			const message_header* pHeader = reinterpret_cast<message_header*>(vecMessagePacket[i].pData);

			if (pHeader->nMessageID == RPC_RESPONSE_ID)
			{
				const rpc_response* pResponse = reinterpret_cast<const rpc_response*>(pHeader);
				//std::cout << "CServiceProxy::dispatch: " << pResponse->nSessionID << std::endl;
				m_rpcContext.dispatch(pResponse->nSessionID, pResponse->nErrorCode, pResponse->szName, reinterpret_cast<const char*>(pResponse + 1) + pResponse->nNameLen, (uint32_t)(pResponse->nMessageSize - sizeof(rpc_response) - pResponse->nNameLen));
			}

			delete [](char*)vecMessagePacket[i].pData;
		}

		vecMessagePacket.clear();
		g_zMessageQueue[eMQT_COMMAND].getMessagePacket(vecMessagePacket);
		for (size_t i = 0; i < vecMessagePacket.size(); ++i)
		{
			request_msg msg;
			msg.set_tt(vecMessagePacket[i].nDataSize);
			msg.set_info((char*)vecMessagePacket[i].pData);
			rpc::CFuture<response_msg> sFuture;
			m_client.async_call(&msg, sFuture, 10);
			
			sFuture.then([this](response_msg* pMsg, uint32_t nErrorCode)
			{
				std::cout << "response_msg: " << pMsg->info() << std::endl;
			});

			delete[](char*)vecMessagePacket[i].pData;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

protected:
	rpc::CRpcContext m_rpcContext;
	rpc::CRpcClient<rpc::codec::CProtobufCodec> m_client;
	std::thread	m_thread;
	CServiceProxy& m_serviceProxy;
};


int main()
{
	for (uint32_t i = 0; i < eMQT_COUNT; ++i)
	{
		g_zMessageQueue[i].init();
	}

	rpc::CRpcService<rpc::codec::CProtobufCodec> rpc_serviceA;
	CServiceA serviceA(rpc_serviceA);
	serviceA.init();

	rpc::CRpcService<rpc::codec::CProtobufCodec> rpc_serviceB;
	CServiceB serviceB(rpc_serviceB);
	serviceB.init();

	rpc::CRpcService<rpc::codec::CProtobufCodec> rpc_serviceProxy;
	CServiceProxy serviceProxy(serviceA, serviceB, rpc_serviceProxy);
	serviceProxy.init();

	CClient client(serviceProxy);
	client.init();

	while (true)
	{
		uint32_t nType;
		std::string szValue;
		std::cin >> nType >> szValue;
		SMessagePacket sMessagePacket;
		sMessagePacket.nDataSize = nType;
		sMessagePacket.pData = new char[szValue.size()+1];
		strncpy((char*)sMessagePacket.pData, szValue.c_str(), szValue.size());
		((char*)sMessagePacket.pData)[szValue.size()] = 0;
		g_zMessageQueue[eMQT_COMMAND].addMessagePacket(sMessagePacket);
	}
	
	return 1;
}

#ifdef _WIN32
#pragma warning(pop)
#endif