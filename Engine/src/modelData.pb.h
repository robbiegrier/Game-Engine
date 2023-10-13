// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modelData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_modelData_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_modelData_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4024000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4024003 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_modelData_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_modelData_2eproto {
  static const ::uint32_t offsets[];
};
class modelData_proto;
struct modelData_protoDefaultTypeInternal;
extern modelData_protoDefaultTypeInternal _modelData_proto_default_instance_;
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google


// ===================================================================


// -------------------------------------------------------------------

class modelData_proto final :
    public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:modelData_proto) */ {
 public:
  inline modelData_proto() : modelData_proto(nullptr) {}
  ~modelData_proto() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR modelData_proto(::google::protobuf::internal::ConstantInitialized);

  modelData_proto(const modelData_proto& from);
  modelData_proto(modelData_proto&& from) noexcept
    : modelData_proto() {
    *this = ::std::move(from);
  }

  inline modelData_proto& operator=(const modelData_proto& from) {
    CopyFrom(from);
    return *this;
  }
  inline modelData_proto& operator=(modelData_proto&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields<std::string>(::google::protobuf::internal::GetEmptyString);
  }
  inline std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<std::string>();
  }

  static const modelData_proto& default_instance() {
    return *internal_default_instance();
  }
  static inline const modelData_proto* internal_default_instance() {
    return reinterpret_cast<const modelData_proto*>(
               &_modelData_proto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(modelData_proto& a, modelData_proto& b) {
    a.Swap(&b);
  }
  inline void Swap(modelData_proto* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(modelData_proto* other) {
    if (other == this) return;
    ABSL_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  modelData_proto* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<modelData_proto>(arena);
  }
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)  final;
  void CopyFrom(const modelData_proto& from);
  void MergeFrom(const modelData_proto& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  ::size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target, ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(modelData_proto* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "modelData_proto";
  }
  protected:
  explicit modelData_proto(::google::protobuf::Arena* arena);
  public:

  std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kVboVertFieldNumber = 1,
    kVboNormFieldNumber = 2,
    kVboUvFieldNumber = 3,
    kVboTrilistFieldNumber = 4,
    kTextColorFieldNumber = 5,
    kTextNormalFieldNumber = 6,
    kTextRoughFieldNumber = 7,
  };
  // optional int32 vbo_vert = 1;
  bool has_vbo_vert() const;
  void clear_vbo_vert() ;
  ::int32_t vbo_vert() const;
  void set_vbo_vert(::int32_t value);

  private:
  ::int32_t _internal_vbo_vert() const;
  void _internal_set_vbo_vert(::int32_t value);

  public:
  // optional int32 vbo_norm = 2;
  bool has_vbo_norm() const;
  void clear_vbo_norm() ;
  ::int32_t vbo_norm() const;
  void set_vbo_norm(::int32_t value);

  private:
  ::int32_t _internal_vbo_norm() const;
  void _internal_set_vbo_norm(::int32_t value);

  public:
  // optional int32 vbo_uv = 3;
  bool has_vbo_uv() const;
  void clear_vbo_uv() ;
  ::int32_t vbo_uv() const;
  void set_vbo_uv(::int32_t value);

  private:
  ::int32_t _internal_vbo_uv() const;
  void _internal_set_vbo_uv(::int32_t value);

  public:
  // optional int32 vbo_trilist = 4;
  bool has_vbo_trilist() const;
  void clear_vbo_trilist() ;
  ::int32_t vbo_trilist() const;
  void set_vbo_trilist(::int32_t value);

  private:
  ::int32_t _internal_vbo_trilist() const;
  void _internal_set_vbo_trilist(::int32_t value);

  public:
  // optional int32 text_color = 5;
  bool has_text_color() const;
  void clear_text_color() ;
  ::int32_t text_color() const;
  void set_text_color(::int32_t value);

  private:
  ::int32_t _internal_text_color() const;
  void _internal_set_text_color(::int32_t value);

  public:
  // optional int32 text_normal = 6;
  bool has_text_normal() const;
  void clear_text_normal() ;
  ::int32_t text_normal() const;
  void set_text_normal(::int32_t value);

  private:
  ::int32_t _internal_text_normal() const;
  void _internal_set_text_normal(::int32_t value);

  public:
  // optional int32 text_rough = 7;
  bool has_text_rough() const;
  void clear_text_rough() ;
  ::int32_t text_rough() const;
  void set_text_rough(::int32_t value);

  private:
  ::int32_t _internal_text_rough() const;
  void _internal_set_text_rough(::int32_t value);

  public:
  // @@protoc_insertion_point(class_scope:modelData_proto)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<3, 7, 0, 0, 2> _table_;
  template <typename T> friend class ::google::protobuf::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::google::protobuf::internal::HasBits<1> _has_bits_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    ::int32_t vbo_vert_;
    ::int32_t vbo_norm_;
    ::int32_t vbo_uv_;
    ::int32_t vbo_trilist_;
    ::int32_t text_color_;
    ::int32_t text_normal_;
    ::int32_t text_rough_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_modelData_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// modelData_proto

// optional int32 vbo_vert = 1;
inline bool modelData_proto::has_vbo_vert() const {
  bool value = (_impl_._has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline void modelData_proto::clear_vbo_vert() {
  _impl_.vbo_vert_ = 0;
  _impl_._has_bits_[0] &= ~0x00000001u;
}
inline ::int32_t modelData_proto::vbo_vert() const {
  // @@protoc_insertion_point(field_get:modelData_proto.vbo_vert)
  return _internal_vbo_vert();
}
inline void modelData_proto::set_vbo_vert(::int32_t value) {
  _internal_set_vbo_vert(value);
  // @@protoc_insertion_point(field_set:modelData_proto.vbo_vert)
}
inline ::int32_t modelData_proto::_internal_vbo_vert() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.vbo_vert_;
}
inline void modelData_proto::_internal_set_vbo_vert(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000001u;
  _impl_.vbo_vert_ = value;
}

// optional int32 vbo_norm = 2;
inline bool modelData_proto::has_vbo_norm() const {
  bool value = (_impl_._has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline void modelData_proto::clear_vbo_norm() {
  _impl_.vbo_norm_ = 0;
  _impl_._has_bits_[0] &= ~0x00000002u;
}
inline ::int32_t modelData_proto::vbo_norm() const {
  // @@protoc_insertion_point(field_get:modelData_proto.vbo_norm)
  return _internal_vbo_norm();
}
inline void modelData_proto::set_vbo_norm(::int32_t value) {
  _internal_set_vbo_norm(value);
  // @@protoc_insertion_point(field_set:modelData_proto.vbo_norm)
}
inline ::int32_t modelData_proto::_internal_vbo_norm() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.vbo_norm_;
}
inline void modelData_proto::_internal_set_vbo_norm(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000002u;
  _impl_.vbo_norm_ = value;
}

// optional int32 vbo_uv = 3;
inline bool modelData_proto::has_vbo_uv() const {
  bool value = (_impl_._has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline void modelData_proto::clear_vbo_uv() {
  _impl_.vbo_uv_ = 0;
  _impl_._has_bits_[0] &= ~0x00000004u;
}
inline ::int32_t modelData_proto::vbo_uv() const {
  // @@protoc_insertion_point(field_get:modelData_proto.vbo_uv)
  return _internal_vbo_uv();
}
inline void modelData_proto::set_vbo_uv(::int32_t value) {
  _internal_set_vbo_uv(value);
  // @@protoc_insertion_point(field_set:modelData_proto.vbo_uv)
}
inline ::int32_t modelData_proto::_internal_vbo_uv() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.vbo_uv_;
}
inline void modelData_proto::_internal_set_vbo_uv(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000004u;
  _impl_.vbo_uv_ = value;
}

// optional int32 vbo_trilist = 4;
inline bool modelData_proto::has_vbo_trilist() const {
  bool value = (_impl_._has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline void modelData_proto::clear_vbo_trilist() {
  _impl_.vbo_trilist_ = 0;
  _impl_._has_bits_[0] &= ~0x00000008u;
}
inline ::int32_t modelData_proto::vbo_trilist() const {
  // @@protoc_insertion_point(field_get:modelData_proto.vbo_trilist)
  return _internal_vbo_trilist();
}
inline void modelData_proto::set_vbo_trilist(::int32_t value) {
  _internal_set_vbo_trilist(value);
  // @@protoc_insertion_point(field_set:modelData_proto.vbo_trilist)
}
inline ::int32_t modelData_proto::_internal_vbo_trilist() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.vbo_trilist_;
}
inline void modelData_proto::_internal_set_vbo_trilist(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000008u;
  _impl_.vbo_trilist_ = value;
}

// optional int32 text_color = 5;
inline bool modelData_proto::has_text_color() const {
  bool value = (_impl_._has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline void modelData_proto::clear_text_color() {
  _impl_.text_color_ = 0;
  _impl_._has_bits_[0] &= ~0x00000010u;
}
inline ::int32_t modelData_proto::text_color() const {
  // @@protoc_insertion_point(field_get:modelData_proto.text_color)
  return _internal_text_color();
}
inline void modelData_proto::set_text_color(::int32_t value) {
  _internal_set_text_color(value);
  // @@protoc_insertion_point(field_set:modelData_proto.text_color)
}
inline ::int32_t modelData_proto::_internal_text_color() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.text_color_;
}
inline void modelData_proto::_internal_set_text_color(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000010u;
  _impl_.text_color_ = value;
}

// optional int32 text_normal = 6;
inline bool modelData_proto::has_text_normal() const {
  bool value = (_impl_._has_bits_[0] & 0x00000020u) != 0;
  return value;
}
inline void modelData_proto::clear_text_normal() {
  _impl_.text_normal_ = 0;
  _impl_._has_bits_[0] &= ~0x00000020u;
}
inline ::int32_t modelData_proto::text_normal() const {
  // @@protoc_insertion_point(field_get:modelData_proto.text_normal)
  return _internal_text_normal();
}
inline void modelData_proto::set_text_normal(::int32_t value) {
  _internal_set_text_normal(value);
  // @@protoc_insertion_point(field_set:modelData_proto.text_normal)
}
inline ::int32_t modelData_proto::_internal_text_normal() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.text_normal_;
}
inline void modelData_proto::_internal_set_text_normal(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000020u;
  _impl_.text_normal_ = value;
}

// optional int32 text_rough = 7;
inline bool modelData_proto::has_text_rough() const {
  bool value = (_impl_._has_bits_[0] & 0x00000040u) != 0;
  return value;
}
inline void modelData_proto::clear_text_rough() {
  _impl_.text_rough_ = 0;
  _impl_._has_bits_[0] &= ~0x00000040u;
}
inline ::int32_t modelData_proto::text_rough() const {
  // @@protoc_insertion_point(field_get:modelData_proto.text_rough)
  return _internal_text_rough();
}
inline void modelData_proto::set_text_rough(::int32_t value) {
  _internal_set_text_rough(value);
  // @@protoc_insertion_point(field_set:modelData_proto.text_rough)
}
inline ::int32_t modelData_proto::_internal_text_rough() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.text_rough_;
}
inline void modelData_proto::_internal_set_text_rough(::int32_t value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_._has_bits_[0] |= 0x00000040u;
  _impl_.text_rough_ = value;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_modelData_2eproto_2epb_2eh
