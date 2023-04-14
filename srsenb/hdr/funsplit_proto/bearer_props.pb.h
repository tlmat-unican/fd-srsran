// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bearer_props.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_bearer_5fprops_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_bearer_5fprops_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_bearer_5fprops_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_bearer_5fprops_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_bearer_5fprops_2eproto;
namespace fs {
namespace gtpu_rrc {
class bearer_props;
class bearer_propsDefaultTypeInternal;
extern bearer_propsDefaultTypeInternal _bearer_props_default_instance_;
}  // namespace gtpu_rrc
}  // namespace fs
PROTOBUF_NAMESPACE_OPEN
template<> ::fs::gtpu_rrc::bearer_props* Arena::CreateMaybeMessage<::fs::gtpu_rrc::bearer_props>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace fs {
namespace gtpu_rrc {

// ===================================================================

class bearer_props PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fs.gtpu_rrc.bearer_props) */ {
 public:
  inline bearer_props() : bearer_props(nullptr) {};
  virtual ~bearer_props();

  bearer_props(const bearer_props& from);
  bearer_props(bearer_props&& from) noexcept
    : bearer_props() {
    *this = ::std::move(from);
  }

  inline bearer_props& operator=(const bearer_props& from) {
    CopyFrom(from);
    return *this;
  }
  inline bearer_props& operator=(bearer_props&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const bearer_props& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const bearer_props* internal_default_instance() {
    return reinterpret_cast<const bearer_props*>(
               &_bearer_props_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(bearer_props& a, bearer_props& b) {
    a.Swap(&b);
  }
  inline void Swap(bearer_props* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(bearer_props* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline bearer_props* New() const final {
    return CreateMaybeMessage<bearer_props>(nullptr);
  }

  bearer_props* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<bearer_props>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const bearer_props& from);
  void MergeFrom(const bearer_props& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(bearer_props* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fs.gtpu_rrc.bearer_props";
  }
  protected:
  explicit bearer_props(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_bearer_5fprops_2eproto);
    return ::descriptor_table_bearer_5fprops_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kForwardFromTeidinPresentFieldNumber = 1,
    kFlushBeforeTeidinPresentFieldNumber = 2,
    kForwardFromTeidinFieldNumber = 3,
    kFlushFromTeidinFieldNumber = 4,
  };
  // bool forward_from_teidin_present = 1;
  void clear_forward_from_teidin_present();
  bool forward_from_teidin_present() const;
  void set_forward_from_teidin_present(bool value);
  private:
  bool _internal_forward_from_teidin_present() const;
  void _internal_set_forward_from_teidin_present(bool value);
  public:

  // bool flush_before_teidin_present = 2;
  void clear_flush_before_teidin_present();
  bool flush_before_teidin_present() const;
  void set_flush_before_teidin_present(bool value);
  private:
  bool _internal_flush_before_teidin_present() const;
  void _internal_set_flush_before_teidin_present(bool value);
  public:

  // uint32 forward_from_teidin = 3;
  void clear_forward_from_teidin();
  ::PROTOBUF_NAMESPACE_ID::uint32 forward_from_teidin() const;
  void set_forward_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_forward_from_teidin() const;
  void _internal_set_forward_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 flush_from_teidin = 4;
  void clear_flush_from_teidin();
  ::PROTOBUF_NAMESPACE_ID::uint32 flush_from_teidin() const;
  void set_flush_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_flush_from_teidin() const;
  void _internal_set_flush_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:fs.gtpu_rrc.bearer_props)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  bool forward_from_teidin_present_;
  bool flush_before_teidin_present_;
  ::PROTOBUF_NAMESPACE_ID::uint32 forward_from_teidin_;
  ::PROTOBUF_NAMESPACE_ID::uint32 flush_from_teidin_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_bearer_5fprops_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// bearer_props

// bool forward_from_teidin_present = 1;
inline void bearer_props::clear_forward_from_teidin_present() {
  forward_from_teidin_present_ = false;
}
inline bool bearer_props::_internal_forward_from_teidin_present() const {
  return forward_from_teidin_present_;
}
inline bool bearer_props::forward_from_teidin_present() const {
  // @@protoc_insertion_point(field_get:fs.gtpu_rrc.bearer_props.forward_from_teidin_present)
  return _internal_forward_from_teidin_present();
}
inline void bearer_props::_internal_set_forward_from_teidin_present(bool value) {
  
  forward_from_teidin_present_ = value;
}
inline void bearer_props::set_forward_from_teidin_present(bool value) {
  _internal_set_forward_from_teidin_present(value);
  // @@protoc_insertion_point(field_set:fs.gtpu_rrc.bearer_props.forward_from_teidin_present)
}

// bool flush_before_teidin_present = 2;
inline void bearer_props::clear_flush_before_teidin_present() {
  flush_before_teidin_present_ = false;
}
inline bool bearer_props::_internal_flush_before_teidin_present() const {
  return flush_before_teidin_present_;
}
inline bool bearer_props::flush_before_teidin_present() const {
  // @@protoc_insertion_point(field_get:fs.gtpu_rrc.bearer_props.flush_before_teidin_present)
  return _internal_flush_before_teidin_present();
}
inline void bearer_props::_internal_set_flush_before_teidin_present(bool value) {
  
  flush_before_teidin_present_ = value;
}
inline void bearer_props::set_flush_before_teidin_present(bool value) {
  _internal_set_flush_before_teidin_present(value);
  // @@protoc_insertion_point(field_set:fs.gtpu_rrc.bearer_props.flush_before_teidin_present)
}

// uint32 forward_from_teidin = 3;
inline void bearer_props::clear_forward_from_teidin() {
  forward_from_teidin_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 bearer_props::_internal_forward_from_teidin() const {
  return forward_from_teidin_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 bearer_props::forward_from_teidin() const {
  // @@protoc_insertion_point(field_get:fs.gtpu_rrc.bearer_props.forward_from_teidin)
  return _internal_forward_from_teidin();
}
inline void bearer_props::_internal_set_forward_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  forward_from_teidin_ = value;
}
inline void bearer_props::set_forward_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_forward_from_teidin(value);
  // @@protoc_insertion_point(field_set:fs.gtpu_rrc.bearer_props.forward_from_teidin)
}

// uint32 flush_from_teidin = 4;
inline void bearer_props::clear_flush_from_teidin() {
  flush_from_teidin_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 bearer_props::_internal_flush_from_teidin() const {
  return flush_from_teidin_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 bearer_props::flush_from_teidin() const {
  // @@protoc_insertion_point(field_get:fs.gtpu_rrc.bearer_props.flush_from_teidin)
  return _internal_flush_from_teidin();
}
inline void bearer_props::_internal_set_flush_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  flush_from_teidin_ = value;
}
inline void bearer_props::set_flush_from_teidin(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_flush_from_teidin(value);
  // @@protoc_insertion_point(field_set:fs.gtpu_rrc.bearer_props.flush_from_teidin)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace gtpu_rrc
}  // namespace fs

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_bearer_5fprops_2eproto
