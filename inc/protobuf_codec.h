#pragma once
#include "rpc_base.h"

#include "google/protobuf/message.h"

namespace rpc
{
	namespace codec
	{
		class CProtobufCodec
		{
		public:
			CProtobufCodec() { }
			~CProtobufCodec() { }

			template<class T>
			bool	serializeRequest(T* pMessage, SRequest& sRequest);
			
			template<class T>
			bool	serializeResponse(T* pMessage, SResponse& sResponse);
			
			template<class T>
			bool	unserialize(const char* szName, const char* szBuf, uint32_t nBufSize, T*& pMessage);

		private:
			std::string	m_szBuf;
		};
	}
}

#include "protobuf_codec.inl"