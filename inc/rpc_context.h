#pragma once
#include <stdint.h>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <queue>

#include "future.h"
#include "rpc_channel.h"

namespace rpc
{
	template<class T>
	class CRpcClient;

	class CRpcContext
	{
	private:
		CRpcContext(const CRpcContext&) = delete;
		const CRpcContext& operator = (const CRpcContext&) = delete;

		template<class T>
		friend class CRpcClient;

	public:
		CRpcContext();
		~CRpcContext();

		void		dispatch(uint64_t nSessionID, uint32_t nErrorCode, const char* szName, const char* szBuf, uint32_t nBufSize);
		void		onTimer();

	private:
		uint64_t	genClientID();
		void		delClient(uint64_t nClientID);
		void		addPendingResponseInfo(SPendingResponseInfo sPendingResponseInfo, uint32_t nTimeout);
		uint64_t	genSession();

	private:
		std::map<uint64_t, SPendingResponseInfo>
						m_mapPendingResponseInfo;
		std::map<uint64_t, std::set<uint64_t>>
						m_mapPendingResponseInfoByClientID;
		std::list<STimeoutInfo>
						m_listTimeout;
		uint64_t		m_nNextSessionID;
		uint64_t		m_nNextClientID;
	};
}