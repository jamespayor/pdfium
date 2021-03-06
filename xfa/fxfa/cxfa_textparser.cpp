// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/cxfa_textparser.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "core/fxcrt/css/cfx_css.h"
#include "core/fxcrt/css/cfx_csscomputedstyle.h"
#include "core/fxcrt/css/cfx_cssstyleselector.h"
#include "core/fxcrt/css/cfx_cssstylesheet.h"
#include "core/fxcrt/fx_codepage.h"
#include "core/fxcrt/xml/cfx_xmlelement.h"
#include "core/fxcrt/xml/cfx_xmlnode.h"
#include "third_party/base/ptr_util.h"
#include "xfa/fgas/font/cfgas_fontmgr.h"
#include "xfa/fxfa/cxfa_ffapp.h"
#include "xfa/fxfa/cxfa_ffdoc.h"
#include "xfa/fxfa/cxfa_fontmgr.h"
#include "xfa/fxfa/cxfa_textparsecontext.h"
#include "xfa/fxfa/cxfa_textprovider.h"
#include "xfa/fxfa/cxfa_texttabstopscontext.h"
#include "xfa/fxfa/parser/cxfa_measurement.h"

namespace {

enum class TabStopStatus {
  Error,
  EOS,
  None,
  Alignment,
  StartLeader,
  Leader,
  Location,
};

}  // namespace

CXFA_TextParser::CXFA_TextParser()
    : m_bParsed(false), m_cssInitialized(false) {}

CXFA_TextParser::~CXFA_TextParser() {}

void CXFA_TextParser::Reset() {
  m_mapXMLNodeToParseContext.clear();
  m_bParsed = false;
}

void CXFA_TextParser::InitCSSData(CXFA_TextProvider* pTextProvider) {
  if (!pTextProvider)
    return;

  if (!m_pSelector) {
    m_pSelector = pdfium::MakeUnique<CFX_CSSStyleSelector>();

    CXFA_FontData fontData = pTextProvider->GetFontData();
    m_pSelector->SetDefFontSize(fontData ? fontData.GetFontSize() : 10.0f);
  }

  if (m_cssInitialized)
    return;

  m_cssInitialized = true;
  auto uaSheet = LoadDefaultSheetStyle();
  m_pSelector->SetUAStyleSheet(std::move(uaSheet));
  m_pSelector->UpdateStyleIndex();
}

std::unique_ptr<CFX_CSSStyleSheet> CXFA_TextParser::LoadDefaultSheetStyle() {
  static const wchar_t s_pStyle[] =
      L"html,body,ol,p,ul{display:block}"
      L"li{display:list-item}"
      L"ol,ul{padding-left:33px;margin:1.12em 0}"
      L"ol{list-style-type:decimal}"
      L"a{color:#0000ff;text-decoration:underline}"
      L"b{font-weight:bolder}"
      L"i{font-style:italic}"
      L"sup{vertical-align:+15em;font-size:.66em}"
      L"sub{vertical-align:-15em;font-size:.66em}";

  auto sheet = pdfium::MakeUnique<CFX_CSSStyleSheet>();
  return sheet->LoadBuffer(s_pStyle, wcslen(s_pStyle)) ? std::move(sheet)
                                                       : nullptr;
}

