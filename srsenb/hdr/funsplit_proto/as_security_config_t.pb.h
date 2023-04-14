// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: as_security_config_t.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_as_5fsecurity_5fconfig_5ft_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_as_5fsecurity_5fconfig_5ft_2eproto

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_as_5fsecurity_5fconfig_5ft_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_as_5fsecurity_5fconfig_5ft_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_as_5fsecurity_5fconfig_5ft_2eproto;
namespace fs {
namespace pdcp_rrc {
class as_key_t;
class as_key_tDefaultTypeInternal;
extern as_key_tDefaultTypeInternal _as_key_t_default_instance_;
class as_security_config_t;
class as_security_config_tDefaultTypeInternal;
extern as_security_config_tDefaultTypeInternal _as_security_config_t_default_instance_;
}  // namespace pdcp_rrc
}  // namespace fs
PROTOBUF_NAMESPACE_OPEN
template<> ::fs::pdcp_rrc::as_key_t* Arena::CreateMaybeMessage<::fs::pdcp_rrc::as_key_t>(Arena*);
template<> ::fs::pdcp_rrc::as_security_config_t* Arena::CreateMaybeMessage<::fs::pdcp_rrc::as_security_config_t>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace fs {
namespace pdcp_rrc {

enum INTEGRITY_ALGORITHM_ID_ENUM : int {
  INTEGRITY_ALGORITHM_ID_EIA0 = 0,
  INTEGRITY_ALGORITHM_ID_128_EIA1 = 1,
  INTEGRITY_ALGORITHM_ID_128_EIA2 = 2,
  INTEGRITY_ALGORITHM_ID_128_EIA3 = 3,
  INTEGRITY_ALGORITHM_ID_N_ITEMS = 4,
  INTEGRITY_ALGORITHM_ID_ENUM_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  INTEGRITY_ALGORITHM_ID_ENUM_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool INTEGRITY_ALGORITHM_ID_ENUM_IsValid(int value);
constexpr INTEGRITY_ALGORITHM_ID_ENUM INTEGRITY_ALGORITHM_ID_ENUM_MIN = INTEGRITY_ALGORITHM_ID_EIA0;
constexpr INTEGRITY_ALGORITHM_ID_ENUM INTEGRITY_ALGORITHM_ID_ENUM_MAX = INTEGRITY_ALGORITHM_ID_N_ITEMS;
constexpr int INTEGRITY_ALGORITHM_ID_ENUM_ARRAYSIZE = INTEGRITY_ALGORITHM_ID_ENUM_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* INTEGRITY_ALGORITHM_ID_ENUM_descriptor();
template<typename T>
inline const std::string& INTEGRITY_ALGORITHM_ID_ENUM_Name(T enum_t_value) {
  static_assert(::std::is_same<T, INTEGRITY_ALGORITHM_ID_ENUM>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function INTEGRITY_ALGORITHM_ID_ENUM_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    INTEGRITY_ALGORITHM_ID_ENUM_descriptor(), enum_t_value);
}
inline bool INTEGRITY_ALGORITHM_ID_ENUM_Parse(
    const std::string& name, INTEGRITY_ALGORITHM_ID_ENUM* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<INTEGRITY_ALGORITHM_ID_ENUM>(
    INTEGRITY_ALGORITHM_ID_ENUM_descriptor(), name, value);
}
enum CIPHERING_ALGORITHM_ID_ENUM : int {
  CIPHERING_ALGORITHM_ID_EEA0 = 0,
  CIPHERING_ALGORITHM_ID_128_EEA1 = 1,
  CIPHERING_ALGORITHM_ID_128_EEA2 = 2,
  CIPHERING_ALGORITHM_ID_128_EEA3 = 3,
  CIPHERING_ALGORITHM_ID_N_ITEMS = 4,
  CIPHERING_ALGORITHM_ID_ENUM_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  CIPHERING_ALGORITHM_ID_ENUM_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool CIPHERING_ALGORITHM_ID_ENUM_IsValid(int value);
constexpr CIPHERING_ALGORITHM_ID_ENUM CIPHERING_ALGORITHM_ID_ENUM_MIN = CIPHERING_ALGORITHM_ID_EEA0;
constexpr CIPHERING_ALGORITHM_ID_ENUM CIPHERING_ALGORITHM_ID_ENUM_MAX = CIPHERING_ALGORITHM_ID_N_ITEMS;
constexpr int CIPHERING_ALGORITHM_ID_ENUM_ARRAYSIZE = CIPHERING_ALGORITHM_ID_ENUM_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CIPHERING_ALGORITHM_ID_ENUM_descriptor();
template<typename T>
inline const std::string& CIPHERING_ALGORITHM_ID_ENUM_Name(T enum_t_value) {
  static_assert(::std::is_same<T, CIPHERING_ALGORITHM_ID_ENUM>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function CIPHERING_ALGORITHM_ID_ENUM_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    CIPHERING_ALGORITHM_ID_ENUM_descriptor(), enum_t_value);
}
inline bool CIPHERING_ALGORITHM_ID_ENUM_Parse(
    const std::string& name, CIPHERING_ALGORITHM_ID_ENUM* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<CIPHERING_ALGORITHM_ID_ENUM>(
    CIPHERING_ALGORITHM_ID_ENUM_descriptor(), name, value);
}
// ===================================================================

class as_key_t PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fs.pdcp_rrc.as_key_t) */ {
 public:
  inline as_key_t() : as_key_t(nullptr) {};
  virtual ~as_key_t();

  as_key_t(const as_key_t& from);
  as_key_t(as_key_t&& from) noexcept
    : as_key_t() {
    *this = ::std::move(from);
  }

  inline as_key_t& operator=(const as_key_t& from) {
    CopyFrom(from);
    return *this;
  }
  inline as_key_t& operator=(as_key_t&& from) noexcept {
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
  static const as_key_t& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const as_key_t* internal_default_instance() {
    return reinterpret_cast<const as_key_t*>(
               &_as_key_t_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(as_key_t& a, as_key_t& b) {
    a.Swap(&b);
  }
  inline void Swap(as_key_t* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(as_key_t* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline as_key_t* New() const final {
    return CreateMaybeMessage<as_key_t>(nullptr);
  }

  as_key_t* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<as_key_t>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const as_key_t& from);
  void MergeFrom(const as_key_t& from);
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
  void InternalSwap(as_key_t* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fs.pdcp_rrc.as_key_t";
  }
  protected:
  explicit as_key_t(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_as_5fsecurity_5fconfig_5ft_2eproto);
    return ::descriptor_table_as_5fsecurity_5fconfig_5ft_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kKeyFieldNumber = 1,
  };
  // bytes key = 1;
  void clear_key();
  const std::string& key() const;
  void set_key(const std::string& value);
  void set_key(std::string&& value);
  void set_key(const char* value);
  void set_key(const void* value, size_t size);
  std::string* mutable_key();
  std::string* release_key();
  void set_allocated_key(std::string* key);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_key();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_key(
      std::string* key);
  private:
  const std::string& _internal_key() const;
  void _internal_set_key(const std::string& value);
  std::string* _internal_mutable_key();
  public:

  // @@protoc_insertion_point(class_scope:fs.pdcp_rrc.as_key_t)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr key_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_as_5fsecurity_5fconfig_5ft_2eproto;
};
// -------------------------------------------------------------------

class as_security_config_t PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fs.pdcp_rrc.as_security_config_t) */ {
 public:
  inline as_security_config_t() : as_security_config_t(nullptr) {};
  virtual ~as_security_config_t();

  as_security_config_t(const as_security_config_t& from);
  as_security_config_t(as_security_config_t&& from) noexcept
    : as_security_config_t() {
    *this = ::std::move(from);
  }

  inline as_security_config_t& operator=(const as_security_config_t& from) {
    CopyFrom(from);
    return *this;
  }
  inline as_security_config_t& operator=(as_security_config_t&& from) noexcept {
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
  static const as_security_config_t& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const as_security_config_t* internal_default_instance() {
    return reinterpret_cast<const as_security_config_t*>(
               &_as_security_config_t_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(as_security_config_t& a, as_security_config_t& b) {
    a.Swap(&b);
  }
  inline void Swap(as_security_config_t* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(as_security_config_t* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline as_security_config_t* New() const final {
    return CreateMaybeMessage<as_security_config_t>(nullptr);
  }

  as_security_config_t* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<as_security_config_t>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const as_security_config_t& from);
  void MergeFrom(const as_security_config_t& from);
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
  void InternalSwap(as_security_config_t* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fs.pdcp_rrc.as_security_config_t";
  }
  protected:
  explicit as_security_config_t(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_as_5fsecurity_5fconfig_5ft_2eproto);
    return ::descriptor_table_as_5fsecurity_5fconfig_5ft_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kKRrcIntFieldNumber = 1,
    kKRrcEncFieldNumber = 2,
    kKUpIntFieldNumber = 3,
    kKUpEncFieldNumber = 4,
    kIntegAlgoFieldNumber = 5,
    kCipherAlgoFieldNumber = 6,
  };
  // .fs.pdcp_rrc.as_key_t k_rrc_int = 1;
  bool has_k_rrc_int() const;
  private:
  bool _internal_has_k_rrc_int() const;
  public:
  void clear_k_rrc_int();
  const ::fs::pdcp_rrc::as_key_t& k_rrc_int() const;
  ::fs::pdcp_rrc::as_key_t* release_k_rrc_int();
  ::fs::pdcp_rrc::as_key_t* mutable_k_rrc_int();
  void set_allocated_k_rrc_int(::fs::pdcp_rrc::as_key_t* k_rrc_int);
  private:
  const ::fs::pdcp_rrc::as_key_t& _internal_k_rrc_int() const;
  ::fs::pdcp_rrc::as_key_t* _internal_mutable_k_rrc_int();
  public:
  void unsafe_arena_set_allocated_k_rrc_int(
      ::fs::pdcp_rrc::as_key_t* k_rrc_int);
  ::fs::pdcp_rrc::as_key_t* unsafe_arena_release_k_rrc_int();

  // .fs.pdcp_rrc.as_key_t k_rrc_enc = 2;
  bool has_k_rrc_enc() const;
  private:
  bool _internal_has_k_rrc_enc() const;
  public:
  void clear_k_rrc_enc();
  const ::fs::pdcp_rrc::as_key_t& k_rrc_enc() const;
  ::fs::pdcp_rrc::as_key_t* release_k_rrc_enc();
  ::fs::pdcp_rrc::as_key_t* mutable_k_rrc_enc();
  void set_allocated_k_rrc_enc(::fs::pdcp_rrc::as_key_t* k_rrc_enc);
  private:
  const ::fs::pdcp_rrc::as_key_t& _internal_k_rrc_enc() const;
  ::fs::pdcp_rrc::as_key_t* _internal_mutable_k_rrc_enc();
  public:
  void unsafe_arena_set_allocated_k_rrc_enc(
      ::fs::pdcp_rrc::as_key_t* k_rrc_enc);
  ::fs::pdcp_rrc::as_key_t* unsafe_arena_release_k_rrc_enc();

  // .fs.pdcp_rrc.as_key_t k_up_int = 3;
  bool has_k_up_int() const;
  private:
  bool _internal_has_k_up_int() const;
  public:
  void clear_k_up_int();
  const ::fs::pdcp_rrc::as_key_t& k_up_int() const;
  ::fs::pdcp_rrc::as_key_t* release_k_up_int();
  ::fs::pdcp_rrc::as_key_t* mutable_k_up_int();
  void set_allocated_k_up_int(::fs::pdcp_rrc::as_key_t* k_up_int);
  private:
  const ::fs::pdcp_rrc::as_key_t& _internal_k_up_int() const;
  ::fs::pdcp_rrc::as_key_t* _internal_mutable_k_up_int();
  public:
  void unsafe_arena_set_allocated_k_up_int(
      ::fs::pdcp_rrc::as_key_t* k_up_int);
  ::fs::pdcp_rrc::as_key_t* unsafe_arena_release_k_up_int();

  // .fs.pdcp_rrc.as_key_t k_up_enc = 4;
  bool has_k_up_enc() const;
  private:
  bool _internal_has_k_up_enc() const;
  public:
  void clear_k_up_enc();
  const ::fs::pdcp_rrc::as_key_t& k_up_enc() const;
  ::fs::pdcp_rrc::as_key_t* release_k_up_enc();
  ::fs::pdcp_rrc::as_key_t* mutable_k_up_enc();
  void set_allocated_k_up_enc(::fs::pdcp_rrc::as_key_t* k_up_enc);
  private:
  const ::fs::pdcp_rrc::as_key_t& _internal_k_up_enc() const;
  ::fs::pdcp_rrc::as_key_t* _internal_mutable_k_up_enc();
  public:
  void unsafe_arena_set_allocated_k_up_enc(
      ::fs::pdcp_rrc::as_key_t* k_up_enc);
  ::fs::pdcp_rrc::as_key_t* unsafe_arena_release_k_up_enc();

  // .fs.pdcp_rrc.INTEGRITY_ALGORITHM_ID_ENUM integ_algo = 5;
  void clear_integ_algo();
  ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM integ_algo() const;
  void set_integ_algo(::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM value);
  private:
  ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM _internal_integ_algo() const;
  void _internal_set_integ_algo(::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM value);
  public:

  // .fs.pdcp_rrc.CIPHERING_ALGORITHM_ID_ENUM cipher_algo = 6;
  void clear_cipher_algo();
  ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM cipher_algo() const;
  void set_cipher_algo(::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM value);
  private:
  ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM _internal_cipher_algo() const;
  void _internal_set_cipher_algo(::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM value);
  public:

  // @@protoc_insertion_point(class_scope:fs.pdcp_rrc.as_security_config_t)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::fs::pdcp_rrc::as_key_t* k_rrc_int_;
  ::fs::pdcp_rrc::as_key_t* k_rrc_enc_;
  ::fs::pdcp_rrc::as_key_t* k_up_int_;
  ::fs::pdcp_rrc::as_key_t* k_up_enc_;
  int integ_algo_;
  int cipher_algo_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_as_5fsecurity_5fconfig_5ft_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// as_key_t

// bytes key = 1;
inline void as_key_t::clear_key() {
  key_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& as_key_t::key() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_key_t.key)
  return _internal_key();
}
inline void as_key_t::set_key(const std::string& value) {
  _internal_set_key(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.as_key_t.key)
}
inline std::string* as_key_t::mutable_key() {
  // @@protoc_insertion_point(field_mutable:fs.pdcp_rrc.as_key_t.key)
  return _internal_mutable_key();
}
inline const std::string& as_key_t::_internal_key() const {
  return key_.Get();
}
inline void as_key_t::_internal_set_key(const std::string& value) {
  
  key_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void as_key_t::set_key(std::string&& value) {
  
  key_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:fs.pdcp_rrc.as_key_t.key)
}
inline void as_key_t::set_key(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  key_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:fs.pdcp_rrc.as_key_t.key)
}
inline void as_key_t::set_key(const void* value,
    size_t size) {
  
  key_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:fs.pdcp_rrc.as_key_t.key)
}
inline std::string* as_key_t::_internal_mutable_key() {
  
  return key_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* as_key_t::release_key() {
  // @@protoc_insertion_point(field_release:fs.pdcp_rrc.as_key_t.key)
  return key_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void as_key_t::set_allocated_key(std::string* key) {
  if (key != nullptr) {
    
  } else {
    
  }
  key_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), key,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:fs.pdcp_rrc.as_key_t.key)
}
inline std::string* as_key_t::unsafe_arena_release_key() {
  // @@protoc_insertion_point(field_unsafe_arena_release:fs.pdcp_rrc.as_key_t.key)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return key_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void as_key_t::unsafe_arena_set_allocated_key(
    std::string* key) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (key != nullptr) {
    
  } else {
    
  }
  key_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      key, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:fs.pdcp_rrc.as_key_t.key)
}

// -------------------------------------------------------------------

// as_security_config_t

// .fs.pdcp_rrc.as_key_t k_rrc_int = 1;
inline bool as_security_config_t::_internal_has_k_rrc_int() const {
  return this != internal_default_instance() && k_rrc_int_ != nullptr;
}
inline bool as_security_config_t::has_k_rrc_int() const {
  return _internal_has_k_rrc_int();
}
inline void as_security_config_t::clear_k_rrc_int() {
  if (GetArena() == nullptr && k_rrc_int_ != nullptr) {
    delete k_rrc_int_;
  }
  k_rrc_int_ = nullptr;
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::_internal_k_rrc_int() const {
  const ::fs::pdcp_rrc::as_key_t* p = k_rrc_int_;
  return p != nullptr ? *p : *reinterpret_cast<const ::fs::pdcp_rrc::as_key_t*>(
      &::fs::pdcp_rrc::_as_key_t_default_instance_);
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::k_rrc_int() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.k_rrc_int)
  return _internal_k_rrc_int();
}
inline void as_security_config_t::unsafe_arena_set_allocated_k_rrc_int(
    ::fs::pdcp_rrc::as_key_t* k_rrc_int) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(k_rrc_int_);
  }
  k_rrc_int_ = k_rrc_int;
  if (k_rrc_int) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:fs.pdcp_rrc.as_security_config_t.k_rrc_int)
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::release_k_rrc_int() {
  auto temp = unsafe_arena_release_k_rrc_int();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::unsafe_arena_release_k_rrc_int() {
  // @@protoc_insertion_point(field_release:fs.pdcp_rrc.as_security_config_t.k_rrc_int)
  
  ::fs::pdcp_rrc::as_key_t* temp = k_rrc_int_;
  k_rrc_int_ = nullptr;
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::_internal_mutable_k_rrc_int() {
  
  if (k_rrc_int_ == nullptr) {
    auto* p = CreateMaybeMessage<::fs::pdcp_rrc::as_key_t>(GetArena());
    k_rrc_int_ = p;
  }
  return k_rrc_int_;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::mutable_k_rrc_int() {
  // @@protoc_insertion_point(field_mutable:fs.pdcp_rrc.as_security_config_t.k_rrc_int)
  return _internal_mutable_k_rrc_int();
}
inline void as_security_config_t::set_allocated_k_rrc_int(::fs::pdcp_rrc::as_key_t* k_rrc_int) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete k_rrc_int_;
  }
  if (k_rrc_int) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(k_rrc_int);
    if (message_arena != submessage_arena) {
      k_rrc_int = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, k_rrc_int, submessage_arena);
    }
    
  } else {
    
  }
  k_rrc_int_ = k_rrc_int;
  // @@protoc_insertion_point(field_set_allocated:fs.pdcp_rrc.as_security_config_t.k_rrc_int)
}

// .fs.pdcp_rrc.as_key_t k_rrc_enc = 2;
inline bool as_security_config_t::_internal_has_k_rrc_enc() const {
  return this != internal_default_instance() && k_rrc_enc_ != nullptr;
}
inline bool as_security_config_t::has_k_rrc_enc() const {
  return _internal_has_k_rrc_enc();
}
inline void as_security_config_t::clear_k_rrc_enc() {
  if (GetArena() == nullptr && k_rrc_enc_ != nullptr) {
    delete k_rrc_enc_;
  }
  k_rrc_enc_ = nullptr;
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::_internal_k_rrc_enc() const {
  const ::fs::pdcp_rrc::as_key_t* p = k_rrc_enc_;
  return p != nullptr ? *p : *reinterpret_cast<const ::fs::pdcp_rrc::as_key_t*>(
      &::fs::pdcp_rrc::_as_key_t_default_instance_);
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::k_rrc_enc() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.k_rrc_enc)
  return _internal_k_rrc_enc();
}
inline void as_security_config_t::unsafe_arena_set_allocated_k_rrc_enc(
    ::fs::pdcp_rrc::as_key_t* k_rrc_enc) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(k_rrc_enc_);
  }
  k_rrc_enc_ = k_rrc_enc;
  if (k_rrc_enc) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:fs.pdcp_rrc.as_security_config_t.k_rrc_enc)
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::release_k_rrc_enc() {
  auto temp = unsafe_arena_release_k_rrc_enc();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::unsafe_arena_release_k_rrc_enc() {
  // @@protoc_insertion_point(field_release:fs.pdcp_rrc.as_security_config_t.k_rrc_enc)
  
  ::fs::pdcp_rrc::as_key_t* temp = k_rrc_enc_;
  k_rrc_enc_ = nullptr;
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::_internal_mutable_k_rrc_enc() {
  
  if (k_rrc_enc_ == nullptr) {
    auto* p = CreateMaybeMessage<::fs::pdcp_rrc::as_key_t>(GetArena());
    k_rrc_enc_ = p;
  }
  return k_rrc_enc_;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::mutable_k_rrc_enc() {
  // @@protoc_insertion_point(field_mutable:fs.pdcp_rrc.as_security_config_t.k_rrc_enc)
  return _internal_mutable_k_rrc_enc();
}
inline void as_security_config_t::set_allocated_k_rrc_enc(::fs::pdcp_rrc::as_key_t* k_rrc_enc) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete k_rrc_enc_;
  }
  if (k_rrc_enc) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(k_rrc_enc);
    if (message_arena != submessage_arena) {
      k_rrc_enc = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, k_rrc_enc, submessage_arena);
    }
    
  } else {
    
  }
  k_rrc_enc_ = k_rrc_enc;
  // @@protoc_insertion_point(field_set_allocated:fs.pdcp_rrc.as_security_config_t.k_rrc_enc)
}

