#pragma once
#include <memory>
#include <functional>
#include <stdint.h>

namespace rpc
{
	template<class T>
	struct SFutureContext
	{
		std::function<void(T*, uint32_t)>
					callback;
		bool		bReady;
		uint64_t	nCoroutineID;
		uint32_t	nErrorCode;
		std::shared_ptr<T>
					val;
	};

	template<class T>
	class CFuture
	{
		template<class U>
		friend class CPromise;
		template<class U>
		friend class CFuture;

	public:
		typedef T ValueType;

	public:
		CFuture();
		CFuture(std::shared_ptr<SFutureContext<T>>& pContext);
		~CFuture();

		bool		isReady() const;
		bool		isVaild() const;
		std::shared_ptr<T>
					getValue();
		uint32_t	getErrorCode() const;
		// һ��������wait, then�����õĻص������ᱻִ��
		bool		wait();
		std::shared_ptr<SFutureContext<T>>
					getContext();

		void		then(const std::function<void(T*, uint32_t)>& fn);
		
		template<class F, class R = typename std::result_of<F(T*, uint32_t)>::type>
		R			then_r(const F& fn);

	private:
		std::shared_ptr<SFutureContext<T>>	m_pContext;
	};

	template<class T>
	static CFuture<T> createFuture(T val);
}

#include "future.inl"