// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: response_msg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "response_msg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* response_msg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  response_msg_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_response_5fmsg_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_response_5fmsg_2eproto() {
  protobuf_AddDesc_response_5fmsg_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "response_msg.proto");
  GOOGLE_CHECK(file != NULL);
  response_msg_descriptor_ = file->message_type(0);
  static const int response_msg_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(response_msg, info_),
  };
  response_msg_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      response_msg_descriptor_,
      response_msg::default_instance_,
      response_msg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(response_msg, _has_bits_[0]),
      -1,
      -1,
      sizeof(response_msg),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(response_msg, _internal_metadata_),
      -1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_response_5fmsg_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      response_msg_descriptor_, &response_msg::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_response_5fmsg_2eproto() {
  delete response_msg::default_instance_;
  delete response_msg_reflection_;
}

void protobuf_AddDesc_response_5fmsg_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_response_5fmsg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022response_msg.proto\"\034\n\014response_msg\022\014\n\004"
    "info\030\001 \002(\t", 50);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "response_msg.proto", &protobuf_RegisterTypes);
  response_msg::default_instance_ = new response_msg();
  response_msg::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_response_5fmsg_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_response_5fmsg_2eproto {
  StaticDescriptorInitializer_response_5fmsg_2eproto() {
    protobuf_AddDesc_response_5fmsg_2eproto();
  }
} static_descriptor_initializer_response_5fmsg_2eproto_;

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int response_msg::kInfoFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

response_msg::response_msg()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:response_msg)
}

void response_msg::InitAsDefaultInstance() {
}

response_msg::response_msg(const response_msg& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:response_msg)
}

void response_msg::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  info_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

response_msg::~response_msg() {
  // @@protoc_insertion_point(destructor:response_msg)
  SharedDtor();
}

void response_msg::SharedDtor() {
  info_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void response_msg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* response_msg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return response_msg_descriptor_;
}

const response_msg& response_msg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_response_5fmsg_2eproto();
  return *default_instance_;
}

response_msg* response_msg::default_instance_ = NULL;

response_msg* response_msg::New(::google::protobuf::Arena* arena) const {
  response_msg* n = new response_msg;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void response_msg::Clear() {
// @@protoc_insertion_point(message_clear_start:response_msg)
  if (has_info()) {
    info_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool response_msg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:response_msg)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string info = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_info()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->info().data(), this->info().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "response_msg.info");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:response_msg)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:response_msg)
  return false;
#undef DO_
}

void response_msg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:response_msg)
  // required string info = 1;
  if (has_info()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->info().data(), this->info().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "response_msg.info");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->info(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:response_msg)
}

::google::protobuf::uint8* response_msg::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:response_msg)
  // required string info = 1;
  if (has_info()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->info().data(), this->info().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "response_msg.info");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->info(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:response_msg)
  return target;
}

int response_msg::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:response_msg)
  int total_size = 0;

  // required string info = 1;
  if (has_info()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->info());
  }
  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void response_msg::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:response_msg)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const response_msg* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const response_msg>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:response_msg)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:response_msg)
    MergeFrom(*source);
  }
}

void response_msg::MergeFrom(const response_msg& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:response_msg)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_info()) {
      set_has_info();
      info_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.info_);
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void response_msg::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:response_msg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void response_msg::CopyFrom(const response_msg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:response_msg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool response_msg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void response_msg::Swap(response_msg* other) {
  if (other == this) return;
  InternalSwap(other);
}
void response_msg::InternalSwap(response_msg* other) {
  info_.Swap(&other->info_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata response_msg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = response_msg_descriptor_;
  metadata.reflection = response_msg_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// response_msg

// required string info = 1;
bool response_msg::has_info() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void response_msg::set_has_info() {
  _has_bits_[0] |= 0x00000001u;
}
void response_msg::clear_has_info() {
  _has_bits_[0] &= ~0x00000001u;
}
void response_msg::clear_info() {
  info_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_info();
}
 const ::std::string& response_msg::info() const {
  // @@protoc_insertion_point(field_get:response_msg.info)
  return info_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void response_msg::set_info(const ::std::string& value) {
  set_has_info();
  info_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:response_msg.info)
}
 void response_msg::set_info(const char* value) {
  set_has_info();
  info_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:response_msg.info)
}
 void response_msg::set_info(const char* value, size_t size) {
  set_has_info();
  info_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:response_msg.info)
}
 ::std::string* response_msg::mutable_info() {
  set_has_info();
  // @@protoc_insertion_point(field_mutable:response_msg.info)
  return info_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* response_msg::release_info() {
  // @@protoc_insertion_point(field_release:response_msg.info)
  clear_has_info();
  return info_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void response_msg::set_allocated_info(::std::string* info) {
  if (info != NULL) {
    set_has_info();
  } else {
    clear_has_info();
  }
  info_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), info);
  // @@protoc_insertion_point(field_set_allocated:response_msg.info)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
