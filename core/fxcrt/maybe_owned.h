// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CORE_FXCRT_MAYBE_OWNED_H_
#define CORE_FXCRT_MAYBE_OWNED_H_

#include <algorithm>
#include <memory>
#include <utility>

#include "core/fxcrt/fx_memory.h"
#include "core/fxcrt/fx_system.h"

namespace fxcrt {

// A template that can hold either owned or unowned references, and cleans up
// appropriately.  Possibly the most pernicious anti-pattern imaginable, but
// it crops up throughout the codebase due to a desire to avoid copying-in
// objects or data.
template <typename T, typename D = std::default_delete<T>>
class MaybeOwned {
 public:
  MaybeOwned() : m_pObj(nullptr) {}
  explicit MaybeOwned(T* ptr) : m_pObj(ptr) {}
  explicit MaybeOwned(std::unique_ptr<T, D> ptr)
      : m_pOwnedObj(std::move(ptr)), m_pObj(m_pOwnedObj.get()) {}

  MaybeOwned(const MaybeOwned& that) = delete;
  MaybeOwned(MaybeOwned&& that) noexcept
      : m_pOwnedObj(that.m_pOwnedObj.release()), m_pObj(that.m_pObj) {
    that.m_pObj = nullptr;
  }

  void Reset(std::unique_ptr<T, D> ptr) {
    m_pOwnedObj = std::move(ptr);
    m_pObj = m_pOwnedObj.get();
  }
  void Reset(T* ptr = nullptr) {
    m_pOwnedObj.reset();
    m_pObj = ptr;
  }

  bool IsOwned() const { return !!m_pOwnedObj; }
  T* Get() const { return m_pObj; }
  std::unique_ptr<T, D> Release() {
    ASSERT(IsOwned());
    return std::move(m_pOwnedObj);
  }

  MaybeOwned& operator=(const MaybeOwned& that) = delete;
  MaybeOwned& operator=(MaybeOwned&& that) {
    m_pOwnedObj = std::move(that.m_pOwnedObj);
    m_pObj = that.m_pObj;
    that.m_pObj = nullptr;
    return *this;
  }
  MaybeOwned& operator=(T* ptr) {
    Reset(ptr);
    return *this;
  }
  MaybeOwned& operator=(std::unique_ptr<T, D> ptr) {
    Reset(std::move(ptr));
    return *this;
  }

  bool operator==(const MaybeOwned& that) const { return Get() == that.Get(); }
  bool operator==(const std::unique_ptr<T, D>& ptr) const {
    return Get() == ptr.get();
  }
  bool operator==(T* ptr) const { return Get() == ptr; }

  bool operator!=(const MaybeOwned& that) const { return !(*this == that); }
  bool operator!=(const std::unique_ptr<T, D> ptr) const {
    return !(*this == ptr);
  }
  bool operator!=(T* ptr) const { return !(*this == ptr); }

  explicit operator bool() const { return !!m_pObj; }
  T& operator*() const { return *m_pObj; }
  T* operator->() const { return m_pObj; }

 private:
  std::unique_ptr<T, D> m_pOwnedObj;
  T* m_pObj;
};

}  // namespace fxcrt

using fxcrt::MaybeOwned;

#endif  // CORE_FXCRT_MAYBE_OWNED_H_
