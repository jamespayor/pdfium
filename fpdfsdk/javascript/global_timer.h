// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_JAVASCRIPT_GLOBAL_TIMER_H_
#define FPDFSDK_JAVASCRIPT_GLOBAL_TIMER_H_

#include <map>

#include "fpdfsdk/javascript/cjs_app.h"

class GlobalTimer {
 public:
  GlobalTimer(app* pObj,
              CPDFSDK_FormFillEnvironment* pFormFillEnv,
              CJS_Runtime* pRuntime,
              int nType,
              const WideString& script,
              uint32_t dwElapse,
              uint32_t dwTimeOut);
  ~GlobalTimer();

  static void Trigger(int nTimerID);
  static void Cancel(int nTimerID);

  bool IsOneShot() const { return m_nType == 1; }
  uint32_t GetTimeOut() const { return m_dwTimeOut; }
  int GetTimerID() const { return m_nTimerID; }
  CJS_Runtime* GetRuntime() const { return m_pRuntime.Get(); }
  WideString GetJScript() const { return m_swJScript; }

 private:
  using TimerMap = std::map<uint32_t, GlobalTimer*>;
  static TimerMap* GetGlobalTimerMap();

  uint32_t m_nTimerID;
  app* const m_pEmbedObj;
  bool m_bProcessing;

  // data
  const int m_nType;  // 0:Interval; 1:TimeOut
  const uint32_t m_dwTimeOut;
  const WideString m_swJScript;
  CJS_Runtime::ObservedPtr m_pRuntime;
  CPDFSDK_FormFillEnvironment::ObservedPtr m_pFormFillEnv;
};

#endif  // FPDFSDK_JAVASCRIPT_GLOBAL_TIMER_H_