RetainPtr<CFX_CSSComputedStyle> CXFA_TextParser::CreateRootStyle(
    CXFA_TextProvider* pTextProvider) {
  CXFA_ParaData paraData = pTextProvider->GetParaData();
  auto pStyle = m_pSelector->CreateComputedStyle(nullptr);
  float fLineHeight = 0;
  float fFontSize = 10;

  if (paraData) {
    fLineHeight = paraData.GetLineHeight();
    CFX_CSSLength indent;
    indent.Set(CFX_CSSLengthUnit::Point, paraData.GetTextIndent());
    pStyle->SetTextIndent(indent);
    CFX_CSSTextAlign hAlign = CFX_CSSTextAlign::Left;
    switch (paraData.GetHorizontalAlign()) {
      case XFA_ATTRIBUTEENUM_Center:
        hAlign = CFX_CSSTextAlign::Center;
        break;
      case XFA_ATTRIBUTEENUM_Right:
        hAlign = CFX_CSSTextAlign::Right;
        break;
      case XFA_ATTRIBUTEENUM_Justify:
        hAlign = CFX_CSSTextAlign::Justify;
        break;
      case XFA_ATTRIBUTEENUM_JustifyAll:
        hAlign = CFX_CSSTextAlign::JustifyAll;
        break;
    }
    pStyle->SetTextAlign(hAlign);
    CFX_CSSRect rtMarginWidth;
    rtMarginWidth.left.Set(CFX_CSSLengthUnit::Point, paraData.GetMarginLeft());
    rtMarginWidth.top.Set(CFX_CSSLengthUnit::Point, paraData.GetSpaceAbove());
    rtMarginWidth.right.Set(CFX_CSSLengthUnit::Point,
                            paraData.GetMarginRight());
    rtMarginWidth.bottom.Set(CFX_CSSLengthUnit::Point,
                             paraData.GetSpaceBelow());
    pStyle->SetMarginWidth(rtMarginWidth);
  }

  CXFA_FontData fontData = pTextProvider->GetFontData();
  if (fontData) {
    pStyle->SetColor(fontData.GetColor());
    pStyle->SetFontStyle(fontData.IsItalic() ? CFX_CSSFontStyle::Italic
                                             : CFX_CSSFontStyle::Normal);
    pStyle->SetFontWeight(fontData.IsBold() ? FXFONT_FW_BOLD
                                            : FXFONT_FW_NORMAL);
    pStyle->SetNumberVerticalAlign(-fontData.GetBaselineShift());
    fFontSize = fontData.GetFontSize();
    CFX_CSSLength letterSpacing;
    letterSpacing.Set(CFX_CSSLengthUnit::Point, fontData.GetLetterSpacing());
    pStyle->SetLetterSpacing(letterSpacing);
    uint32_t dwDecoration = 0;
    if (fontData.GetLineThrough() > 0)
      dwDecoration |= CFX_CSSTEXTDECORATION_LineThrough;
    if (fontData.GetUnderline() > 1)
      dwDecoration |= CFX_CSSTEXTDECORATION_Double;
    else if (fontData.GetUnderline() > 0)
      dwDecoration |= CFX_CSSTEXTDECORATION_Underline;

    pStyle->SetTextDecoration(dwDecoration);
  }
  pStyle->SetLineHeight(fLineHeight);
  pStyle->SetFontSize(fFontSize);
  return pStyle;
}

RetainPtr<CFX_CSSComputedStyle> CXFA_TextParser::CreateStyle(
    CFX_CSSComputedStyle* pParentStyle) {
  auto pNewStyle = m_pSelector->CreateComputedStyle(pParentStyle);
  ASSERT(pNewStyle);
  if (!pParentStyle)
    return pNewStyle;

  uint32_t dwDecoration = pParentStyle->GetTextDecoration();
  float fBaseLine = 0;
  if (pParentStyle->GetVerticalAlign() == CFX_CSSVerticalAlign::Number)
    fBaseLine = pParentStyle->GetNumberVerticalAlign();

  pNewStyle->SetTextDecoration(dwDecoration);
  pNewStyle->SetNumberVerticalAlign(fBaseLine);

  const CFX_CSSRect* pRect = pParentStyle->GetMarginWidth();
  if (pRect)
    pNewStyle->SetMarginWidth(*pRect);
  return pNewStyle;
}

RetainPtr<CFX_CSSComputedStyle> CXFA_TextParser::ComputeStyle(
    CFX_XMLNode* pXMLNode,
    CFX_CSSComputedStyle* pParentStyle) {
  auto it = m_mapXMLNodeToParseContext.find(pXMLNode);
  if (it == m_mapXMLNodeToParseContext.end())
    return nullptr;

  CXFA_TextParseContext* pContext = it->second.get();
  if (!pContext)
    return nullptr;

  pContext->m_pParentStyle.Reset(pParentStyle);

  auto tagProvider = ParseTagInfo(pXMLNode);
  if (tagProvider->m_bContent)
    return nullptr;

  auto pStyle = CreateStyle(pParentStyle);
  m_pSelector->ComputeStyle(pContext->GetDecls(),
                            tagProvider->GetAttribute(L"style"),
                            tagProvider->GetAttribute(L"align"), pStyle.Get());
  return pStyle;
}

