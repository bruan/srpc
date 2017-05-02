#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include <list>

namespace rpc
{
	enum EResponseCode
	{
		eRC_OK = 0,
		eRC_Timeout,
		eRC_Codec_Error,
		eRC_Channel_Error,
		eRC_Unknown,
	};

	struct SRequest
	{
		uint32_t	nID;
		uint64_t	nSessionID;
		const char* szData;
		uint32_t	nDataSize;
	};

	struct SResponse
	{
		uint64_t	nSessionID;
		std::string	szName;
		uint32_t	nErrorCode;
		const char* szData;
		uint32_t	nDataSize;
	};

	struct STimeoutInfo
	{
		uint64_t	nSessionID;
		int64_t		nTimeout;
	};

	struct STimeoutInfoComp
	{
		bool operator()(const STimeoutInfo& lhs, const STimeoutInfo& rhs)
		{
			return lhs.nTimeout < rhs.nTimeout;
		}
	};

	struct SPendingResponseInfo
	{
		uint64_t	nSessionID;
		uint64_t	nClientID;
		std::list<STimeoutInfo>::iterator
					iterTimeout;
		std::function<void(const char*, const char*, uint32_t, uint32_t)>
					pInvoke;
	};

	inline uint32_t getID(const std::string& szName)
	{
		uint32_t nHash = 1315423911;

		for (size_t i = 0; i < szName.size(); ++i)
		{
			nHash ^= ((nHash << 5) + (szName.at(i)) + (nHash >> 2));
		}

		return (nHash & 0x7FFFFFFF);
	}
}