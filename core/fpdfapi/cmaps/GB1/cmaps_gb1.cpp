// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "core/fpdfapi/cmaps/GB1/cmaps_gb1.h"

#include "core/fpdfapi/cmaps/cmap_int.h"
#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/font/cpdf_fontglobals.h"
#include "core/fpdfapi/page/cpdf_pagemodule.h"

static const FXCMAP_CMap g_FXCMAP_GB1_cmaps[] = {
    {"GB-EUC-H", g_FXCMAP_GB_EUC_H_0, nullptr, 90, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, 0},
    {"GB-EUC-V", g_FXCMAP_GB_EUC_V_0, nullptr, 20, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -1},
    {"GBpc-EUC-H", g_FXCMAP_GBpc_EUC_H_0, nullptr, 91, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, 0},
    {"GBpc-EUC-V", g_FXCMAP_GBpc_EUC_V_0, nullptr, 20, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -1},
    {"GBK-EUC-H", g_FXCMAP_GBK_EUC_H_2, nullptr, 4071, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, 0},
    {"GBK-EUC-V", g_FXCMAP_GBK_EUC_V_2, nullptr, 20, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -1},
    {"GBKp-EUC-H", g_FXCMAP_GBKp_EUC_H_2, nullptr, 4070, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -2},
    {"GBKp-EUC-V", g_FXCMAP_GBKp_EUC_V_2, nullptr, 20, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -1},
    {"GBK2K-H", g_FXCMAP_GBK2K_H_5, g_FXCMAP_GBK2K_H_5_DWord, 4071, 1017,
     FXCMAP_CMap::Range, FXCMAP_CMap::Single, -4},
    {"GBK2K-V", g_FXCMAP_GBK2K_V_5, nullptr, 41, 0, FXCMAP_CMap::Range,
     FXCMAP_CMap::None, -1},
    {"UniGB-UCS2-H", g_FXCMAP_UniGB_UCS2_H_4, nullptr, 13825, 0,
     FXCMAP_CMap::Range, FXCMAP_CMap::None, 0},
    {"UniGB-UCS2-V", g_FXCMAP_UniGB_UCS2_V_4, nullptr, 24, 0,
     FXCMAP_CMap::Range, FXCMAP_CMap::None, -1},
    {"UniGB-UTF16-H", g_FXCMAP_UniGB_UCS2_H_4, nullptr, 13825, 0,
     FXCMAP_CMap::Range, FXCMAP_CMap::None, 0},
    {"UniGB-UTF16-V", g_FXCMAP_UniGB_UCS2_V_4, nullptr, 24, 0,
     FXCMAP_CMap::Range, FXCMAP_CMap::None, -1},
};

void CPDF_ModuleMgr::LoadEmbeddedGB1CMaps() {
  CPDF_FontGlobals* pFontGlobals =
      CPDF_ModuleMgr::Get()->GetPageModule()->GetFontGlobals();
  pFontGlobals->SetEmbeddedCharset(CIDSET_GB1, g_FXCMAP_GB1_cmaps,
                                   FX_ArraySize(g_FXCMAP_GB1_cmaps));
  pFontGlobals->SetEmbeddedToUnicode(CIDSET_GB1, g_FXCMAP_GB1CID2Unicode_5,
                                     30284);
}
