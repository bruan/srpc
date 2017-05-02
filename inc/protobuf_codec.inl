namespace rpc
{
	namespace codec
	{
		template<class T>
		bool CProtobufCodec::serializeRequest(T* pMessage, SRequest& sRequest)
		{
			if (!pMessage->SerializeToString(&this->m_szBuf))
				return false;

			sRequest.nID = getID(pMessage->GetTypeName().c_str());
			sRequest.szData = this->m_szBuf.c_str();
			sRequest.nDataSize = (uint32_t)this->m_szBuf.size();

			return true;
		}

		template<class T>
		bool CProtobufCodec::serializeResponse(T* pMessage, SResponse& sResponse)
		{
			if (!pMessage->SerializeToString(&this->m_szBuf))
				return false;

			sResponse.szName = pMessage->GetTypeName();
			sResponse.szData = this->m_szBuf.c_str();
			sResponse.nDataSize = (uint32_t)this->m_szBuf.size();

			return true;
		}

		template<class T>
		bool CProtobufCodec::unserialize(const char* szName, const char* szBuf, uint32_t nBufSize, T*& pMessage)
		{
			const google::protobuf::Descriptor* pDescriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(szName);
			if (nullptr == pDescriptor)
				return false;

			google::protobuf::Message* pNewMessage = nullptr;
			const google::protobuf::Message* pPrototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(pDescriptor);
			if (pPrototype != nullptr)
				pNewMessage = pPrototype->New();

			if (!pNewMessage->ParseFromArray(szBuf, nBufSize))
			{
				delete pNewMessage;
				return false;
			}

			pMessage = dynamic_cast<T*>(pNewMessage);
			if (pMessage == nullptr)
			{
				delete pNewMessage;
				return false;
			}

			return true;
		}
	}
}