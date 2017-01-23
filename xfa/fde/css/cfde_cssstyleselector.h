// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FDE_CSS_CFDE_CSSSTYLESELECTOR_H_
#define XFA_FDE_CSS_CFDE_CSSSTYLESELECTOR_H_

#include <memory>
#include <vector>

#include "core/fxcrt/fx_basic.h"
#include "core/fxcrt/fx_system.h"
#include "xfa/fde/css/cfde_cssrulecollection.h"
#include "xfa/fde/css/fde_css.h"

class CFDE_CSSAccelerator;
class CFDE_CSSComputedStyle;
class CFDE_CSSDeclaration;
class CFDE_CSSSelector;
class CFDE_CSSStyleSheet;
class CFDE_CSSTagCache;
class CFDE_CSSValue;
class CFDE_CSSValueList;
class CFGAS_FontMgr;
class CXFA_CSSTagProvider;

class CFDE_CSSStyleSelector {
 public:
  explicit CFDE_CSSStyleSelector(CFGAS_FontMgr* pFontMgr);
  ~CFDE_CSSStyleSelector();

  void SetDefFontSize(FX_FLOAT fFontSize);

  void SetUAStyleSheet(std::unique_ptr<CFDE_CSSStyleSheet> pSheet);

  void UpdateStyleIndex();
  CFDE_CSSAccelerator* InitAccelerator();
  CFX_RetainPtr<CFDE_CSSComputedStyle> CreateComputedStyle(
      CFDE_CSSComputedStyle* pParentStyle);
  int32_t MatchDeclarations(
      CXFA_CSSTagProvider* pTag,
      CFX_ArrayTemplate<CFDE_CSSDeclaration*>& matchedDecls);
  void ComputeStyle(CXFA_CSSTagProvider* pTag,
                    const CFDE_CSSDeclaration** ppDeclArray,
                    int32_t iDeclCount,
                    CFDE_CSSComputedStyle* pDestStyle);

 private:
  void MatchRules(
      std::vector<CFDE_CSSRuleCollection::Data*>* matchedRules,
      CFDE_CSSTagCache* pCache,
      const std::vector<std::unique_ptr<CFDE_CSSRuleCollection::Data>>* pList);
  bool MatchSelector(CFDE_CSSTagCache* pCache, CFDE_CSSSelector* pSel);
  void AppendInlineStyle(CFDE_CSSDeclaration* pDecl,
                         const FX_WCHAR* psz,
                         int32_t iLen);
  void ApplyDeclarations(bool bPriority,
                         const CFDE_CSSDeclaration** ppDeclArray,
                         int32_t iDeclCount,
                         CFDE_CSSComputedStyle* pDestStyle);
  void ApplyProperty(FDE_CSSProperty eProperty,
                     CFDE_CSSValue* pValue,
                     CFDE_CSSComputedStyle* pComputedStyle);

  bool SetLengthWithPercent(FDE_CSSLength& width,
                            FDE_CSSPrimitiveType eType,
                            CFDE_CSSValue* pValue,
                            FX_FLOAT fFontSize);
  FX_FLOAT ToFontSize(FDE_CSSPropertyValue eValue, FX_FLOAT fCurFontSize);
  FDE_CSSDisplay ToDisplay(FDE_CSSPropertyValue eValue);
  FDE_CSSTextAlign ToTextAlign(FDE_CSSPropertyValue eValue);
  uint16_t ToFontWeight(FDE_CSSPropertyValue eValue);
  FDE_CSSFontStyle ToFontStyle(FDE_CSSPropertyValue eValue);
  FDE_CSSVerticalAlign ToVerticalAlign(FDE_CSSPropertyValue eValue);
  uint32_t ToTextDecoration(CFDE_CSSValueList* pList);
  FDE_CSSFontVariant ToFontVariant(FDE_CSSPropertyValue eValue);

  CFGAS_FontMgr* const m_pFontMgr;
  FX_FLOAT m_fDefFontSize;
  std::unique_ptr<CFDE_CSSStyleSheet> m_UAStyles;
  CFDE_CSSRuleCollection m_UARules;
  std::unique_ptr<CFDE_CSSAccelerator> m_pAccelerator;
};

#endif  // XFA_FDE_CSS_CFDE_CSSSTYLESELECTOR_H_