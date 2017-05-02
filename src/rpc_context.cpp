#include "rpc_context.h"
#include "coroutine.h"
#include "coroutine_mgr.h"

#include <time.h>
#include <algorithm>

using namespace rpc;

CRpcContext::CRpcContext()
	: m_nNextSessionID(1)
	, m_nNextClientID(1)
{
}

CRpcContext::~CRpcContext()
{
}

void CRpcContext::dispatch(uint64_t nSessionID, uint32_t nErrorCode, const char* szName, const char* szBuf, uint32_t nBufSize)
{
	auto iter = this->m_mapPendingResponseInfo.find(nSessionID);
	if (iter == this->m_mapPendingResponseInfo.end())
		return;

	auto sResponseInfo = iter->second;
	if (sResponseInfo.iterTimeout != this->m_listTimeout.end())
		this->m_listTimeout.erase(sResponseInfo.iterTimeout);

	this->m_mapPendingResponseInfo.erase(iter);

	auto iterSessionID = this->m_mapPendingResponseInfoByClientID.find(sResponseInfo.nClientID);
	if (iterSessionID != this->m_mapPendingResponseInfoByClientID.end())
		iterSessionID->second.erase(sResponseInfo.nSessionID);

	sResponseInfo.pInvoke(szName, szBuf, nBufSize, nErrorCode);
}

uint64_t CRpcContext::genSession()
{
	return this->m_nNextSessionID++;
}

void CRpcContext::addPendingResponseInfo(SPendingResponseInfo sPendingResponseInfo, uint32_t nTimeout)
{
	STimeoutInfo sTimeoutInfo;
	sTimeoutInfo.nSessionID = sPendingResponseInfo.nSessionID;
	sTimeoutInfo.nTimeout = (int64_t)time(nullptr) + nTimeout;
	auto iter = lower_bound(this->m_listTimeout.begin(), this->m_listTimeout.end(), sTimeoutInfo, STimeoutInfoComp());
	iter = this->m_listTimeout.insert(iter, sTimeoutInfo);
	if (iter == this->m_listTimeout.end())
		return;

	sPendingResponseInfo.iterTimeout = iter;
	this->m_mapPendingResponseInfo[sPendingResponseInfo.nSessionID] = sPendingResponseInfo;
	this->m_mapPendingResponseInfoByClientID[sPendingResponseInfo.nClientID].insert(sPendingResponseInfo.nSessionID);
}

void CRpcContext::delClient(uint64_t nClientID)
{
	auto iter = this->m_mapPendingResponseInfoByClientID.find(nClientID);
	if (iter == this->m_mapPendingResponseInfoByClientID.end())
		return;

	const std::set<uint64_t>& setSessionID = iter->second;
	for (auto iterSessionID = setSessionID.begin(); iterSessionID != setSessionID.end(); ++iterSessionID)
	{
		auto iterResponseInfo = this->m_mapPendingResponseInfo.find(*iterSessionID);
		if (iterResponseInfo == this->m_mapPendingResponseInfo.end())
			continue;

		this->m_listTimeout.erase(iterResponseInfo->second.iterTimeout);
		this->m_mapPendingResponseInfo.erase(iterResponseInfo);
	}

	this->m_mapPendingResponseInfoByClientID.erase(iter);
}

uint64_t CRpcContext::genClientID()
{
	return this->m_nNextClientID++;
}

void CRpcContext::onTimer()
{
	int64_t nCurTime = time(nullptr);
	for (auto iter = this->m_listTimeout.begin(); iter != this->m_listTimeout.end();)
	{
		if (nCurTime < iter->nTimeout)
			break;

		uint64_t nSessionID = iter->nSessionID;
		iter = this->m_listTimeout.erase(iter);

		auto iterResponseInfo = this->m_mapPendingResponseInfo.find(nSessionID);
		if (iterResponseInfo == this->m_mapPendingResponseInfo.end())
			continue;

		SPendingResponseInfo sPendingResponseInfo = iterResponseInfo->second;
		this->m_mapPendingResponseInfo.erase(iterResponseInfo);
		auto iterClient = this->m_mapPendingResponseInfoByClientID.find(sPendingResponseInfo.nClientID);
		if (iterClient != this->m_mapPendingResponseInfoByClientID.end())
			iterClient->second.erase(nSessionID);

		sPendingResponseInfo.pInvoke(nullptr, nullptr, 0, eRC_Timeout);
	}
}