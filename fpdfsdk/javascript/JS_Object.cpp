// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "fpdfsdk/javascript/JS_Object.h"

#include "fpdfsdk/include/fsdk_mgr.h"
#include "fpdfsdk/javascript/JS_Define.h"
#include "fpdfsdk/javascript/cjs_context.h"

CJS_EmbedObj::CJS_EmbedObj(CJS_Object* pJSObject) : m_pJSObject(pJSObject) {}

CJS_EmbedObj::~CJS_EmbedObj() {
  m_pJSObject = nullptr;
}

int CJS_EmbedObj::MsgBox(CPDFDoc_Environment* pApp,
                         const FX_WCHAR* swMsg,
                         const FX_WCHAR* swTitle,
                         FX_UINT nType,
                         FX_UINT nIcon) {
  if (!pApp)
    return 0;

  if (CPDFSDK_Document* pDoc = pApp->GetSDKDocument())
    pDoc->KillFocusAnnot();

  return pApp->JS_appAlert(swMsg, swTitle, nType, nIcon);
}

void CJS_EmbedObj::Alert(CJS_Context* pContext, const FX_WCHAR* swMsg) {
  CJS_Object::Alert(pContext, swMsg);
}

void FreeObject(const v8::WeakCallbackInfo<CJS_Object>& data) {
  CJS_Object* pJSObj = data.GetParameter();
  pJSObj->ExitInstance();
  delete pJSObj;
  FXJS_FreePrivate(data.GetInternalField(0));
}

void DisposeObject(const v8::WeakCallbackInfo<CJS_Object>& data) {
  CJS_Object* pJSObj = data.GetParameter();
  pJSObj->Dispose();
  data.SetSecondPassCallback(FreeObject);
}

CJS_Object::CJS_Object(v8::Local<v8::Object> pObject) {
  m_pIsolate = pObject->GetIsolate();
  m_pV8Object.Reset(m_pIsolate, pObject);
}

CJS_Object::~CJS_Object() {}

void CJS_Object::MakeWeak() {
  m_pV8Object.SetWeak(this, DisposeObject,
                      v8::WeakCallbackType::kInternalFields);
}

void CJS_Object::Dispose() {
  m_pV8Object.Reset();
}

void CJS_Object::InitInstance(IJS_Runtime* pIRuntime) {}

void CJS_Object::ExitInstance() {}

void CJS_Object::Alert(CJS_Context* pContext, const FX_WCHAR* swMsg) {
  CPDFDoc_Environment* pApp = pContext->GetReaderApp();
  if (pApp)
    pApp->JS_appAlert(swMsg, nullptr, 0, 3);
}

CJS_Timer::CJS_Timer(CJS_EmbedObj* pObj,
                     CPDFDoc_Environment* pApp,
                     CJS_Runtime* pRuntime,
                     int nType,
                     const CFX_WideString& script,
                     uint32_t dwElapse,
                     uint32_t dwTimeOut)
    : m_nTimerID(0),
      m_pEmbedObj(pObj),
      m_bProcessing(false),
      m_bValid(true),
      m_nType(nType),
      m_dwTimeOut(dwTimeOut),
      m_swJScript(script),
      m_pRuntime(pRuntime),
      m_pApp(pApp) {
  CFX_SystemHandler* pHandler = m_pApp->GetSysHandler();
  m_nTimerID = pHandler->SetTimer(dwElapse, Trigger);
  (*GetGlobalTimerMap())[m_nTimerID] = this;
  m_pRuntime->AddObserver(this);
}

CJS_Timer::~CJS_Timer() {
  CJS_Runtime* pRuntime = GetRuntime();
  if (pRuntime)
    pRuntime->RemoveObserver(this);

  if (!m_nTimerID)
    return;

  if (m_bValid)
    m_pApp->GetSysHandler()->KillTimer(m_nTimerID);

  GetGlobalTimerMap()->erase(m_nTimerID);
}

// static
void CJS_Timer::Trigger(int nTimerID) {
  auto it = GetGlobalTimerMap()->find(nTimerID);
  if (it == GetGlobalTimerMap()->end())
    return;

  CJS_Timer* pTimer = it->second;
  if (pTimer->m_bProcessing)
    return;

  pTimer->m_bProcessing = true;
  if (pTimer->m_pEmbedObj)
    pTimer->m_pEmbedObj->TimerProc(pTimer);

  // Timer proc may have destroyed timer, find it again.
  it = GetGlobalTimerMap()->find(nTimerID);
  if (it == GetGlobalTimerMap()->end())
    return;

  pTimer = it->second;
  pTimer->m_bProcessing = false;
  if (pTimer->IsOneShot())
    pTimer->m_pEmbedObj->CancelProc(pTimer);
}

// static
void CJS_Timer::Cancel(int nTimerID) {
  auto it = GetGlobalTimerMap()->find(nTimerID);
  if (it == GetGlobalTimerMap()->end())
    return;

  CJS_Timer* pTimer = it->second;
  pTimer->m_pEmbedObj->CancelProc(pTimer);
}

// static
CJS_Timer::TimerMap* CJS_Timer::GetGlobalTimerMap() {
  // Leak the timer array at shutdown.
  static auto* s_TimerMap = new TimerMap;
  return s_TimerMap;
}

void CJS_Timer::OnDestroyed() {
  m_bValid = false;
}
