#include "coroutine.h"

namespace rpc
{
	template<class CodecPolicy>
	CRpcService<CodecPolicy>::CRpcService()
	{

	}

	template<class CodecPolicy>
	CRpcService<CodecPolicy>::~CRpcService()
	{
	}

	template<class CodecPolicy>
	void CRpcService<CodecPolicy>::dispatch(uint32_t nID, const char* szBuf, uint32_t nBufSize, const SSessionInfo& sSessionInfo)
	{
		auto iter = this->m_mapFunctionInfo.find(nID);
		if (iter == this->m_mapFunctionInfo.end())
			return;

		const SFunctionInfo& sFunctionInfo = iter->second;
		uint64_t nCoroutineID = coroutine::create(0, [&sFunctionInfo, szBuf, nBufSize, sSessionInfo](uint64_t)
		{
			sFunctionInfo.pInvoke(sFunctionInfo.szName.c_str(), szBuf, nBufSize, sSessionInfo);
		});
		coroutine::resume(nCoroutineID, 0);
	}

	template<class CodecPolicy>
	template<class T, class R>
	bool CRpcService<CodecPolicy>::registerHandler(const std::string& szName, const std::function<bool(T*, R&, const SSessionInfo&)>& callback)
	{
		if (callback == nullptr)
			return false;

		uint32_t nID = getID(szName);

		if (this->m_mapFunctionInfo.find(nID) != this->m_mapFunctionInfo.end())
			return false;

		auto callback_proxy = [this, callback](const char* szName, const char* szBuf, uint32_t nBufSize, const SSessionInfo& sSessionInfo)->void
		{
			T* pMessage = nullptr;
			if (!this->m_codecPolicy.unserialize(szName, szBuf, nBufSize, pMessage))
				return;

			std::unique_ptr<T> pMessagePtr = std::unique_ptr<T>(pMessage);
			R responseMsg;
			if (callback(pMessage, responseMsg, sSessionInfo))
			{
				this->response(sSessionInfo, &responseMsg);
			}
		};

		SFunctionInfo sFunctionInfo;
		sFunctionInfo.szName = szName;
		sFunctionInfo.pInvoke = callback_proxy;

		this->m_mapFunctionInfo[nID] = sFunctionInfo;

		return true;
	}

	template<class CodecPolicy>
	template<class T>
	bool CRpcService<CodecPolicy>::registerHandler(const std::string& szName, const std::function<void(T*)>& callback)
	{
		if (callback == nullptr)
			return false;

		uint32_t nID = getID(szName);

		if (this->m_mapFunctionInfo.find(nID) != this->m_mapFunctionInfo.end())
			return false;

		auto callback_proxy = [this, callback](const char* szName, const char* szBuf, uint32_t nBufSize, const SSessionInfo& sSessionInfo)->void
		{
			T* pMessage = nullptr;
			if (!this->m_codecPolicy.unserialize(szName, szBuf, nBufSize, pMessage))
				return;

			std::unique_ptr<T> pMessagePtr = std::unique_ptr<T>(pMessage);
			callback(pMessage);
		};

		SFunctionInfo sFunctionInfo;
		sFunctionInfo.szName = szName;
		sFunctionInfo.pInvoke = callback_proxy;

		this->m_mapFunctionInfo[nID] = sFunctionInfo;

		return true;
	}

	template<class CodecPolicy>
	template<class T>
	void CRpcService<CodecPolicy>::response(const SSessionInfo& sSessionInfo, T* pMessage)
	{
		SResponse sResponse;
		if (!this->m_codecPolicy.serializeResponse(pMessage, sResponse))
			return;

		sResponse.nErrorCode = eRC_OK;
		sResponse.nSessionID = sSessionInfo.nSessionID;
		
		CRpcChannel* pRpcChannel = this->m_rpcChannelGroup.getChannel(sSessionInfo.nChannelID);
		if (pRpcChannel == nullptr)
			return;

		pRpcChannel->writeResponse(sResponse);
	}

	template<class CodecPolicy>
	CRpcChannelGroup* CRpcService<CodecPolicy>::getRpcChannelGroup()
	{
		return &this->m_rpcChannelGroup;
	}
}