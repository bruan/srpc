#include "coroutine.h"
#include <tuple>

namespace rpc
{
	template<class T>
	CFuture<T>::CFuture()
		: m_pContext(nullptr)
	{

	}

	template<class T>
	CFuture<T>::CFuture(std::shared_ptr<SFutureContext<T>>& pContext)
		: m_pContext(pContext)
	{

	}

	template<class T>
	CFuture<T>::~CFuture()
	{

	}

	template<class T>
	bool CFuture<T>::isReady() const
	{
		if (!this->isVaild())
			return false;

		return this->m_pContext->bReady;
	}

	template<class T>
	bool CFuture<T>::isVaild() const
	{
		return this->m_pContext != nullptr;
	}

	template<class T>
	std::shared_ptr<T> CFuture<T>::getValue()
	{
		return this->m_pContext->val;
	}

	template<class T>
	uint32_t CFuture<T>::getErrorCode() const
	{
		if (!this->isReady())
			return 0;

		return this->m_pContext->nErrorCode;
	}

	template<class T>
	std::shared_ptr<SFutureContext<T>> CFuture<T>::getContext()
	{
		return this->m_pContext;
	}

	template<class T>
	bool CFuture<T>::wait()
	{
		if (this->m_pContext == nullptr)
			return false;
		if (this->m_pContext->nCoroutineID != 0)
			return false;

		this->m_pContext->nCoroutineID = coroutine::getCurrentID();
		coroutine::yield();
		return true;
	}

	template<class T>
	void CFuture<T>::then(const std::function<void(T*, uint32_t)>& fn)
	{
		if (this->m_pContext == nullptr || fn == nullptr)
			return;

		this->m_pContext->callback = fn;

		if (this->m_pContext->bReady && this->m_pContext->nErrorCode == 0)
			this->m_pContext->callback(this->m_pContext->val.get(), this->m_pContext->nErrorCode);
	}

	template<class T>
	template<class F, class R>
	R CFuture<T>::then_r(const F& fn)
	{
		if (this->m_pContext == nullptr)
			return R();

		using ValueType = typename R::ValueType;
		auto pContext = std::make_shared<SFutureContext<ValueType>>();
		pContext->bReady = false;
		pContext->nErrorCode = 0;

		this->m_pContext->callback = [fn, pContext](T* val, uint32_t nErrorCode)->void
		{
			CFuture<ValueType> sNewFuture = fn(val, nErrorCode);
			if (sNewFuture.m_pContext == nullptr)
				return;
			
			// 有可能返回的future已经是一个准备好的future，所以需要拷贝到pContext中
			pContext->bReady = sNewFuture.m_pContext->bReady;
			pContext->val = sNewFuture.m_pContext->val;
			pContext->nErrorCode = sNewFuture.m_pContext->nErrorCode;
			sNewFuture.m_pContext->callback = pContext->callback;
			// 在回调完成后如果返回的future已经准备好，并且回调函数也已经设置好了，就回调他，不然就没有机会回调了
			if (sNewFuture.m_pContext->bReady && sNewFuture.m_pContext->callback != nullptr)
				sNewFuture.m_pContext->callback(sNewFuture.m_pContext->val.get(), sNewFuture.m_pContext->nErrorCode);
		};

		// 在调用then时future已经可用了
		if (this->m_pContext->bReady)
			this->m_pContext->callback(this->m_pContext->val.get(), this->m_pContext->nErrorCode);
		
		return CFuture<ValueType>(pContext);
	}

	template<class T>
	CFuture<T> createFuture(T val)
	{
		auto pContext = std::make_shared<SFutureContext<T>>();
		pContext->bReady = true;
		pContext->val = val;

		return CFuture<T>(pContext);
	}
}