// .fs.pdcp_rrc.as_key_t k_up_int = 3;
inline bool as_security_config_t::_internal_has_k_up_int() const {
  return this != internal_default_instance() && k_up_int_ != nullptr;
}
inline bool as_security_config_t::has_k_up_int() const {
  return _internal_has_k_up_int();
}
inline void as_security_config_t::clear_k_up_int() {
  if (GetArena() == nullptr && k_up_int_ != nullptr) {
    delete k_up_int_;
  }
  k_up_int_ = nullptr;
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::_internal_k_up_int() const {
  const ::fs::pdcp_rrc::as_key_t* p = k_up_int_;
  return p != nullptr ? *p : *reinterpret_cast<const ::fs::pdcp_rrc::as_key_t*>(
      &::fs::pdcp_rrc::_as_key_t_default_instance_);
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::k_up_int() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.k_up_int)
  return _internal_k_up_int();
}
inline void as_security_config_t::unsafe_arena_set_allocated_k_up_int(
    ::fs::pdcp_rrc::as_key_t* k_up_int) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(k_up_int_);
  }
  k_up_int_ = k_up_int;
  if (k_up_int) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:fs.pdcp_rrc.as_security_config_t.k_up_int)
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::release_k_up_int() {
  auto temp = unsafe_arena_release_k_up_int();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::unsafe_arena_release_k_up_int() {
  // @@protoc_insertion_point(field_release:fs.pdcp_rrc.as_security_config_t.k_up_int)
  
  ::fs::pdcp_rrc::as_key_t* temp = k_up_int_;
  k_up_int_ = nullptr;
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::_internal_mutable_k_up_int() {
  
  if (k_up_int_ == nullptr) {
    auto* p = CreateMaybeMessage<::fs::pdcp_rrc::as_key_t>(GetArena());
    k_up_int_ = p;
  }
  return k_up_int_;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::mutable_k_up_int() {
  // @@protoc_insertion_point(field_mutable:fs.pdcp_rrc.as_security_config_t.k_up_int)
  return _internal_mutable_k_up_int();
}
inline void as_security_config_t::set_allocated_k_up_int(::fs::pdcp_rrc::as_key_t* k_up_int) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete k_up_int_;
  }
  if (k_up_int) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(k_up_int);
    if (message_arena != submessage_arena) {
      k_up_int = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, k_up_int, submessage_arena);
    }
    
  } else {
    
  }
  k_up_int_ = k_up_int;
  // @@protoc_insertion_point(field_set_allocated:fs.pdcp_rrc.as_security_config_t.k_up_int)
}

