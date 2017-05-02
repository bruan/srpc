#pragma once
#include "rpc_base.h"

namespace rpc
{
	class CRpcChannelGroup;
	class CRpcChannel
	{
		friend class CRpcChannelGroup;

	public:
		CRpcChannel();
		CRpcChannel(CRpcChannelGroup* pRpcChannelGroup);
		virtual ~CRpcChannel();

		uint64_t		getChannelID() const { return this->m_nChannelID; }

		virtual bool	writeRequest(const SRequest& sRequest) { return true; }
		virtual bool	writeResponse(const SResponse& sResponse) { return true; }

	private:
		uint64_t			m_nChannelID;
		CRpcChannelGroup*	m_pRpcChannelGroup;
	};
}