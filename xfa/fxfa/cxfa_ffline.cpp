// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/cxfa_ffline.h"

#include "xfa/fxgraphics/cxfa_gecolor.h"
#include "xfa/fxgraphics/cxfa_gepath.h"
#include "xfa/fxgraphics/cxfa_graphics.h"

CXFA_FFLine::CXFA_FFLine(CXFA_WidgetAcc* pDataAcc) : CXFA_FFDraw(pDataAcc) {}

CXFA_FFLine::~CXFA_FFLine() {}

void CXFA_FFLine::GetRectFromHand(CFX_RectF& rect,
                                  int32_t iHand,
                                  float fLineWidth) {
  float fHalfWidth = fLineWidth / 2.0f;
  if (rect.height < 1.0f) {
    switch (iHand) {
      case XFA_ATTRIBUTEENUM_Left:
        rect.top -= fHalfWidth;
        break;
      case XFA_ATTRIBUTEENUM_Right:
        rect.top += fHalfWidth;
    }
  } else if (rect.width < 1.0f) {
    switch (iHand) {
      case XFA_ATTRIBUTEENUM_Left:
        rect.left += fHalfWidth;
        break;
      case XFA_ATTRIBUTEENUM_Right:
        rect.left += fHalfWidth;
        break;
    }
  } else {
    switch (iHand) {
      case XFA_ATTRIBUTEENUM_Left:
        rect.Inflate(fHalfWidth, fHalfWidth);
        break;
      case XFA_ATTRIBUTEENUM_Right:
        rect.Deflate(fHalfWidth, fHalfWidth);
        break;
    }
  }
}

void CXFA_FFLine::RenderWidget(CXFA_Graphics* pGS,
                               const CFX_Matrix& matrix,
                               uint32_t dwStatus) {
  if (!IsMatchVisibleStatus(dwStatus))
    return;

  CXFA_ValueData valueData = m_pDataAcc->GetFormValueData();
  if (!valueData)
    return;

  CXFA_LineData lineData = valueData.GetLineData();
  FX_ARGB lineColor = 0xFF000000;
  int32_t iStrokeType = 0;
  float fLineWidth = 1.0f;
  int32_t iCap = 0;
  CXFA_EdgeData edgeData = lineData.GetEdgeData();
  if (edgeData) {
    if (edgeData.GetPresence() != XFA_ATTRIBUTEENUM_Visible)
      return;

    lineColor = edgeData.GetColor();
    iStrokeType = edgeData.GetStrokeType();
    fLineWidth = edgeData.GetThickness();
    iCap = edgeData.GetCapType();
  }

  CFX_Matrix mtRotate = GetRotateMatrix();
  mtRotate.Concat(matrix);

  CFX_RectF rtLine = GetRectWithoutRotate();
  if (CXFA_MarginData marginData = m_pDataAcc->GetMarginData())
    XFA_RectWidthoutMargin(rtLine, marginData);

  GetRectFromHand(rtLine, lineData.GetHand(), fLineWidth);
  CXFA_GEPath linePath;
  if (lineData.GetSlope() && rtLine.right() > 0.0f && rtLine.bottom() > 0.0f)
    linePath.AddLine(rtLine.TopRight(), rtLine.BottomLeft());
  else
    linePath.AddLine(rtLine.TopLeft(), rtLine.BottomRight());

  pGS->SaveGraphState();
  pGS->SetLineWidth(fLineWidth);
  pGS->EnableActOnDash();
  XFA_StrokeTypeSetLineDash(pGS, iStrokeType, iCap);
  pGS->SetStrokeColor(CXFA_GEColor(lineColor));
  pGS->SetLineCap(XFA_LineCapToFXGE(iCap));
  pGS->StrokePath(&linePath, &mtRotate);
  pGS->RestoreGraphState();
}