// .fs.pdcp_rrc.as_key_t k_up_enc = 4;
inline bool as_security_config_t::_internal_has_k_up_enc() const {
  return this != internal_default_instance() && k_up_enc_ != nullptr;
}
inline bool as_security_config_t::has_k_up_enc() const {
  return _internal_has_k_up_enc();
}
inline void as_security_config_t::clear_k_up_enc() {
  if (GetArena() == nullptr && k_up_enc_ != nullptr) {
    delete k_up_enc_;
  }
  k_up_enc_ = nullptr;
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::_internal_k_up_enc() const {
  const ::fs::pdcp_rrc::as_key_t* p = k_up_enc_;
  return p != nullptr ? *p : *reinterpret_cast<const ::fs::pdcp_rrc::as_key_t*>(
      &::fs::pdcp_rrc::_as_key_t_default_instance_);
}
inline const ::fs::pdcp_rrc::as_key_t& as_security_config_t::k_up_enc() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.k_up_enc)
  return _internal_k_up_enc();
}
inline void as_security_config_t::unsafe_arena_set_allocated_k_up_enc(
    ::fs::pdcp_rrc::as_key_t* k_up_enc) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(k_up_enc_);
  }
  k_up_enc_ = k_up_enc;
  if (k_up_enc) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:fs.pdcp_rrc.as_security_config_t.k_up_enc)
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::release_k_up_enc() {
  auto temp = unsafe_arena_release_k_up_enc();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::unsafe_arena_release_k_up_enc() {
  // @@protoc_insertion_point(field_release:fs.pdcp_rrc.as_security_config_t.k_up_enc)
  
  ::fs::pdcp_rrc::as_key_t* temp = k_up_enc_;
  k_up_enc_ = nullptr;
  return temp;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::_internal_mutable_k_up_enc() {
  
  if (k_up_enc_ == nullptr) {
    auto* p = CreateMaybeMessage<::fs::pdcp_rrc::as_key_t>(GetArena());
    k_up_enc_ = p;
  }
  return k_up_enc_;
}
inline ::fs::pdcp_rrc::as_key_t* as_security_config_t::mutable_k_up_enc() {
  // @@protoc_insertion_point(field_mutable:fs.pdcp_rrc.as_security_config_t.k_up_enc)
  return _internal_mutable_k_up_enc();
}
inline void as_security_config_t::set_allocated_k_up_enc(::fs::pdcp_rrc::as_key_t* k_up_enc) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete k_up_enc_;
  }
  if (k_up_enc) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(k_up_enc);
    if (message_arena != submessage_arena) {
      k_up_enc = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, k_up_enc, submessage_arena);
    }
    
  } else {
    
  }
  k_up_enc_ = k_up_enc;
  // @@protoc_insertion_point(field_set_allocated:fs.pdcp_rrc.as_security_config_t.k_up_enc)
}