void CXFA_TextParser::DoParse(CFX_XMLNode* pXMLContainer,
                              CXFA_TextProvider* pTextProvider) {
  if (!pXMLContainer || !pTextProvider || m_bParsed)
    return;

  m_bParsed = true;
  InitCSSData(pTextProvider);
  auto pRootStyle = CreateRootStyle(pTextProvider);
  ParseRichText(pXMLContainer, pRootStyle.Get());
}

void CXFA_TextParser::ParseRichText(CFX_XMLNode* pXMLNode,
                                    CFX_CSSComputedStyle* pParentStyle) {
  if (!pXMLNode)
    return;

  auto tagProvider = ParseTagInfo(pXMLNode);
  if (!tagProvider->m_bTagAvailable)
    return;

  RetainPtr<CFX_CSSComputedStyle> pNewStyle;
  if ((tagProvider->GetTagName() != L"body") ||
      (tagProvider->GetTagName() != L"html")) {
    auto pTextContext = pdfium::MakeUnique<CXFA_TextParseContext>();
    CFX_CSSDisplay eDisplay = CFX_CSSDisplay::Inline;
    if (!tagProvider->m_bContent) {
      auto declArray =
          m_pSelector->MatchDeclarations(tagProvider->GetTagName());
      pNewStyle = CreateStyle(pParentStyle);
      m_pSelector->ComputeStyle(declArray, tagProvider->GetAttribute(L"style"),
                                tagProvider->GetAttribute(L"align"),
                                pNewStyle.Get());

      if (!declArray.empty())
        pTextContext->SetDecls(std::move(declArray));

      eDisplay = pNewStyle->GetDisplay();
    }
    pTextContext->SetDisplay(eDisplay);
    m_mapXMLNodeToParseContext[pXMLNode] = std::move(pTextContext);
  }

  for (CFX_XMLNode* pXMLChild = pXMLNode->GetNodeItem(CFX_XMLNode::FirstChild);
       pXMLChild;
       pXMLChild = pXMLChild->GetNodeItem(CFX_XMLNode::NextSibling)) {
    ParseRichText(pXMLChild, pNewStyle.Get());
  }
}

bool CXFA_TextParser::TagValidate(const WideString& wsName) const {
  static const uint32_t s_XFATagName[] = {
      0x61,        // a
      0x62,        // b
      0x69,        // i
      0x70,        // p
      0x0001f714,  // br
      0x00022a55,  // li
      0x000239bb,  // ol
      0x00025881,  // ul
      0x0bd37faa,  // sub
      0x0bd37fb8,  // sup
      0xa73e3af2,  // span
      0xb182eaae,  // body
      0xdb8ac455,  // html
  };
  static const int32_t s_iCount = FX_ArraySize(s_XFATagName);

  return std::binary_search(s_XFATagName, s_XFATagName + s_iCount,
                            FX_HashCode_GetW(wsName.AsStringView(), true));
}

std::unique_ptr<CXFA_TextParser::TagProvider> CXFA_TextParser::ParseTagInfo(
    CFX_XMLNode* pXMLNode) {
  auto tagProvider = pdfium::MakeUnique<TagProvider>();

  WideString wsName;
  if (pXMLNode->GetType() == FX_XMLNODE_Element) {
    CFX_XMLElement* pXMLElement = static_cast<CFX_XMLElement*>(pXMLNode);
    wsName = pXMLElement->GetLocalTagName();
    tagProvider->SetTagName(wsName);
    tagProvider->m_bTagAvailable = TagValidate(wsName);

    WideString wsValue = pXMLElement->GetString(L"style");
    if (!wsValue.IsEmpty())
      tagProvider->SetAttribute(L"style", wsValue);
  } else if (pXMLNode->GetType() == FX_XMLNODE_Text) {
    tagProvider->m_bTagAvailable = true;
    tagProvider->m_bContent = true;
  }
  return tagProvider;
}

int32_t CXFA_TextParser::GetVAlign(CXFA_TextProvider* pTextProvider) const {
  CXFA_ParaData paraData = pTextProvider->GetParaData();
  return paraData ? paraData.GetVerticalAlign() : XFA_ATTRIBUTEENUM_Top;
}

