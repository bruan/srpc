#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "rpc_channel.h"
#include "rpc_channel_group.h"

namespace rpc
{
	struct SSessionInfo
	{
		uint64_t	nChannelID;
		uint64_t	nSessionID;
	};

	template<class CodecPolicy>
	class CRpcService
	{
	private:
		CRpcService(const CRpcService&) = delete;
		const CRpcService& operator = (const CRpcService&) = delete;

	public:
		CRpcService();
		~CRpcService();

		template<class T, class R>
		bool	registerHandler(const std::string& szName, const std::function<bool(T*, R&, const SSessionInfo&)>& callback);

		template<class T>
		bool	registerHandler(const std::string& szName, const std::function<void(T*)>& callback);

		template<class T>
		void	response(const SSessionInfo& sSessionInfo, T* pMessage);

		void	dispatch(uint32_t nID, const char* szBuf, uint32_t nBufSize, const SSessionInfo& sSessionInfo);

		CRpcChannelGroup*
			getRpcChannelGroup();

	private:
		struct SFunctionInfo
		{
			std::string	szName;
			std::function<void(const char*, const char*, uint32_t, const SSessionInfo&)>
						pInvoke;
		};

		std::map<uint32_t, SFunctionInfo>	m_mapFunctionInfo;
		CodecPolicy							m_codecPolicy;
		CRpcChannelGroup					m_rpcChannelGroup;
	};
}

#include "rpc_service.inl"