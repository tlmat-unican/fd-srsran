// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pdcp_config_t.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_pdcp_5fconfig_5ft_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_pdcp_5fconfig_5ft_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_pdcp_5fconfig_5ft_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_pdcp_5fconfig_5ft_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_pdcp_5fconfig_5ft_2eproto;
namespace fs {
namespace pdcp_rrc {
class pdcp_config_t;
class pdcp_config_tDefaultTypeInternal;
extern pdcp_config_tDefaultTypeInternal _pdcp_config_t_default_instance_;
}  // namespace pdcp_rrc
}  // namespace fs
PROTOBUF_NAMESPACE_OPEN
template<> ::fs::pdcp_rrc::pdcp_config_t* Arena::CreateMaybeMessage<::fs::pdcp_rrc::pdcp_config_t>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace fs {
namespace pdcp_rrc {

enum pdcp_rb_type_t : int {
  PDCP_RB_IS_SRB = 0,
  PDCP_RB_IS_DRB = 1,
  pdcp_rb_type_t_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  pdcp_rb_type_t_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool pdcp_rb_type_t_IsValid(int value);
constexpr pdcp_rb_type_t pdcp_rb_type_t_MIN = PDCP_RB_IS_SRB;
constexpr pdcp_rb_type_t pdcp_rb_type_t_MAX = PDCP_RB_IS_DRB;
constexpr int pdcp_rb_type_t_ARRAYSIZE = pdcp_rb_type_t_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* pdcp_rb_type_t_descriptor();
template<typename T>
inline const std::string& pdcp_rb_type_t_Name(T enum_t_value) {
  static_assert(::std::is_same<T, pdcp_rb_type_t>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function pdcp_rb_type_t_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    pdcp_rb_type_t_descriptor(), enum_t_value);
}
inline bool pdcp_rb_type_t_Parse(
    const std::string& name, pdcp_rb_type_t* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<pdcp_rb_type_t>(
    pdcp_rb_type_t_descriptor(), name, value);
}
enum security_direction_t : int {
  SECURITY_DIRECTION_UPLINK = 0,
  SECURITY_DIRECTION_DOWNLINK = 1,
  SECURITY_DIRECTION_N_ITEMS = 2,
  security_direction_t_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  security_direction_t_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool security_direction_t_IsValid(int value);
constexpr security_direction_t security_direction_t_MIN = SECURITY_DIRECTION_UPLINK;
constexpr security_direction_t security_direction_t_MAX = SECURITY_DIRECTION_N_ITEMS;
constexpr int security_direction_t_ARRAYSIZE = security_direction_t_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* security_direction_t_descriptor();
template<typename T>
inline const std::string& security_direction_t_Name(T enum_t_value) {
  static_assert(::std::is_same<T, security_direction_t>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function security_direction_t_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    security_direction_t_descriptor(), enum_t_value);
}
inline bool security_direction_t_Parse(
    const std::string& name, security_direction_t* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<security_direction_t>(
    security_direction_t_descriptor(), name, value);
}
enum pdcp_t_reordering_t : int {
  ms_0 = 0,
  ms_1 = 1,
  ms_2 = 2,
  ms_4 = 4,
  ms_5 = 5,
  ms_8 = 6,
  ms_10 = 7,
  ms_15 = 8,
  ms_20 = 9,
  ms_30 = 10,
  ms_40 = 11,
  ms_50 = 12,
  ms_60 = 13,
  ms_80 = 14,
  ms_100 = 15,
  ms_120 = 16,
  ms_140 = 17,
  ms_160 = 18,
  ms_180 = 19,
  ms_200 = 20,
  ms_220 = 21,
  ms_240 = 22,
  ms_260 = 23,
  ms_280 = 24,
  ms_300 = 25,
  ms_500 = 26,
  ms_750 = 27,
  ms_1000 = 28,
  ms_1250 = 29,
  ms_1500 = 30,
  ms_1750 = 31,
  ms_2000 = 32,
  ms_2250 = 33,
  ms_2500 = 34,
  ms_2750 = 35,
  ms_3000 = 36,
  pdcp_t_reordering_t_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  pdcp_t_reordering_t_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool pdcp_t_reordering_t_IsValid(int value);
constexpr pdcp_t_reordering_t pdcp_t_reordering_t_MIN = ms_0;
constexpr pdcp_t_reordering_t pdcp_t_reordering_t_MAX = ms_3000;
constexpr int pdcp_t_reordering_t_ARRAYSIZE = pdcp_t_reordering_t_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* pdcp_t_reordering_t_descriptor();
template<typename T>
inline const std::string& pdcp_t_reordering_t_Name(T enum_t_value) {
  static_assert(::std::is_same<T, pdcp_t_reordering_t>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function pdcp_t_reordering_t_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    pdcp_t_reordering_t_descriptor(), enum_t_value);
}
inline bool pdcp_t_reordering_t_Parse(
    const std::string& name, pdcp_t_reordering_t* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<pdcp_t_reordering_t>(
    pdcp_t_reordering_t_descriptor(), name, value);
}
enum pdcp_discard_timer_t : int {
  ms10 = 0,
  ms20 = 1,
  ms30 = 2,
  ms40 = 3,
  ms50 = 4,
  ms60 = 5,
  ms75 = 6,
  ms100 = 7,
  ms150 = 8,
  ms200 = 9,
  ms250 = 10,
  ms300 = 11,
  ms500 = 12,
  ms750 = 13,
  ms1500 = 14,
  infinity = 15,
  pdcp_discard_timer_t_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  pdcp_discard_timer_t_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool pdcp_discard_timer_t_IsValid(int value);
constexpr pdcp_discard_timer_t pdcp_discard_timer_t_MIN = ms10;
constexpr pdcp_discard_timer_t pdcp_discard_timer_t_MAX = infinity;
constexpr int pdcp_discard_timer_t_ARRAYSIZE = pdcp_discard_timer_t_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* pdcp_discard_timer_t_descriptor();
template<typename T>
inline const std::string& pdcp_discard_timer_t_Name(T enum_t_value) {
  static_assert(::std::is_same<T, pdcp_discard_timer_t>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function pdcp_discard_timer_t_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    pdcp_discard_timer_t_descriptor(), enum_t_value);
}
inline bool pdcp_discard_timer_t_Parse(
    const std::string& name, pdcp_discard_timer_t* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<pdcp_discard_timer_t>(
    pdcp_discard_timer_t_descriptor(), name, value);
}
enum srsran_rat_t : int {
  lte = 0,
  nr = 1,
  nulltype = 2,
  srsran_rat_t_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  srsran_rat_t_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool srsran_rat_t_IsValid(int value);
constexpr srsran_rat_t srsran_rat_t_MIN = lte;
constexpr srsran_rat_t srsran_rat_t_MAX = nulltype;
constexpr int srsran_rat_t_ARRAYSIZE = srsran_rat_t_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* srsran_rat_t_descriptor();
template<typename T>
inline const std::string& srsran_rat_t_Name(T enum_t_value) {
  static_assert(::std::is_same<T, srsran_rat_t>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function srsran_rat_t_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    srsran_rat_t_descriptor(), enum_t_value);
}
inline bool srsran_rat_t_Parse(
    const std::string& name, srsran_rat_t* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<srsran_rat_t>(
    srsran_rat_t_descriptor(), name, value);
}
// ===================================================================

class pdcp_config_t PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fs.pdcp_rrc.pdcp_config_t) */ {
 public:
  inline pdcp_config_t() : pdcp_config_t(nullptr) {};
  virtual ~pdcp_config_t();

  pdcp_config_t(const pdcp_config_t& from);
  pdcp_config_t(pdcp_config_t&& from) noexcept
    : pdcp_config_t() {
    *this = ::std::move(from);
  }

  inline pdcp_config_t& operator=(const pdcp_config_t& from) {
    CopyFrom(from);
    return *this;
  }
  inline pdcp_config_t& operator=(pdcp_config_t&& from) noexcept {
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
  static const pdcp_config_t& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const pdcp_config_t* internal_default_instance() {
    return reinterpret_cast<const pdcp_config_t*>(
               &_pdcp_config_t_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(pdcp_config_t& a, pdcp_config_t& b) {
    a.Swap(&b);
  }
  inline void Swap(pdcp_config_t* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(pdcp_config_t* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline pdcp_config_t* New() const final {
    return CreateMaybeMessage<pdcp_config_t>(nullptr);
  }

  pdcp_config_t* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<pdcp_config_t>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const pdcp_config_t& from);
  void MergeFrom(const pdcp_config_t& from);
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
  void InternalSwap(pdcp_config_t* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fs.pdcp_rrc.pdcp_config_t";
  }
  protected:
  explicit pdcp_config_t(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_pdcp_5fconfig_5ft_2eproto);
    return ::descriptor_table_pdcp_5fconfig_5ft_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBearerIdFieldNumber = 1,
    kRbTypeFieldNumber = 2,
    kTxDirectionFieldNumber = 3,
    kRxDirectionFieldNumber = 4,
    kSnLenFieldNumber = 5,
    kTReorderingFieldNumber = 6,
    kDiscardTimerFieldNumber = 7,
    kStatusReportRequiredFieldNumber = 8,
    kRatFieldNumber = 9,
  };
  // uint32 bearer_id_ = 1;
  void clear_bearer_id_();
  ::PROTOBUF_NAMESPACE_ID::uint32 bearer_id_() const;
  void set_bearer_id_(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_bearer_id_() const;
  void _internal_set_bearer_id_(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // .fs.pdcp_rrc.pdcp_rb_type_t rb_type_ = 2;
  void clear_rb_type_();
  ::fs::pdcp_rrc::pdcp_rb_type_t rb_type_() const;
  void set_rb_type_(::fs::pdcp_rrc::pdcp_rb_type_t value);
  private:
  ::fs::pdcp_rrc::pdcp_rb_type_t _internal_rb_type_() const;
  void _internal_set_rb_type_(::fs::pdcp_rrc::pdcp_rb_type_t value);
  public:

  // .fs.pdcp_rrc.security_direction_t tx_direction_ = 3;
  void clear_tx_direction_();
  ::fs::pdcp_rrc::security_direction_t tx_direction_() const;
  void set_tx_direction_(::fs::pdcp_rrc::security_direction_t value);
  private:
  ::fs::pdcp_rrc::security_direction_t _internal_tx_direction_() const;
  void _internal_set_tx_direction_(::fs::pdcp_rrc::security_direction_t value);
  public:

  // .fs.pdcp_rrc.security_direction_t rx_direction_ = 4;
  void clear_rx_direction_();
  ::fs::pdcp_rrc::security_direction_t rx_direction_() const;
  void set_rx_direction_(::fs::pdcp_rrc::security_direction_t value);
  private:
  ::fs::pdcp_rrc::security_direction_t _internal_rx_direction_() const;
  void _internal_set_rx_direction_(::fs::pdcp_rrc::security_direction_t value);
  public:

  // uint32 sn_len_ = 5;
  void clear_sn_len_();
  ::PROTOBUF_NAMESPACE_ID::uint32 sn_len_() const;
  void set_sn_len_(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_sn_len_() const;
  void _internal_set_sn_len_(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // .fs.pdcp_rrc.pdcp_t_reordering_t t_reordering_ = 6;
  void clear_t_reordering_();
  ::fs::pdcp_rrc::pdcp_t_reordering_t t_reordering_() const;
  void set_t_reordering_(::fs::pdcp_rrc::pdcp_t_reordering_t value);
  private:
  ::fs::pdcp_rrc::pdcp_t_reordering_t _internal_t_reordering_() const;
  void _internal_set_t_reordering_(::fs::pdcp_rrc::pdcp_t_reordering_t value);
  public:

  // .fs.pdcp_rrc.pdcp_discard_timer_t discard_timer_ = 7;
  void clear_discard_timer_();
  ::fs::pdcp_rrc::pdcp_discard_timer_t discard_timer_() const;
  void set_discard_timer_(::fs::pdcp_rrc::pdcp_discard_timer_t value);
  private:
  ::fs::pdcp_rrc::pdcp_discard_timer_t _internal_discard_timer_() const;
  void _internal_set_discard_timer_(::fs::pdcp_rrc::pdcp_discard_timer_t value);
  public:

  // bool status_report_required_ = 8;
  void clear_status_report_required_();
  bool status_report_required_() const;
  void set_status_report_required_(bool value);
  private:
  bool _internal_status_report_required_() const;
  void _internal_set_status_report_required_(bool value);
  public:

  // .fs.pdcp_rrc.srsran_rat_t rat_ = 9;
  void clear_rat_();
  ::fs::pdcp_rrc::srsran_rat_t rat_() const;
  void set_rat_(::fs::pdcp_rrc::srsran_rat_t value);
  private:
  ::fs::pdcp_rrc::srsran_rat_t _internal_rat_() const;
  void _internal_set_rat_(::fs::pdcp_rrc::srsran_rat_t value);
  public:

  // @@protoc_insertion_point(class_scope:fs.pdcp_rrc.pdcp_config_t)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint32 bearer_id__;
  int rb_type__;
  int tx_direction__;
  int rx_direction__;
  ::PROTOBUF_NAMESPACE_ID::uint32 sn_len__;
  int t_reordering__;
  int discard_timer__;
  bool status_report_required__;
  int rat__;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_pdcp_5fconfig_5ft_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// pdcp_config_t

// uint32 bearer_id_ = 1;
inline void pdcp_config_t::clear_bearer_id_() {
  bearer_id__ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 pdcp_config_t::_internal_bearer_id_() const {
  return bearer_id__;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 pdcp_config_t::bearer_id_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.bearer_id_)
  return _internal_bearer_id_();
}
inline void pdcp_config_t::_internal_set_bearer_id_(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  bearer_id__ = value;
}
inline void pdcp_config_t::set_bearer_id_(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_bearer_id_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.bearer_id_)
}

// .fs.pdcp_rrc.pdcp_rb_type_t rb_type_ = 2;
inline void pdcp_config_t::clear_rb_type_() {
  rb_type__ = 0;
}
inline ::fs::pdcp_rrc::pdcp_rb_type_t pdcp_config_t::_internal_rb_type_() const {
  return static_cast< ::fs::pdcp_rrc::pdcp_rb_type_t >(rb_type__);
}
inline ::fs::pdcp_rrc::pdcp_rb_type_t pdcp_config_t::rb_type_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.rb_type_)
  return _internal_rb_type_();
}
inline void pdcp_config_t::_internal_set_rb_type_(::fs::pdcp_rrc::pdcp_rb_type_t value) {
  
  rb_type__ = value;
}
inline void pdcp_config_t::set_rb_type_(::fs::pdcp_rrc::pdcp_rb_type_t value) {
  _internal_set_rb_type_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.rb_type_)
}

// .fs.pdcp_rrc.security_direction_t tx_direction_ = 3;
inline void pdcp_config_t::clear_tx_direction_() {
  tx_direction__ = 0;
}
inline ::fs::pdcp_rrc::security_direction_t pdcp_config_t::_internal_tx_direction_() const {
  return static_cast< ::fs::pdcp_rrc::security_direction_t >(tx_direction__);
}
inline ::fs::pdcp_rrc::security_direction_t pdcp_config_t::tx_direction_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.tx_direction_)
  return _internal_tx_direction_();
}
inline void pdcp_config_t::_internal_set_tx_direction_(::fs::pdcp_rrc::security_direction_t value) {
  
  tx_direction__ = value;
}
inline void pdcp_config_t::set_tx_direction_(::fs::pdcp_rrc::security_direction_t value) {
  _internal_set_tx_direction_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.tx_direction_)
}

// .fs.pdcp_rrc.security_direction_t rx_direction_ = 4;
inline void pdcp_config_t::clear_rx_direction_() {
  rx_direction__ = 0;
}
inline ::fs::pdcp_rrc::security_direction_t pdcp_config_t::_internal_rx_direction_() const {
  return static_cast< ::fs::pdcp_rrc::security_direction_t >(rx_direction__);
}
inline ::fs::pdcp_rrc::security_direction_t pdcp_config_t::rx_direction_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.rx_direction_)
  return _internal_rx_direction_();
}
inline void pdcp_config_t::_internal_set_rx_direction_(::fs::pdcp_rrc::security_direction_t value) {
  
  rx_direction__ = value;
}
inline void pdcp_config_t::set_rx_direction_(::fs::pdcp_rrc::security_direction_t value) {
  _internal_set_rx_direction_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.rx_direction_)
}

// uint32 sn_len_ = 5;
inline void pdcp_config_t::clear_sn_len_() {
  sn_len__ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 pdcp_config_t::_internal_sn_len_() const {
  return sn_len__;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 pdcp_config_t::sn_len_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.sn_len_)
  return _internal_sn_len_();
}
inline void pdcp_config_t::_internal_set_sn_len_(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  sn_len__ = value;
}
inline void pdcp_config_t::set_sn_len_(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_sn_len_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.sn_len_)
}

// .fs.pdcp_rrc.pdcp_t_reordering_t t_reordering_ = 6;
inline void pdcp_config_t::clear_t_reordering_() {
  t_reordering__ = 0;
}
inline ::fs::pdcp_rrc::pdcp_t_reordering_t pdcp_config_t::_internal_t_reordering_() const {
  return static_cast< ::fs::pdcp_rrc::pdcp_t_reordering_t >(t_reordering__);
}
inline ::fs::pdcp_rrc::pdcp_t_reordering_t pdcp_config_t::t_reordering_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.t_reordering_)
  return _internal_t_reordering_();
}
inline void pdcp_config_t::_internal_set_t_reordering_(::fs::pdcp_rrc::pdcp_t_reordering_t value) {
  
  t_reordering__ = value;
}
inline void pdcp_config_t::set_t_reordering_(::fs::pdcp_rrc::pdcp_t_reordering_t value) {
  _internal_set_t_reordering_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.t_reordering_)
}

