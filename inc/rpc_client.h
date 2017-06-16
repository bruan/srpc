#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "future.h"
#include "rpc_channel.h"
#include "rpc_context.h"

namespace rpc
{
	template<class CodecPolicy>
	class CRpcClient
	{
	private:
		CRpcClient(const CRpcClient&) = delete;
		const CRpcClient& operator = (const CRpcClient&) = delete;

	public:
		CRpcClient(CRpcContext* pRpcContext);
		~CRpcClient();

		void		setChannel(CRpcChannel* pRpcChannel);

		/*
		发送网络消息，对方不需要响应
		*/
		template<class T>
		uint32_t	send(const T* pMessage);

		/*
		异步的调用远程的接口，通过CFuture来拿到响应结果
		*/
		template<class T, class R>
		uint32_t	async_call(const T* pMessage, CFuture<R>& sFuture, uint32_t nTimeout);

		/*
		同步的调用远程的接口，通过pResponseMessage来拿到响应结果，这里用shared_ptr的原因是为了自动释放pResponseMessage
		*/
		template<class T, class R>
		uint32_t	sync_call(const T* pMessage, std::shared_ptr<R>& pResponseMessage, uint32_t nTimeout);

	private:
		uint64_t		m_nClientID;
		CRpcContext*	m_pRpcContext;
		CRpcChannel*	m_pRpcChannel;
		CodecPolicy		m_codecPolicy;
	};
}

#include "rpc_client.inl"