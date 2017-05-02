#include "promise.h"
#include "coroutine.h"

namespace rpc
{
	template<class CodecPolicy>
	CRpcClient<CodecPolicy>::CRpcClient(CRpcContext* pRpcContext)
		: m_pRpcChannel(nullptr)
		, m_pRpcContext(pRpcContext)
	{
		this->m_nClientID = pRpcContext->genClientID();
	}

	template<class CodecPolicy>
	CRpcClient<CodecPolicy>::~CRpcClient()
	{
		this->m_pRpcContext->delClient(this->m_nClientID);
	}

	template<class CodecPolicy>
	void CRpcClient<CodecPolicy>::setChannel(CRpcChannel* pRpcChannel)
	{
		this->m_pRpcChannel = pRpcChannel;
	}

	template<class CodecPolicy>
	template<class T>
	uint32_t CRpcClient<CodecPolicy>::send(const T* pMessage)
	{
		SRequest sRequest;
		if (!this->m_codecPolicy.serializeRequest(pMessage, sRequest))
			return eRC_Codec_Error;

		sRequest.nSessionID = 0;

		if (!this->m_pRpcChannel->writeRequest(sRequest))
			return eRC_Channel_Error;

		return eRC_OK;
	}

	template<class CodecPolicy>
	template<class T, class R>
	uint32_t CRpcClient<CodecPolicy>::async_call(const T* pMessage, CFuture<R>& sFuture, uint32_t nTimeout)
	{
		SRequest sRequest;
		if (!this->m_codecPolicy.serializeRequest(pMessage, sRequest))
			return eRC_Codec_Error;

		uint64_t nSessionID = this->m_pRpcContext->genSession();
		sRequest.nSessionID = nSessionID;

		if (!this->m_pRpcChannel->writeRequest(sRequest))
			return eRC_Channel_Error;

		auto pPromise = std::make_shared<CPromise<R>>();
		auto callback = [nSessionID, pPromise, this](const char* szName, const char* szBuf, uint32_t nBufSize, uint32_t nErrorCode)->void
		{
			uint64_t nWaitCoroutineID = pPromise->getWaitCoroutineID();
			if (nWaitCoroutineID == 0)
			{
				uint64_t nCoroutineID = coroutine::create(0, [pPromise, szName, szBuf, nBufSize, &nErrorCode, this](uint64_t)
				{
					if (nErrorCode == eRC_OK)
					{
						R* pResponseMessage = nullptr;
						if (!this->m_codecPolicy.unserialize(szName, szBuf, nBufSize, pResponseMessage))
							nErrorCode = eRC_Codec_Error;
						
						pPromise->setValue(std::shared_ptr<R>(pResponseMessage), nErrorCode);
					}
					else
					{
						pPromise->setValue(nullptr, nErrorCode);
					}
				});

				coroutine::resume(nCoroutineID, 0);
			}
			else
			{
				if (nErrorCode == eRC_OK)
				{
					R* pResponseMessage = nullptr;
					if (!this->m_codecPolicy.unserialize(szName, szBuf, nBufSize, pResponseMessage))
						nErrorCode = eRC_Codec_Error;

					pPromise->setValue(std::shared_ptr<R>(pResponseMessage), nErrorCode);
					coroutine::resume(nWaitCoroutineID, 0);
				}
				else
				{
					pPromise->setValue(nullptr, nErrorCode);
					coroutine::resume(nWaitCoroutineID, 0);
				}
			}
		};

		SPendingResponseInfo sPendingResponseInfo;
		sPendingResponseInfo.nSessionID = nSessionID;
		sPendingResponseInfo.nClientID = this->m_nClientID;
		sPendingResponseInfo.pInvoke = callback;
		sFuture = pPromise->getFuture();

		this->m_pRpcContext->addPendingResponseInfo(sPendingResponseInfo, nTimeout);

		return eRC_OK;
	}

	template<class CodecPolicy>
	template<class T, class R>
	uint32_t CRpcClient<CodecPolicy>::sync_call(const T* pMessage, std::shared_ptr<R>& pResponseMessage, uint32_t nTimeout)
	{
		SRequest sRequest;
		if (!this->m_codecPolicy.serializeRequest(pMessage, sRequest))
			return eRC_Codec_Error;

		uint64_t nSessionID = this->m_pRpcContext->genSession();
		sRequest.nSessionID = nSessionID;

		if (!this->m_pRpcChannel->writeRequest(sRequest))
			return eRC_Channel_Error;

		uint64_t nCoroutineID = coroutine::getCurrentID();
		auto callback = [nSessionID, nCoroutineID, this](const char* szName, const char* szBuf, uint32_t nBufSize, uint32_t nErrorCode)->void
		{
			if (nErrorCode == eRC_OK)
			{
				R* pNewMessage = nullptr;
				if (!this->m_codecPolicy.unserialize(szName, szBuf, nBufSize, pNewMessage))
					nErrorCode = eRC_Codec_Error;

				coroutine::setLocalData(nCoroutineID, "message", (uintptr_t)pNewMessage);
				coroutine::setLocalData(nCoroutineID, "error_code", nErrorCode);
			}
			else
			{
				coroutine::setLocalData(nCoroutineID, "message", (uintptr_t)nullptr);
				coroutine::setLocalData(nCoroutineID, "error_code", nErrorCode);
			}

			coroutine::resume(nCoroutineID, 0);
		};

		SPendingResponseInfo sPendingResponseInfo;
		sPendingResponseInfo.nSessionID = nSessionID;
		sPendingResponseInfo.nClientID = this->m_nClientID;
		sPendingResponseInfo.pInvoke = callback;
		
		this->m_pRpcContext->addPendingResponseInfo(sPendingResponseInfo, nTimeout);

		coroutine::yield();
		uintptr_t nResponseMessage = 0;
		if (!coroutine::getLocalData(coroutine::getCurrentID(), "message", nResponseMessage))
			return eRC_Unknown;

		uintptr_t nErrorCode = 0;
		if (!coroutine::getLocalData(coroutine::getCurrentID(), "error_code", nErrorCode))
			return eRC_Unknown;

		pResponseMessage = std::shared_ptr<R>(reinterpret_cast<R*>(nResponseMessage));
		
		return (uint32_t)nErrorCode;
	}
}