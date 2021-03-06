// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_CXFA_WIDGETDATA_H_
#define XFA_FXFA_PARSER_CXFA_WIDGETDATA_H_

#include <vector>

#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_string.h"
#include "core/fxcrt/fx_system.h"
#include "xfa/fxfa/parser/cxfa_binddata.h"
#include "xfa/fxfa/parser/cxfa_borderdata.h"
#include "xfa/fxfa/parser/cxfa_calculatedata.h"
#include "xfa/fxfa/parser/cxfa_captiondata.h"
#include "xfa/fxfa/parser/cxfa_datadata.h"
#include "xfa/fxfa/parser/cxfa_fontdata.h"
#include "xfa/fxfa/parser/cxfa_margindata.h"
#include "xfa/fxfa/parser/cxfa_paradata.h"
#include "xfa/fxfa/parser/cxfa_validatedata.h"

enum XFA_CHECKSTATE {
  XFA_CHECKSTATE_On = 0,
  XFA_CHECKSTATE_Off = 1,
  XFA_CHECKSTATE_Neutral = 2,
};

enum XFA_VALUEPICTURE {
  XFA_VALUEPICTURE_Raw = 0,
  XFA_VALUEPICTURE_Display,
  XFA_VALUEPICTURE_Edit,
  XFA_VALUEPICTURE_DataBind,
};

class CXFA_Node;
class IFX_Locale;

class CXFA_WidgetData : public CXFA_DataData {
 public:
  explicit CXFA_WidgetData(CXFA_Node* pNode);

  CXFA_Node* GetUIChild();
  XFA_Element GetUIType();
  WideString GetRawValue();
  int32_t GetAccess();
  int32_t GetRotate();

  CXFA_BorderData GetBorderData(bool bModified);
  CXFA_CaptionData GetCaptionData();
  CXFA_FontData GetFontData(bool bModified);
  CXFA_MarginData GetMarginData();
  CXFA_ParaData GetParaData();
  std::vector<CXFA_Node*> GetEventList();
  std::vector<CXFA_Node*> GetEventByActivity(int32_t iActivity,
                                             bool bIsFormReady);
  CXFA_ValueData GetDefaultValueData();
  CXFA_ValueData GetFormValueData();
  CXFA_CalculateData GetCalculateData();
  CXFA_ValidateData GetValidateData(bool bModified);

  bool GetWidth(float& fWidth);
  bool GetHeight(float& fHeight);
  bool GetMinWidth(float& fMinWidth);
  bool GetMinHeight(float& fMinHeight);
  bool GetMaxWidth(float& fMaxWidth);
  bool GetMaxHeight(float& fMaxHeight);

