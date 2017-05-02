namespace rpc
{
	template<class T>
	CPromise<T>::CPromise()
		: m_bFuture(false)
	{
		this->m_pFutureContext = std::make_shared<SFutureContext<T>>();
		this->m_pFutureContext->bReady = false;
		this->m_pFutureContext->nErrorCode = 0;
		this->m_pFutureContext->nCoroutineID = 0;
	}

	template<class T>
	CPromise<T>::~CPromise()
	{

	}

	template<class T>
	CPromise<T>::CPromise(CPromise<T>&& lhs)
	{
		this->m_bFuture = lhs.m_bFuture;
		this->m_pFutureContext = lhs.m_pFutureContext;

		lhs.m_bFuture = false;
		lhs.m_pFutureContext = nullptr;
	}

	template<class T>
	CPromise<T>& CPromise<T>::operator = (CPromise<T>&& lhs)
	{
		if (this == &lhs)
			return *this;

		this->m_bFuture = lhs.m_bFuture;
		this->m_pFutureContext = lhs.m_pFutureContext;

		lhs.m_bFuture = false;
		lhs.m_pFutureContext = nullptr;

		return *this;
	}

	template<class T>
	CFuture<T> CPromise<T>::getFuture()
	{
		CFuture<T> sFuture;
		if (this->m_bFuture)
			return sFuture;

		this->m_bFuture = true;
		sFuture.m_pContext = this->m_pFutureContext;
		return sFuture;
	}

	template<class T>
	uint64_t CPromise<T>::getWaitCoroutineID() const
	{
		if (this->m_pFutureContext == nullptr)
			return 0;

		return this->m_pFutureContext->nCoroutineID;
	}

	template<class T>
	void CPromise<T>::setValue(std::shared_ptr<T> val, uint32_t nErrorCode /* = 0 */)
	{
		if (this->m_pFutureContext == nullptr)
			return;

		if (this->m_pFutureContext->bReady)
			return;
		
		this->m_pFutureContext->val = val;
		this->m_pFutureContext->nErrorCode = nErrorCode;
		this->m_pFutureContext->bReady = true;

		// 一旦调用了wait，callback就不会执行了
		if (this->m_pFutureContext->nCoroutineID != 0)
			return;

		if (this->m_pFutureContext->callback == nullptr)
			return;

		this->m_pFutureContext->callback(val.get(), nErrorCode);
	}
}