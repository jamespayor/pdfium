// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FXCODEC_GIF_CFX_GIFCONTEXT_H_
#define CORE_FXCODEC_GIF_CFX_GIFCONTEXT_H_

#include <memory>
#include <vector>

#include "core/fxcodec/codec/ccodec_gifmodule.h"
#include "core/fxcodec/gif/cfx_gif.h"
#include "core/fxcodec/gif/cfx_lzwdecompressor.h"
#include "core/fxcrt/fx_string.h"
#include "core/fxcrt/unowned_ptr.h"

class CFX_GifContext : public CCodec_GifModule::Context {
 public:
  CFX_GifContext(CCodec_GifModule* gif_module,
                 CCodec_GifModule::Delegate* delegate);
  ~CFX_GifContext() override;

  void RecordCurrentPosition(uint32_t* cur_pos);
  void ReadScanline(int32_t row_num, uint8_t* row_buf);
  bool GetRecordPosition(uint32_t cur_pos,
                         int32_t left,
                         int32_t top,
                         int32_t width,
                         int32_t height,
                         int32_t pal_num,
                         CFX_GifPalette* pal,
                         int32_t delay_time,
                         bool user_input,
                         int32_t trans_index,
                         int32_t disposal_method,
                         bool interlace);
  CFX_GifDecodeStatus ReadHeader();
  CFX_GifDecodeStatus GetFrame();
  CFX_GifDecodeStatus LoadFrame(int32_t frame_num);
  void SetInputBuffer(uint8_t* src_buf, uint32_t src_size);
  uint32_t GetAvailInput(uint8_t** avail_buf) const;
  int32_t GetFrameNum() const;

  UnownedPtr<CCodec_GifModule> gif_module_;
  UnownedPtr<CCodec_GifModule::Delegate> delegate_;
  std::vector<CFX_GifPalette> global_palette_;
  uint8_t global_pal_exp_;
  uint32_t img_row_offset_;
  uint32_t img_row_avail_size_;
  uint32_t avail_in_;
  int32_t decode_status_;
  uint32_t skip_size_;
  ByteString cmt_data_;
  std::unique_ptr<CFX_GifGraphicControlExtension> graphic_control_extension_;
  uint8_t* next_in_;
  std::vector<std::unique_ptr<CFX_GifImage>> images_;
  std::unique_ptr<CFX_LZWDecompressor> lzw_decompressor_;
  int width_;
  int height_;
  uint8_t bc_index_;
  uint8_t pixel_aspect_;
  uint8_t global_sort_flag_;
  uint8_t global_color_resolution_;
  uint8_t img_pass_num_;

 protected:
  uint8_t* ReadData(uint8_t** des_buf_pp, uint32_t data_size);
  CFX_GifDecodeStatus ReadGifSignature();
  CFX_GifDecodeStatus ReadLogicalScreenDescriptor();

 private:
  void SaveDecodingStatus(int32_t status);
  CFX_GifDecodeStatus DecodeExtension();
  CFX_GifDecodeStatus DecodeImageInfo();
  void DecodingFailureAtTailCleanup(CFX_GifImage* gif_image);
};

#endif  // CORE_FXCODEC_GIF_CFX_GIFCONTEXT_H_
