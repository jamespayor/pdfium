// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_PDFWINDOW_CPWL_SCROLL_BAR_H_
#define FPDFSDK_PDFWINDOW_CPWL_SCROLL_BAR_H_

#include "core/fxcrt/cfx_unowned_ptr.h"
#include "fpdfsdk/pdfwindow/cpwl_wnd.h"

class CPWL_SBButton;
class CPWL_ScrollBar;

struct PWL_SCROLL_INFO {
 public:
  PWL_SCROLL_INFO()
      : fContentMin(0.0f),
        fContentMax(0.0f),
        fPlateWidth(0.0f),
        fBigStep(0.0f),
        fSmallStep(0.0f) {}

  bool operator==(const PWL_SCROLL_INFO& that) const {
    return fContentMin == that.fContentMin && fContentMax == that.fContentMax &&
           fPlateWidth == that.fPlateWidth && fBigStep == that.fBigStep &&
           fSmallStep == that.fSmallStep;
  }
  bool operator!=(const PWL_SCROLL_INFO& that) const {
    return !(*this == that);
  }

  float fContentMin;
  float fContentMax;
  float fPlateWidth;
  float fBigStep;
  float fSmallStep;
};

enum PWL_SCROLLBAR_TYPE { SBT_HSCROLL, SBT_VSCROLL };

enum PWL_SBBUTTON_TYPE { PSBT_MIN, PSBT_MAX, PSBT_POS };

class CPWL_SBButton : public CPWL_Wnd {
 public:
  CPWL_SBButton(PWL_SCROLLBAR_TYPE eScrollBarType,
                PWL_SBBUTTON_TYPE eButtonType);
  ~CPWL_SBButton() override;

  // CPWL_Wnd
  CFX_ByteString GetClassName() const override;
  void OnCreate(PWL_CREATEPARAM& cp) override;
  void GetThisAppearanceStream(CFX_ByteTextBuf& sAppStream) override;
  void DrawThisAppearance(CFX_RenderDevice* pDevice,
                          CFX_Matrix* pUser2Device) override;
  bool OnLButtonDown(const CFX_PointF& point, uint32_t nFlag) override;
  bool OnLButtonUp(const CFX_PointF& point, uint32_t nFlag) override;
  bool OnMouseMove(const CFX_PointF& point, uint32_t nFlag) override;

 protected:
  PWL_SCROLLBAR_TYPE m_eScrollBarType;
  PWL_SBBUTTON_TYPE m_eSBButtonType;

  bool m_bMouseDown;
};

struct PWL_FLOATRANGE {
 public:
  PWL_FLOATRANGE();
  PWL_FLOATRANGE(float min, float max);

  bool operator==(const PWL_FLOATRANGE& that) const {
    return fMin == that.fMin && fMax == that.fMax;
  }
  bool operator!=(const PWL_FLOATRANGE& that) const { return !(*this == that); }

  void Default();
  void Set(float min, float max);
  bool In(float x) const;
  float GetWidth() const;

  float fMin;
  float fMax;
};

struct PWL_SCROLL_PRIVATEDATA {
 public:
  PWL_SCROLL_PRIVATEDATA();

  bool operator==(const PWL_SCROLL_PRIVATEDATA& that) const {
    return ScrollRange == that.ScrollRange &&
           fClientWidth == that.fClientWidth && fScrollPos == that.fScrollPos &&
           fBigStep == that.fBigStep && fSmallStep == that.fSmallStep;
  }
  bool operator!=(const PWL_SCROLL_PRIVATEDATA& that) const {
    return !(*this == that);
  }

  void Default();
  void SetScrollRange(float min, float max);
  void SetClientWidth(float width);
  void SetSmallStep(float step);
  void SetBigStep(float step);
  bool SetPos(float pos);

  void AddSmall();
  void SubSmall();
  void AddBig();
  void SubBig();

  PWL_FLOATRANGE ScrollRange;
  float fClientWidth;
  float fScrollPos;
  float fBigStep;
  float fSmallStep;
};

class CPWL_ScrollBar : public CPWL_Wnd {
 public:
  explicit CPWL_ScrollBar(PWL_SCROLLBAR_TYPE sbType = SBT_HSCROLL);
  ~CPWL_ScrollBar() override;

  // CPWL_Wnd
  CFX_ByteString GetClassName() const override;
  void OnCreate(PWL_CREATEPARAM& cp) override;
  void OnDestroy() override;
  void RePosChildWnd() override;
  void GetThisAppearanceStream(CFX_ByteTextBuf& sAppStream) override;
  void DrawThisAppearance(CFX_RenderDevice* pDevice,
                          CFX_Matrix* pUser2Device) override;
  bool OnLButtonDown(const CFX_PointF& point, uint32_t nFlag) override;
  bool OnLButtonUp(const CFX_PointF& point, uint32_t nFlag) override;
  void OnNotify(CPWL_Wnd* pWnd,
                uint32_t msg,
                intptr_t wParam = 0,
                intptr_t lParam = 0) override;
  void CreateChildWnd(const PWL_CREATEPARAM& cp) override;
  void TimerProc() override;

  float GetScrollBarWidth() const;
  PWL_SCROLLBAR_TYPE GetScrollBarType() const { return m_sbType; }

  void SetNotifyForever(bool bForever) { m_bNotifyForever = bForever; }

 protected:
  void SetScrollRange(float fMin, float fMax, float fClientWidth);
  void SetScrollPos(float fPos);
  void MovePosButton(bool bRefresh);
  void SetScrollStep(float fBigStep, float fSmallStep);
  void NotifyScrollWindow();
  CFX_FloatRect GetScrollArea() const;

 private:
  void CreateButtons(const PWL_CREATEPARAM& cp);

  void OnMinButtonLBDown(const CFX_PointF& point);
  void OnMinButtonLBUp(const CFX_PointF& point);
  void OnMinButtonMouseMove(const CFX_PointF& point);

  void OnMaxButtonLBDown(const CFX_PointF& point);
  void OnMaxButtonLBUp(const CFX_PointF& point);
  void OnMaxButtonMouseMove(const CFX_PointF& point);

  void OnPosButtonLBDown(const CFX_PointF& point);
  void OnPosButtonLBUp(const CFX_PointF& point);
  void OnPosButtonMouseMove(const CFX_PointF& point);

  float TrueToFace(float);
  float FaceToTrue(float);

  PWL_SCROLLBAR_TYPE m_sbType;
  PWL_SCROLL_INFO m_OriginInfo;
  CFX_UnownedPtr<CPWL_SBButton> m_pMinButton;
  CFX_UnownedPtr<CPWL_SBButton> m_pMaxButton;
  CFX_UnownedPtr<CPWL_SBButton> m_pPosButton;
  PWL_SCROLL_PRIVATEDATA m_sData;
  bool m_bMouseDown;
  bool m_bMinOrMax;
  bool m_bNotifyForever;
  float m_nOldPos;
  float m_fOldPosButton;
};

#endif  // FPDFSDK_PDFWINDOW_CPWL_SCROLL_BAR_H_