float CXFA_TextParser::GetTabInterval(CFX_CSSComputedStyle* pStyle) const {
  WideString wsValue;
  if (pStyle && pStyle->GetCustomStyle(L"tab-interval", wsValue))
    return CXFA_Measurement(wsValue.AsStringView()).ToUnit(XFA_Unit::Pt);
  return 36;
}

int32_t CXFA_TextParser::CountTabs(CFX_CSSComputedStyle* pStyle) const {
  WideString wsValue;
  if (pStyle && pStyle->GetCustomStyle(L"xfa-tab-count", wsValue))
    return wsValue.GetInteger();
  return 0;
}

bool CXFA_TextParser::IsSpaceRun(CFX_CSSComputedStyle* pStyle) const {
  WideString wsValue;
  if (pStyle && pStyle->GetCustomStyle(L"xfa-spacerun", wsValue)) {
    wsValue.MakeLower();
    return wsValue == L"yes";
  }
  return false;
}

RetainPtr<CFGAS_GEFont> CXFA_TextParser::GetFont(
    CXFA_TextProvider* pTextProvider,
    CFX_CSSComputedStyle* pStyle) const {
  WideString wsFamily = L"Courier";
  uint32_t dwStyle = 0;
  CXFA_FontData fontData = pTextProvider->GetFontData();
  if (fontData) {
    fontData.GetTypeface(wsFamily);
    if (fontData.IsBold())
      dwStyle |= FXFONT_BOLD;
    if (fontData.IsItalic())
      dwStyle |= FXFONT_BOLD;
  }

  if (pStyle) {
    int32_t iCount = pStyle->CountFontFamilies();
    if (iCount > 0)
      wsFamily = pStyle->GetFontFamily(iCount - 1).AsStringView();

    dwStyle = 0;
    if (pStyle->GetFontWeight() > FXFONT_FW_NORMAL)
      dwStyle |= FXFONT_BOLD;
    if (pStyle->GetFontStyle() == CFX_CSSFontStyle::Italic)
      dwStyle |= FXFONT_ITALIC;
  }

  CXFA_FFDoc* pDoc = pTextProvider->GetDocNode();
  CXFA_FontMgr* pFontMgr = pDoc->GetApp()->GetXFAFontMgr();
  return pFontMgr->GetFont(pDoc, wsFamily.AsStringView(), dwStyle);
}

float CXFA_TextParser::GetFontSize(CXFA_TextProvider* pTextProvider,
                                   CFX_CSSComputedStyle* pStyle) const {
  if (pStyle)
    return pStyle->GetFontSize();

  CXFA_FontData fontData = pTextProvider->GetFontData();
  return fontData ? fontData.GetFontSize() : 10;
}

int32_t CXFA_TextParser::GetHorScale(CXFA_TextProvider* pTextProvider,
                                     CFX_CSSComputedStyle* pStyle,
                                     CFX_XMLNode* pXMLNode) const {
  if (pStyle) {
    WideString wsValue;
    if (pStyle->GetCustomStyle(L"xfa-font-horizontal-scale", wsValue))
      return wsValue.GetInteger();

    while (pXMLNode) {
      auto it = m_mapXMLNodeToParseContext.find(pXMLNode);
      if (it != m_mapXMLNodeToParseContext.end()) {
        CXFA_TextParseContext* pContext = it->second.get();
        if (pContext && pContext->m_pParentStyle &&
            pContext->m_pParentStyle->GetCustomStyle(
                L"xfa-font-horizontal-scale", wsValue)) {
          return wsValue.GetInteger();
        }
      }
      pXMLNode = pXMLNode->GetNodeItem(CFX_XMLNode::Parent);
    }
  }

  CXFA_FontData fontData = pTextProvider->GetFontData();
  return fontData ? static_cast<int32_t>(fontData.GetHorizontalScale()) : 100;
}

int32_t CXFA_TextParser::GetVerScale(CXFA_TextProvider* pTextProvider,
                                     CFX_CSSComputedStyle* pStyle) const {
  if (pStyle) {
    WideString wsValue;
    if (pStyle->GetCustomStyle(L"xfa-font-vertical-scale", wsValue))
      return wsValue.GetInteger();
  }

  CXFA_FontData fontData = pTextProvider->GetFontData();
  return fontData ? static_cast<int32_t>(fontData.GetVerticalScale()) : 100;
}