// .fs.pdcp_rrc.INTEGRITY_ALGORITHM_ID_ENUM integ_algo = 5;
inline void as_security_config_t::clear_integ_algo() {
  integ_algo_ = 0;
}
inline ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM as_security_config_t::_internal_integ_algo() const {
  return static_cast< ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM >(integ_algo_);
}
inline ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM as_security_config_t::integ_algo() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.integ_algo)
  return _internal_integ_algo();
}
inline void as_security_config_t::_internal_set_integ_algo(::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM value) {
  
  integ_algo_ = value;
}
inline void as_security_config_t::set_integ_algo(::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM value) {
  _internal_set_integ_algo(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.as_security_config_t.integ_algo)
}

// .fs.pdcp_rrc.CIPHERING_ALGORITHM_ID_ENUM cipher_algo = 6;
inline void as_security_config_t::clear_cipher_algo() {
  cipher_algo_ = 0;
}
inline ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM as_security_config_t::_internal_cipher_algo() const {
  return static_cast< ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM >(cipher_algo_);
}
inline ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM as_security_config_t::cipher_algo() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.as_security_config_t.cipher_algo)
  return _internal_cipher_algo();
}
inline void as_security_config_t::_internal_set_cipher_algo(::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM value) {
  
  cipher_algo_ = value;
}
inline void as_security_config_t::set_cipher_algo(::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM value) {
  _internal_set_cipher_algo(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.as_security_config_t.cipher_algo)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace pdcp_rrc
}  // namespace fs

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM>() {
  return ::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM_descriptor();
}
template <> struct is_proto_enum< ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM>() {
  return ::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_as_5fsecurity_5fconfig_5ft_2eproto