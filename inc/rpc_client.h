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
		����������Ϣ���Է�����Ҫ��Ӧ
		*/
		template<class T>
		uint32_t	send(const T* pMessage);

		/*
		�첽�ĵ���Զ�̵Ľӿڣ�ͨ��CFuture���õ���Ӧ���
		*/
		template<class T, class R>
		uint32_t	async_call(const T* pMessage, CFuture<R>& sFuture, uint32_t nTimeout);

		/*
		ͬ���ĵ���Զ�̵Ľӿڣ�ͨ��pResponseMessage���õ���Ӧ�����������shared_ptr��ԭ����Ϊ���Զ��ͷ�pResponseMessage
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