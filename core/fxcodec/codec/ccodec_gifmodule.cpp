// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "core/fxcodec/codec/ccodec_gifmodule.h"

#include "core/fxcodec/codec/codec_int.h"
#include "core/fxcodec/fx_codec.h"
#include "core/fxcodec/gif/cfx_gif.h"
#include "core/fxcodec/gif/cfx_gifcontext.h"
#include "core/fxge/fx_dib.h"
#include "third_party/base/ptr_util.h"

CCodec_GifModule::CCodec_GifModule() {}

CCodec_GifModule::~CCodec_GifModule() {}

std::unique_ptr<CCodec_GifModule::Context> CCodec_GifModule::Start(
    Delegate* pDelegate) {
  return pdfium::MakeUnique<CFX_GifContext>(this, pDelegate);
}

CFX_GifDecodeStatus CCodec_GifModule::ReadHeader(Context* pContext,
                                                 int* width,
                                                 int* height,
                                                 int* pal_num,
                                                 void** pal_pp,
                                                 int* bg_index,
                                                 CFX_DIBAttribute* pAttribute) {
  auto* context = static_cast<CFX_GifContext*>(pContext);
  CFX_GifDecodeStatus ret = context->ReadHeader();
  if (ret != CFX_GifDecodeStatus::Success)
    return ret;

  *width = context->width_;
  *height = context->height_;
  *pal_num = (2 << context->global_pal_exp_);
  *pal_pp = context->global_palette_.empty() ? nullptr
                                             : context->global_palette_.data();
  *bg_index = context->bc_index_;
  return CFX_GifDecodeStatus::Success;
}

CFX_GifDecodeStatus CCodec_GifModule::LoadFrameInfo(Context* pContext,
                                                    int* frame_num) {
  auto* context = static_cast<CFX_GifContext*>(pContext);
  CFX_GifDecodeStatus ret = context->GetFrame();
  if (ret != CFX_GifDecodeStatus::Success)
    return ret;

  *frame_num = context->GetFrameNum();
  return CFX_GifDecodeStatus::Success;
}

CFX_GifDecodeStatus CCodec_GifModule::LoadFrame(Context* pContext,
                                                int frame_num,
                                                CFX_DIBAttribute* pAttribute) {
  auto* context = static_cast<CFX_GifContext*>(pContext);
  CFX_GifDecodeStatus ret = context->LoadFrame(frame_num);
  if (ret != CFX_GifDecodeStatus::Success || !pAttribute)
    return ret;

  pAttribute->m_nGifLeft = context->images_[frame_num]->image_info.left;
  pAttribute->m_nGifTop = context->images_[frame_num]->image_info.top;
  pAttribute->m_fAspectRatio = context->pixel_aspect_;
  const uint8_t* buf =
      reinterpret_cast<const uint8_t*>(context->cmt_data_.GetBuffer(0));
  uint32_t len = context->cmt_data_.GetLength();
  if (len > 21) {
    uint8_t size = *buf++;
    if (size != 0)
      pAttribute->m_strAuthor = ByteString(buf, size);
    else
      pAttribute->m_strAuthor.clear();
  }
  return CFX_GifDecodeStatus::Success;
}

uint32_t CCodec_GifModule::GetAvailInput(Context* pContext,
                                         uint8_t** avail_buf_ptr) {
  auto* context = static_cast<CFX_GifContext*>(pContext);
  return context->GetAvailInput(avail_buf_ptr);
}

void CCodec_GifModule::Input(Context* pContext,
                             const uint8_t* src_buf,
                             uint32_t src_size) {
  auto* context = static_cast<CFX_GifContext*>(pContext);
  context->SetInputBuffer((uint8_t*)src_buf, src_size);
}
