// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/parser/cxfa_datadata.h"

#include "core/fxcrt/fx_extension.h"
#include "xfa/fxfa/parser/cxfa_measurement.h"
#include "xfa/fxfa/parser/cxfa_node.h"

// Static.
FX_ARGB CXFA_DataData::ToColor(const WideStringView& wsValue) {
  uint8_t r = 0, g = 0, b = 0;
  if (wsValue.GetLength() == 0)
    return 0xff000000;

  int cc = 0;
  const wchar_t* str = wsValue.unterminated_c_str();
  int len = wsValue.GetLength();
  while (FXSYS_iswspace(str[cc]) && cc < len)
    cc++;

  if (cc >= len)
    return 0xff000000;

  while (cc < len) {
    if (str[cc] == ',' || !FXSYS_isDecimalDigit(str[cc]))
      break;

    r = r * 10 + str[cc] - '0';
    cc++;
  }
  if (cc < len && str[cc] == ',') {
    cc++;
    while (FXSYS_iswspace(str[cc]) && cc < len)
      cc++;

    while (cc < len) {
      if (str[cc] == ',' || !FXSYS_isDecimalDigit(str[cc]))
        break;

      g = g * 10 + str[cc] - '0';
      cc++;
    }
    if (cc < len && str[cc] == ',') {
      cc++;
      while (FXSYS_iswspace(str[cc]) && cc < len)
        cc++;

      while (cc < len) {
        if (str[cc] == ',' || !FXSYS_isDecimalDigit(str[cc]))
          break;

        b = b * 10 + str[cc] - '0';
        cc++;
      }
    }
  }
  return (0xff << 24) | (r << 16) | (g << 8) | b;
}

XFA_Element CXFA_DataData::GetElementType() const {
  return m_pNode ? m_pNode->GetElementType() : XFA_Element::Unknown;
}

bool CXFA_DataData::TryMeasure(XFA_Attribute eAttr,
                               float& fValue,
                               bool bUseDefault) const {
  pdfium::Optional<CXFA_Measurement> measure =
      m_pNode->JSNode()->TryMeasure(eAttr, bUseDefault);
  if (!measure)
    return false;

  fValue = measure->ToUnit(XFA_Unit::Pt);
  return true;
}

bool CXFA_DataData::SetMeasure(XFA_Attribute eAttr, float fValue) {
  CXFA_Measurement ms(fValue, XFA_Unit::Pt);
  return m_pNode->JSNode()->SetMeasure(eAttr, ms, false);
}