// .fs.pdcp_rrc.pdcp_discard_timer_t discard_timer_ = 7;
inline void pdcp_config_t::clear_discard_timer_() {
  discard_timer__ = 0;
}
inline ::fs::pdcp_rrc::pdcp_discard_timer_t pdcp_config_t::_internal_discard_timer_() const {
  return static_cast< ::fs::pdcp_rrc::pdcp_discard_timer_t >(discard_timer__);
}
inline ::fs::pdcp_rrc::pdcp_discard_timer_t pdcp_config_t::discard_timer_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.discard_timer_)
  return _internal_discard_timer_();
}
inline void pdcp_config_t::_internal_set_discard_timer_(::fs::pdcp_rrc::pdcp_discard_timer_t value) {
  
  discard_timer__ = value;
}
inline void pdcp_config_t::set_discard_timer_(::fs::pdcp_rrc::pdcp_discard_timer_t value) {
  _internal_set_discard_timer_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.discard_timer_)
}

// bool status_report_required_ = 8;
inline void pdcp_config_t::clear_status_report_required_() {
  status_report_required__ = false;
}
inline bool pdcp_config_t::_internal_status_report_required_() const {
  return status_report_required__;
}
inline bool pdcp_config_t::status_report_required_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.status_report_required_)
  return _internal_status_report_required_();
}
inline void pdcp_config_t::_internal_set_status_report_required_(bool value) {
  
  status_report_required__ = value;
}
inline void pdcp_config_t::set_status_report_required_(bool value) {
  _internal_set_status_report_required_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.status_report_required_)
}