  CXFA_BorderData GetUIBorderData();
  CFX_RectF GetUIMargin();
  XFA_ATTRIBUTEENUM GetButtonHighlight();
  bool GetButtonRollover(WideString& wsRollover, bool& bRichText);
  bool GetButtonDown(WideString& wsDown, bool& bRichText);
  XFA_ATTRIBUTEENUM GetCheckButtonShape();
  XFA_ATTRIBUTEENUM GetCheckButtonMark();
  float GetCheckButtonSize();
  bool IsAllowNeutral();
  bool IsRadioButton();
  XFA_CHECKSTATE GetCheckState();
  void SetCheckState(XFA_CHECKSTATE eCheckState, bool bNotify);
  CXFA_Node* GetExclGroupNode();
  CXFA_Node* GetSelectedMember();
  CXFA_Node* SetSelectedMember(const WideStringView& wsName, bool bNotify);
  void SetSelectedMemberByValue(const WideStringView& wsValue,
                                bool bNotify,
                                bool bScriptModify,
                                bool bSyncData);
  CXFA_Node* GetExclGroupFirstMember();
  CXFA_Node* GetExclGroupNextMember(CXFA_Node* pNode);
  XFA_ATTRIBUTEENUM GetChoiceListCommitOn();
  bool IsChoiceListAllowTextEntry();
  XFA_ATTRIBUTEENUM GetChoiceListOpen();
  bool IsListBox();
  int32_t CountChoiceListItems(bool bSaveValue);
  bool GetChoiceListItem(WideString& wsText, int32_t nIndex, bool bSaveValue);
  std::vector<WideString> GetChoiceListItems(bool bSaveValue);
  int32_t CountSelectedItems();
  int32_t GetSelectedItem(int32_t nIndex);
  std::vector<int32_t> GetSelectedItems();
  std::vector<WideString> GetSelectedItemsValue();
  bool GetItemState(int32_t nIndex);
  void SetItemState(int32_t nIndex,
                    bool bSelected,
                    bool bNotify,
                    bool bScriptModify,
                    bool bSyncData);
  void SetSelectedItems(const std::vector<int32_t>& iSelArray,
                        bool bNotify,
                        bool bScriptModify,
                        bool bSyncData);
  void ClearAllSelections();
  void InsertItem(const WideString& wsLabel,
                  const WideString& wsValue,
                  bool bNotify);
  void GetItemLabel(const WideStringView& wsValue, WideString& wsLabel);
  void GetItemValue(const WideStringView& wsLabel, WideString& wsValue);
  bool DeleteItem(int32_t nIndex, bool bNotify, bool bScriptModify);
  int32_t GetHorizontalScrollPolicy();
  int32_t GetNumberOfCells();
  bool SetValue(const WideString& wsValue, XFA_VALUEPICTURE eValueType);
  bool GetPictureContent(WideString& wsPicture, XFA_VALUEPICTURE ePicture);
  IFX_Locale* GetLocal();
  bool GetValue(WideString& wsValue, XFA_VALUEPICTURE eValueType);
  bool GetNormalizeDataValue(const WideString& wsValue,
                             WideString& wsNormalizeValue);
  bool GetFormatDataValue(const WideString& wsValue,
                          WideString& wsFormattedValue);
  void NormalizeNumStr(const WideString& wsValue, WideString& wsOutput);

  WideString GetBarcodeType();
  bool GetBarcodeAttribute_CharEncoding(int32_t* val);
  bool GetBarcodeAttribute_Checksum(bool* val);
  bool GetBarcodeAttribute_DataLength(int32_t* val);
  bool GetBarcodeAttribute_StartChar(char* val);
  bool GetBarcodeAttribute_EndChar(char* val);
  bool GetBarcodeAttribute_ECLevel(int32_t* val);
  bool GetBarcodeAttribute_ModuleWidth(int32_t* val);
  bool GetBarcodeAttribute_ModuleHeight(int32_t* val);
  bool GetBarcodeAttribute_PrintChecksum(bool* val);
  bool GetBarcodeAttribute_TextLocation(int32_t* val);
  bool GetBarcodeAttribute_Truncate(bool* val);
  bool GetBarcodeAttribute_WideNarrowRatio(float* val);
  void GetPasswordChar(WideString& wsPassWord);

  bool IsMultiLine();
  XFA_ATTRIBUTEENUM GetVerticalScrollPolicy();
  int32_t GetMaxChars(XFA_Element& eType);
  bool GetFracDigits(int32_t& iFracDigits);
  bool GetLeadDigits(int32_t& iLeadDigits);

  WideString NumericLimit(const WideString& wsValue,
                          int32_t iLead,
                          int32_t iTread) const;

  bool m_bIsNull;
  bool m_bPreNull;

 private:
  CXFA_BindData GetBindData();
  void SyncValue(const WideString& wsValue, bool bNotify);
  void InsertListTextItem(CXFA_Node* pItems,
                          const WideString& wsText,
                          int32_t nIndex);
  void FormatNumStr(const WideString& wsValue,
                    IFX_Locale* pLocale,
                    WideString& wsOutput);

  CXFA_Node* m_pUiChildNode;
  XFA_Element m_eUIType;
};

#endif  // XFA_FXFA_PARSER_CXFA_WIDGETDATA_H_