void CXFA_TextParser::GetUnderline(CXFA_TextProvider* pTextProvider,
                                   CFX_CSSComputedStyle* pStyle,
                                   int32_t& iUnderline,
                                   int32_t& iPeriod) const {
  iUnderline = 0;
  iPeriod = XFA_ATTRIBUTEENUM_All;
  CXFA_FontData fontData = pTextProvider->GetFontData();
  if (!pStyle) {
    if (fontData) {
      iUnderline = fontData.GetUnderline();
      iPeriod = fontData.GetUnderlinePeriod();
    }
    return;
  }

  uint32_t dwDecoration = pStyle->GetTextDecoration();
  if (dwDecoration & CFX_CSSTEXTDECORATION_Double)
    iUnderline = 2;
  else if (dwDecoration & CFX_CSSTEXTDECORATION_Underline)
    iUnderline = 1;

  WideString wsValue;
  if (pStyle->GetCustomStyle(L"underlinePeriod", wsValue)) {
    if (wsValue == L"word")
      iPeriod = XFA_ATTRIBUTEENUM_Word;
  } else if (fontData) {
    iPeriod = fontData.GetUnderlinePeriod();
  }
}

void CXFA_TextParser::GetLinethrough(CXFA_TextProvider* pTextProvider,
                                     CFX_CSSComputedStyle* pStyle,
                                     int32_t& iLinethrough) const {
  if (pStyle) {
    uint32_t dwDecoration = pStyle->GetTextDecoration();
    iLinethrough = (dwDecoration & CFX_CSSTEXTDECORATION_LineThrough) ? 1 : 0;
    return;
  }

  CXFA_FontData fontData = pTextProvider->GetFontData();
  if (fontData)
    iLinethrough = fontData.GetLineThrough();
}

FX_ARGB CXFA_TextParser::GetColor(CXFA_TextProvider* pTextProvider,
                                  CFX_CSSComputedStyle* pStyle) const {
  if (pStyle)
    return pStyle->GetColor();

  CXFA_FontData fontData = pTextProvider->GetFontData();
  return fontData ? fontData.GetColor() : 0xFF000000;
}

float CXFA_TextParser::GetBaseline(CXFA_TextProvider* pTextProvider,
                                   CFX_CSSComputedStyle* pStyle) const {
  if (pStyle) {
    if (pStyle->GetVerticalAlign() == CFX_CSSVerticalAlign::Number)
      return pStyle->GetNumberVerticalAlign();
  } else if (CXFA_FontData fontData = pTextProvider->GetFontData()) {
    return fontData.GetBaselineShift();
  }
  return 0;
}

float CXFA_TextParser::GetLineHeight(CXFA_TextProvider* pTextProvider,
                                     CFX_CSSComputedStyle* pStyle,
                                     bool bFirst,
                                     float fVerScale) const {
  float fLineHeight = 0;
  if (pStyle)
    fLineHeight = pStyle->GetLineHeight();
  else if (CXFA_ParaData paraData = pTextProvider->GetParaData())
    fLineHeight = paraData.GetLineHeight();

  if (bFirst) {
    float fFontSize = GetFontSize(pTextProvider, pStyle);
    if (fLineHeight < 0.1f)
      fLineHeight = fFontSize;
    else
      fLineHeight = std::min(fLineHeight, fFontSize);
  } else if (fLineHeight < 0.1f) {
    fLineHeight = GetFontSize(pTextProvider, pStyle) * 1.2f;
  }
  fLineHeight *= fVerScale;
  return fLineHeight;
}

