#pragma once

#include "future.h"

namespace rpc
{
	template<class T>
	class CPromise
	{
	private:
		CPromise(const CPromise&) = delete;
		const CPromise& operator = (const CPromise&) = delete;

	public:
		CPromise();
		~CPromise();

		CPromise(CPromise<T>&& lhs);

		CPromise<T>& operator = (CPromise<T>&& lhs);

		CFuture<T>	getFuture();

		uint64_t	getWaitCoroutineID() const;
		void		setValue(std::shared_ptr<T> val, uint32_t nErrorCode = 0);

	private:
		std::shared_ptr<SFutureContext<T>>	m_pFutureContext;
		bool								m_bFuture;
	};
}

#include "promise.inl"