// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_PARSER_XFA_UTILS_H_
#define XFA_FXFA_PARSER_XFA_UTILS_H_

#include "core/fxcrt/cfx_seekablestreamproxy.h"
#include "xfa/fxfa/fxfa_basic.h"

class CFX_XMLElement;
class CFX_XMLNode;
class CXFA_LocaleValue;
class CXFA_Node;
class CXFA_WidgetData;

double XFA_GetFractionalScale(uint32_t idx);
int XFA_GetMaxFractionalScale();

bool XFA_FDEExtension_ResolveNamespaceQualifier(CFX_XMLElement* pNode,
                                                const WideString& wsQualifier,
                                                WideString* wsNamespaceURI);

CXFA_LocaleValue XFA_GetLocaleValue(CXFA_WidgetData* pWidgetData);
int32_t XFA_MapRotation(int32_t nRotation);

bool XFA_RecognizeRichText(CFX_XMLElement* pRichTextXMLNode);
void XFA_GetPlainTextFromRichText(CFX_XMLNode* pXMLNode,
                                  WideString& wsPlainText);
bool XFA_FieldIsMultiListBox(CXFA_Node* pFieldNode);

void XFA_DataExporter_DealWithDataGroupNode(CXFA_Node* pDataNode);
void XFA_DataExporter_RegenerateFormFile(
    CXFA_Node* pNode,
    const RetainPtr<CFX_SeekableStreamProxy>& pStream,
    const char* pChecksum = nullptr,
    bool bSaveXML = false);

const XFA_NOTSUREATTRIBUTE* XFA_GetNotsureAttribute(
    XFA_Element eElement,
    XFA_Attribute eAttribute,
    XFA_AttributeType eType = XFA_AttributeType::NotSure);

const XFA_SCRIPTATTRIBUTEINFO* XFA_GetScriptAttributeByName(
    XFA_Element eElement,
    const WideStringView& wsAttributeName);

const XFA_PROPERTY* XFA_GetPropertyOfElement(XFA_Element eElement,
                                             XFA_Element eProperty,
                                             uint32_t dwPacket);
const XFA_PROPERTY* XFA_GetElementProperties(XFA_Element eElement,
                                             int32_t& iCount);
const XFA_Attribute* XFA_GetElementAttributes(XFA_Element eElement,
                                              int32_t& iCount);
const XFA_ELEMENTINFO* XFA_GetElementByID(XFA_Element eName);
XFA_Element XFA_GetElementTypeForName(const WideStringView& wsName);
bool XFA_GetAttributeDefaultValue(void*& pValue,
                                  XFA_Element eElement,
                                  XFA_Attribute eAttribute,
                                  XFA_AttributeType eType,
                                  uint32_t dwPacket);
const XFA_ATTRIBUTEINFO* XFA_GetAttributeByName(const WideStringView& wsName);
const XFA_ATTRIBUTEINFO* XFA_GetAttributeByID(XFA_Attribute eName);
const XFA_ATTRIBUTEENUMINFO* XFA_GetAttributeEnumByName(
    const WideStringView& wsName);
const XFA_PACKETINFO* XFA_GetPacketByIndex(XFA_PacketType ePacket);
const XFA_PACKETINFO* XFA_GetPacketByID(uint32_t dwPacket);

#endif  // XFA_FXFA_PARSER_XFA_UTILS_H_