// .fs.pdcp_rrc.srsran_rat_t rat_ = 9;
inline void pdcp_config_t::clear_rat_() {
  rat__ = 0;
}
inline ::fs::pdcp_rrc::srsran_rat_t pdcp_config_t::_internal_rat_() const {
  return static_cast< ::fs::pdcp_rrc::srsran_rat_t >(rat__);
}
inline ::fs::pdcp_rrc::srsran_rat_t pdcp_config_t::rat_() const {
  // @@protoc_insertion_point(field_get:fs.pdcp_rrc.pdcp_config_t.rat_)
  return _internal_rat_();
}
inline void pdcp_config_t::_internal_set_rat_(::fs::pdcp_rrc::srsran_rat_t value) {
  
  rat__ = value;
}
inline void pdcp_config_t::set_rat_(::fs::pdcp_rrc::srsran_rat_t value) {
  _internal_set_rat_(value);
  // @@protoc_insertion_point(field_set:fs.pdcp_rrc.pdcp_config_t.rat_)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace pdcp_rrc
}  // namespace fs

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::fs::pdcp_rrc::pdcp_rb_type_t> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::pdcp_rb_type_t>() {
  return ::fs::pdcp_rrc::pdcp_rb_type_t_descriptor();
}
template <> struct is_proto_enum< ::fs::pdcp_rrc::security_direction_t> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::security_direction_t>() {
  return ::fs::pdcp_rrc::security_direction_t_descriptor();
}
template <> struct is_proto_enum< ::fs::pdcp_rrc::pdcp_t_reordering_t> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::pdcp_t_reordering_t>() {
  return ::fs::pdcp_rrc::pdcp_t_reordering_t_descriptor();
}
template <> struct is_proto_enum< ::fs::pdcp_rrc::pdcp_discard_timer_t> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::pdcp_discard_timer_t>() {
  return ::fs::pdcp_rrc::pdcp_discard_timer_t_descriptor();
}
template <> struct is_proto_enum< ::fs::pdcp_rrc::srsran_rat_t> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::fs::pdcp_rrc::srsran_rat_t>() {
  return ::fs::pdcp_rrc::srsran_rat_t_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_pdcp_5fconfig_5ft_2eproto
