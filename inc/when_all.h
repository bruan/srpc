#pragma once
#include "future.h"
#include "promise.h"

#include <tuple>
#include <stdint.h>

namespace rpc
{
	template<class ...Args>
	CFuture<std::tuple<Args...>> whenAll(Args... args);
}

#include "when_all.inl"