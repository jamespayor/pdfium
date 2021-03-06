// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_CXFA_MARGINDATA_H_
#define XFA_FXFA_PARSER_CXFA_MARGINDATA_H_

#include "core/fxcrt/fx_system.h"
#include "xfa/fxfa/parser/cxfa_datadata.h"

class CXFA_Node;

class CXFA_MarginData : public CXFA_DataData {
 public:
  explicit CXFA_MarginData(CXFA_Node* pNode);

  bool GetLeftInset(float& fInset, float fDefInset = 0) const;
  bool GetTopInset(float& fInset, float fDefInset = 0) const;
  bool GetRightInset(float& fInset, float fDefInset = 0) const;
  bool GetBottomInset(float& fInset, float fDefInset = 0) const;
};

#endif  // XFA_FXFA_PARSER_CXFA_MARGINDATA_H_
