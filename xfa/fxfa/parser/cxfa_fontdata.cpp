// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/parser/cxfa_fontdata.h"

#include "core/fxge/fx_dib.h"
#include "xfa/fxfa/parser/cxfa_filldata.h"
#include "xfa/fxfa/parser/cxfa_measurement.h"
#include "xfa/fxfa/parser/cxfa_node.h"

CXFA_FontData::CXFA_FontData(CXFA_Node* pNode) : CXFA_DataData(pNode) {}

float CXFA_FontData::GetBaselineShift() {
  return m_pNode->JSNode()
      ->GetMeasure(XFA_Attribute::BaselineShift)
      .ToUnit(XFA_Unit::Pt);
}

float CXFA_FontData::GetHorizontalScale() {
  WideString wsValue =
      m_pNode->JSNode()->GetCData(XFA_Attribute::FontHorizontalScale);
  int32_t iScale = FXSYS_wtoi(wsValue.c_str());
  return iScale > 0 ? (float)iScale : 100.0f;
}

float CXFA_FontData::GetVerticalScale() {
  WideString wsValue =
      m_pNode->JSNode()->GetCData(XFA_Attribute::FontVerticalScale);
  int32_t iScale = FXSYS_wtoi(wsValue.c_str());
  return iScale > 0 ? (float)iScale : 100.0f;
}

float CXFA_FontData::GetLetterSpacing() {
  WideString wsValue =
      m_pNode->JSNode()->GetCData(XFA_Attribute::LetterSpacing);
  CXFA_Measurement ms(wsValue.AsStringView());
  if (ms.GetUnit() == XFA_Unit::Em)
    return ms.GetValue() * GetFontSize();
  return ms.ToUnit(XFA_Unit::Pt);
}

int32_t CXFA_FontData::GetLineThrough() {
  return m_pNode->JSNode()->GetInteger(XFA_Attribute::LineThrough);
}

int32_t CXFA_FontData::GetUnderline() {
  return m_pNode->JSNode()->GetInteger(XFA_Attribute::Underline);
}

int32_t CXFA_FontData::GetUnderlinePeriod() {
  return m_pNode->JSNode()
      ->TryEnum(XFA_Attribute::UnderlinePeriod, true)
      .value_or(XFA_ATTRIBUTEENUM_All);
}

float CXFA_FontData::GetFontSize() {
  return m_pNode->JSNode()
      ->GetMeasure(XFA_Attribute::Size)
      .ToUnit(XFA_Unit::Pt);
}

void CXFA_FontData::GetTypeface(WideString& wsTypeFace) {
  wsTypeFace = m_pNode->JSNode()->GetCData(XFA_Attribute::Typeface);
}

bool CXFA_FontData::IsBold() {
  return m_pNode->JSNode()->GetEnum(XFA_Attribute::Weight) ==
         XFA_ATTRIBUTEENUM_Bold;
}

bool CXFA_FontData::IsItalic() {
  return m_pNode->JSNode()->GetEnum(XFA_Attribute::Posture) ==
         XFA_ATTRIBUTEENUM_Italic;
}

void CXFA_FontData::SetColor(FX_ARGB color) {
  CXFA_FillData(m_pNode->JSNode()->GetProperty(0, XFA_Element::Fill, true))
      .SetColor(color);
}

FX_ARGB CXFA_FontData::GetColor() {
  CXFA_FillData fillData(m_pNode->GetChild(0, XFA_Element::Fill, false));
  return fillData ? fillData.GetColor(true) : 0xFF000000;
}
