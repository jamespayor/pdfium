// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/cxfa_ffrectangle.h"

CXFA_FFRectangle::CXFA_FFRectangle(CXFA_WidgetAcc* pDataAcc)
    : CXFA_FFDraw(pDataAcc) {}

CXFA_FFRectangle::~CXFA_FFRectangle() {}

void CXFA_FFRectangle::RenderWidget(CXFA_Graphics* pGS,
                                    const CFX_Matrix& matrix,
                                    uint32_t dwStatus) {
  if (!IsMatchVisibleStatus(dwStatus))
    return;

  CXFA_ValueData valueData = m_pDataAcc->GetFormValueData();
  if (!valueData)
    return;

  CFX_RectF rect = GetRectWithoutRotate();
  if (CXFA_MarginData marginData = m_pDataAcc->GetMarginData())
    XFA_RectWidthoutMargin(rect, marginData);

  CFX_Matrix mtRotate = GetRotateMatrix();
  mtRotate.Concat(matrix);

  DrawBorder(pGS, valueData.GetRectangleData(), rect, mtRotate);
}
