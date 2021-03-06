// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_CXFA_BINDITEMSDATA_H_
#define XFA_FXFA_PARSER_CXFA_BINDITEMSDATA_H_

#include "core/fxcrt/fx_string.h"
#include "xfa/fxfa/parser/cxfa_datadata.h"

class CXFA_Node;

class CXFA_BindItemsData : public CXFA_DataData {
 public:
  explicit CXFA_BindItemsData(CXFA_Node* pNode);

  void GetLabelRef(WideString& wsLabelRef);
  void GetValueRef(WideString& wsValueRef);
  void GetRef(WideString& wsRef);
  bool SetConnection(const WideString& wsConnection);
};

#endif  // XFA_FXFA_PARSER_CXFA_BINDITEMSDATA_H_
