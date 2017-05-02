#include "rpc_channel.h"
#include "rpc_channel_group.h"

namespace rpc
{
	CRpcChannelGroup::CRpcChannelGroup()
		: m_nNextChannelID(1)
	{

	}

	CRpcChannelGroup::~CRpcChannelGroup()
	{

	}

	void CRpcChannelGroup::addChannel(CRpcChannel* pRpcChannel)
	{
		if (nullptr == pRpcChannel)
			return;

		pRpcChannel->m_nChannelID = this->m_nNextChannelID++;
		this->m_mapRpcChannel[pRpcChannel->m_nChannelID] = pRpcChannel;
	}

	void CRpcChannelGroup::delChannel(CRpcChannel* pRpcChannel)
	{
		if (nullptr == pRpcChannel)
			return;

		this->m_mapRpcChannel.erase(pRpcChannel->m_nChannelID);
	}

	CRpcChannel* CRpcChannelGroup::getChannel(uint64_t nID) const
	{
		auto iter = this->m_mapRpcChannel.find(nID);
		if (iter == this->m_mapRpcChannel.end())
			return nullptr;

		return iter->second;
	}

}