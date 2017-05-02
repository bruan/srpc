#include "rpc_channel.h"
#include "rpc_channel_group.h"

namespace rpc
{
	CRpcChannel::CRpcChannel()
		: m_nChannelID(0)
		, m_pRpcChannelGroup(nullptr)
	{

	}

	CRpcChannel::CRpcChannel(CRpcChannelGroup* pRpcChannelGroup)
		: m_nChannelID(0)
		, m_pRpcChannelGroup(pRpcChannelGroup)
	{
		if (this->m_pRpcChannelGroup != nullptr)
			this->m_pRpcChannelGroup->addChannel(this);
	}

	CRpcChannel::~CRpcChannel()
	{
		if (this->m_pRpcChannelGroup != nullptr)
			this->m_pRpcChannelGroup->delChannel(this);
	}
}