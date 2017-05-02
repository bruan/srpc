#### 这是一个简单rpc库

1. 支持同步调用跟异步调用
2. 支持异步转同步
3. 支持串行异步，以及并行异步
4. 自定义序列化方式
5. 没有网络相关操作
6. 跨平台
7. 基于c++11实现


#### 实现说明
同步方式的实现采用了协程（windows采用纤程api实现，linux采用自定义实现），异步的方式采用future的方式，支持串行（then）以及并行（whenAll）

#### 编译方式
1. 首先需要下载编译第三方依赖库（windows下执行3rd/win_3rd.bat linux下执行3rd/linux_3rd.sh）
2. 接着编译项目（windows下直接用vs2015打开build/srpc.sln就行，linux gcc版本至少4.8以上 cmake 2.8版本以上 执行build/build.sh all）

#### 下面以一个调用另外两个服务的例子来说明

#### 异步串行的方式
    bool fun1(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture, 10);
		sFuture.then_r([this](response_msg1* pMsg, uint32_t nErrorCode)
		{
			rpc::CFuture<response_msg2> sFuture;
			request_msg2 msg2;
			msg2.set_info(pMsg->info());
			m_client2.async_call(&msg2, sFuture, 10);

			return sFuture;
		}).then([this, sSessionInfo](response_msg2* pMsg, uint32_t nErrorCode)
		{
			response_msg rmsg;
			rmsg.set_info(pMsg->info() + "[fun1]");
			this->m_service.response(sSessionInfo, &rmsg);
		});

		return false;
	}
	
#### 异步转同步串行
    bool fun2(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);
		sFuture1.wait();
		auto pMsg1 = sFuture1.getValue();
		if (nullptr == pMsg1)
			return false;
		
		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);
		sFuture2.wait();
		auto pMsg2 = sFuture2.getValue();
		if (nullptr == pMsg2)
			return false;

		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun2]");

		return true;
	}
	
#### 同步串行
    bool fun3(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		request_msg1 msg1;
		std::shared_ptr<response_msg1> pMsg1 = nullptr;
		msg1.set_info(pRequest->info());
		m_client1.sync_call(&msg1, pMsg1, 10);
		if (nullptr == pMsg1)
			return false;

		request_msg2 msg2;
		std::shared_ptr<response_msg2> pMsg2 = nullptr;
		msg2.set_info(pRequest->info());
		m_client2.sync_call(&msg2, pMsg2, 10);
		if (nullptr == pMsg2)
			return false;
		
		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun3]");

		return true;
	}
	
#### 异步并行
    bool fun4(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);

		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);

		auto sFuture = rpc::whenAll(sFuture1, sFuture2);

		sFuture.then([sSessionInfo, this](std::tuple<rpc::CFuture<response_msg1>, rpc::CFuture<response_msg2>>* all, uint32_t nErrorCode)
		{
			auto pMsg1 = std::get<0>(*all).getValue();
			auto pMsg2 = std::get<1>(*all).getValue();
			if (nullptr == pMsg1 || nullptr == pMsg2)
				return;

			response_msg rmsg;
			rmsg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun4]");
			this->m_service.response(sSessionInfo, &rmsg);
		});

		return false;
	}
	
#### 异步转同步并行
    bool fun5(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);

		rpc::CFuture<response_msg2> sFuture2;
		request_msg2 msg2;
		msg2.set_info(pRequest->info());
		m_client2.async_call(&msg2, sFuture2, 10);

		sFuture1.wait();
		sFuture2.wait();

		auto pMsg1 = sFuture1.getValue();
		auto pMsg2 = sFuture2.getValue();
		if (nullptr == pMsg1 || nullptr == pMsg2)
			return false;

		msg.set_info(pMsg1->info() + "|" + pMsg2->info() + "[fun5]");

		return true;
	}
	
#### 异步串行2
    bool fun6(request_msg* pRequest, response_msg& msg, const rpc::SSessionInfo& sSessionInfo)
	{
		rpc::CFuture<response_msg1> sFuture1;
		request_msg1 msg1;
		msg1.set_info(pRequest->info());
		m_client1.async_call(&msg1, sFuture1, 10);
		sFuture1
			.then_r(std::bind(&CServiceProxy::ff1, this, std::placeholders::_1, std::placeholders::_2))
			.then(std::bind(&CServiceProxy::ff2, this, sSessionInfo, std::placeholders::_1, std::placeholders::_2));

		return false;
	}

	rpc::CFuture<response_msg2> ff1(response_msg1* pMsg, uint32_t nErrorCode)
	{
		rpc::CFuture<response_msg2> sFuture;
		request_msg2 msg2;
		msg2.set_info(pMsg->info());
		m_client2.async_call(&msg2, sFuture, 10);

		return sFuture;
	}

	void ff2(const rpc::SSessionInfo& sSessionInfo, response_msg2* pMsg, uint32_t nErrorCode)
	{
		response_msg rmsg;
		rmsg.set_info(pMsg->info() + "[fun6]");
		this->m_service.response(sSessionInfo, &rmsg);
	}
	
详细的例子见test_srpc例子