bool CXFA_TextParser::GetEmbbedObj(CXFA_TextProvider* pTextProvider,
                                   CFX_XMLNode* pXMLNode,
                                   WideString& wsValue) {
  wsValue.clear();
  if (!pXMLNode)
    return false;

  bool bRet = false;
  if (pXMLNode->GetType() == FX_XMLNODE_Element) {
    CFX_XMLElement* pElement = static_cast<CFX_XMLElement*>(pXMLNode);
    WideString wsAttr = pElement->GetString(L"xfa:embed");
    if (wsAttr.IsEmpty())
      return false;
    if (wsAttr[0] == L'#')
      wsAttr.Delete(0);

    WideString ws = pElement->GetString(L"xfa:embedType");
    if (ws.IsEmpty())
      ws = L"som";
    else
      ws.MakeLower();

    bool bURI = (ws == L"uri");
    if (!bURI && ws != L"som")
      return false;

    ws = pElement->GetString(L"xfa:embedMode");
    if (ws.IsEmpty())
      ws = L"formatted";
    else
      ws.MakeLower();

    bool bRaw = (ws == L"raw");
    if (!bRaw && ws != L"formatted")
      return false;

    bRet = pTextProvider->GetEmbbedObj(bURI, bRaw, wsAttr, wsValue);
  }
  return bRet;
}

CXFA_TextParseContext* CXFA_TextParser::GetParseContextFromMap(
    CFX_XMLNode* pXMLNode) {
  auto it = m_mapXMLNodeToParseContext.find(pXMLNode);
  return it != m_mapXMLNodeToParseContext.end() ? it->second.get() : nullptr;
}

bool CXFA_TextParser::GetTabstops(CFX_CSSComputedStyle* pStyle,
                                  CXFA_TextTabstopsContext* pTabstopContext) {
  if (!pStyle || !pTabstopContext)
    return false;

  WideString wsValue;
  if (!pStyle->GetCustomStyle(L"xfa-tab-stops", wsValue) &&
      !pStyle->GetCustomStyle(L"tab-stops", wsValue)) {
    return false;
  }

  int32_t iLength = wsValue.GetLength();
  const wchar_t* pTabStops = wsValue.c_str();
  int32_t iCur = 0;
  int32_t iLast = 0;
  WideString wsAlign;
  TabStopStatus eStatus = TabStopStatus::None;
  wchar_t ch;
  while (iCur < iLength) {
    ch = pTabStops[iCur];
    switch (eStatus) {
      case TabStopStatus::None:
        if (ch <= ' ') {
          iCur++;
        } else {
          eStatus = TabStopStatus::Alignment;
          iLast = iCur;
        }
        break;
      case TabStopStatus::Alignment:
        if (ch == ' ') {
          wsAlign = WideStringView(pTabStops + iLast, iCur - iLast);
          eStatus = TabStopStatus::StartLeader;
          iCur++;
          while (iCur < iLength && pTabStops[iCur] <= ' ')
            iCur++;
          iLast = iCur;
        } else {
          iCur++;
        }
        break;
      case TabStopStatus::StartLeader:
        if (ch != 'l') {
          eStatus = TabStopStatus::Location;
        } else {
          int32_t iCount = 0;
          while (iCur < iLength) {
            ch = pTabStops[iCur];
            iCur++;
            if (ch == '(') {
              iCount++;
            } else if (ch == ')') {
              iCount--;
              if (iCount == 0)
                break;
            }
          }
          while (iCur < iLength && pTabStops[iCur] <= ' ')
            iCur++;

          iLast = iCur;
          eStatus = TabStopStatus::Location;
        }
        break;
      case TabStopStatus::Location:
        if (ch == ' ') {
          uint32_t dwHashCode = FX_HashCode_GetW(wsAlign.AsStringView(), true);
          CXFA_Measurement ms(WideStringView(pTabStops + iLast, iCur - iLast));
          float fPos = ms.ToUnit(XFA_Unit::Pt);
          pTabstopContext->Append(dwHashCode, fPos);
          wsAlign.clear();
          eStatus = TabStopStatus::None;
        }
        iCur++;
        break;
      default:
        break;
    }
  }

  if (!wsAlign.IsEmpty()) {
    uint32_t dwHashCode = FX_HashCode_GetW(wsAlign.AsStringView(), true);
    CXFA_Measurement ms(WideStringView(pTabStops + iLast, iCur - iLast));
    float fPos = ms.ToUnit(XFA_Unit::Pt);
    pTabstopContext->Append(dwHashCode, fPos);
  }
  return true;
}

CXFA_TextParser::TagProvider::TagProvider()
    : m_bTagAvailable(false), m_bContent(false) {}

CXFA_TextParser::TagProvider::~TagProvider() {}
