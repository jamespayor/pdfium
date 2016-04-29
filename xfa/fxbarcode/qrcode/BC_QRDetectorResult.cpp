// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com
// Original code is licensed as follows:
/*
 * Copyright 2007 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xfa/fxbarcode/BC_ResultPoint.h"
#include "xfa/fxbarcode/common/BC_CommonBitMatrix.h"
#include "xfa/fxbarcode/qrcode/BC_QRDetectorResult.h"

CBC_QRDetectorResult::CBC_QRDetectorResult(
    CBC_CommonBitMatrix* bits,
    CFX_ArrayTemplate<CBC_ResultPoint*>* points)
    : m_bits(bits), m_points(points) {}

CBC_QRDetectorResult::~CBC_QRDetectorResult() {
  for (int32_t i = 0; i < m_points->GetSize(); i++)
    delete (*m_points)[i];
}

CBC_CommonBitMatrix* CBC_QRDetectorResult::GetBits() const {
  return m_bits.get();
}

CFX_ArrayTemplate<CBC_ResultPoint*>* CBC_QRDetectorResult::GetPoints() const {
  return m_points.get();
}
