// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_CXFA_TEXTDATA_H_
#define XFA_FXFA_PARSER_CXFA_TEXTDATA_H_

#include "core/fxcrt/fx_string.h"
#include "xfa/fxfa/parser/cxfa_datadata.h"

class CXFA_Node;

class CXFA_TextData : public CXFA_DataData {
 public:
  explicit CXFA_TextData(CXFA_Node* pNode);

  void GetContent(WideString& wsText);
};

#endif  // XFA_FXFA_PARSER_CXFA_TEXTDATA_H_
