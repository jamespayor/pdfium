// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/parser/cscript_eventpseudomodel.h"

#include "fxjs/cfxjse_arguments.h"
#include "fxjs/cjx_object.h"
#include "third_party/base/ptr_util.h"

CScript_EventPseudoModel::CScript_EventPseudoModel(CXFA_Document* pDocument)
    : CXFA_Object(pDocument,
                  XFA_ObjectType::Object,
                  XFA_Element::EventPseudoModel,
                  WideStringView(L"eventPseudoModel"),
                  pdfium::MakeUnique<CJX_EventPseudoModel>(this)) {}

CScript_EventPseudoModel::~CScript_EventPseudoModel() {}
