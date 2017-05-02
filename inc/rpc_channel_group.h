#pragma once
#include <map>
#include <stdint.h>

namespace rpc
{
	class CRpcChannel;
	class CRpcChannelGroup
	{
	public:
		CRpcChannelGroup();
		~CRpcChannelGroup();

		void			addChannel(CRpcChannel* pRpcChannel);
		void			delChannel(CRpcChannel* pRpcChannel);
		CRpcChannel*	getChannel(uint64_t nID) const;

	private:
		uint64_t							m_nNextChannelID;
		std::map<uint64_t, CRpcChannel*>	m_mapRpcChannel;
	};
}