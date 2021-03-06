// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FXJS_CFXJSE_RESOLVEPROCESSOR_H_
#define FXJS_CFXJSE_RESOLVEPROCESSOR_H_

#include <memory>
#include <vector>

#include "xfa/fxfa/parser/xfa_resolvenode_rs.h"

class CXFA_NodeHelper;
class CFXJSE_Engine;

class CFXJSE_ResolveNodeData {
 public:
  explicit CFXJSE_ResolveNodeData(CFXJSE_Engine* pSC);
  ~CFXJSE_ResolveNodeData();

  CFXJSE_Engine* m_pSC;
  CXFA_Object* m_CurObject;
  WideString m_wsName;
  XFA_HashCode m_uHashName;
  WideString m_wsCondition;
  int32_t m_nLevel;
  std::vector<CXFA_Object*> m_Objects;  // Not owned.
  uint32_t m_dwStyles;
  const XFA_SCRIPTATTRIBUTEINFO* m_pScriptAttribute;
  XFA_RESOLVENODE_RSTYPE m_dwFlag;
};

class CFXJSE_ResolveProcessor {
 public:
  CFXJSE_ResolveProcessor();
  ~CFXJSE_ResolveProcessor();

  int32_t Resolve(CFXJSE_ResolveNodeData& rnd);
  int32_t GetFilter(const WideStringView& wsExpression,
                    int32_t nStart,
                    CFXJSE_ResolveNodeData& rnd);
  int32_t SetResultCreateNode(XFA_RESOLVENODE_RS& resolveNodeRS,
                              WideString& wsLastCondition);
  void SetIndexDataBind(WideString& wsNextCondition,
                        int32_t& iIndex,
                        int32_t iCount);
  void SetCurStart(int32_t start) { m_iCurStart = start; }

  CXFA_NodeHelper* GetNodeHelper() const { return m_pNodeHelper.get(); }

 private:
  int32_t ResolveForAttributeRs(CXFA_Object* curNode,
                                CFXJSE_ResolveNodeData& rnd,
                                const WideStringView& strAttr);
  int32_t ResolveAnyChild(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolveDollar(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolveExcalmatory(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolveNumberSign(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolveAsterisk(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolveNormal(CFXJSE_ResolveNodeData& rnd);
  int32_t ResolvePopStack(std::vector<int32_t>* stack);
  void SetStylesForChild(uint32_t dwParentStyles, CFXJSE_ResolveNodeData& rnd);

  void ConditionArray(int32_t iCurIndex,
                      WideString wsCondition,
                      int32_t iFoundCount,
                      CFXJSE_ResolveNodeData& rnd);
  void DoPredicateFilter(int32_t iCurIndex,
                         WideString wsCondition,
                         int32_t iFoundCount,
                         CFXJSE_ResolveNodeData& rnd);
  void FilterCondition(CFXJSE_ResolveNodeData& rnd, WideString wsCondition);

  int32_t m_iCurStart;
  std::unique_ptr<CXFA_NodeHelper> m_pNodeHelper;
};

#endif  // FXJS_CFXJSE_RESOLVEPROCESSOR_H_
