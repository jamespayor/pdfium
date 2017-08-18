#include "public/fpdfview.h"

#include <memory>
#include <utility>
#include <vector>

#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/fpdf_parser_decode.h"
#include "core/fpdfapi/render/cpdf_progressiverenderer.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fpdfdoc/cpdf_annotlist.h"
#include "core/fpdfdoc/cpdf_nametree.h"
#include "core/fpdfdoc/cpdf_occontext.h"
#include "core/fpdfdoc/cpdf_viewerpreferences.h"
#include "core/fxcodec/fx_codec.h"
#include "core/fxcrt/fx_memory.h"
#include "core/fxcrt/fx_safe_types.h"
#include "core/fxge/cfx_defaultrenderdevice.h"
#include "core/fxge/cfx_gemodule.h"
#include "fpdfsdk/cpdfsdk_formfillenvironment.h"
#include "fpdfsdk/cpdfsdk_pageview.h"
#include "fpdfsdk/fsdk_define.h"
#include "fpdfsdk/fsdk_pauseadapter.h"
#include "fpdfsdk/javascript/ijs_runtime.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_ext.h"
#include "public/fpdf_progressive.h"


// Note: Most imports probably unnecessary - I just brought them over from fpdfview.cpp mostly...

extern "C" {
  DLLEXPORT FPDF_DOCUMENT STDCALL FPDF_LoadDocumentW(FPDF_WIDESTRING file_path, FPDF_BYTESTRING password);
}

// Taken straight from fpdfview.cpp:
static FPDF_DOCUMENT LoadDocumentImpl(
    const CFX_RetainPtr<IFX_SeekableReadStream>& pFileAccess,
    FPDF_BYTESTRING password) {
  if (!pFileAccess)
    return nullptr;

  auto pParser = pdfium::MakeUnique<CPDF_Parser>();
  pParser->SetPassword(password);

  auto pDocument = pdfium::MakeUnique<CPDF_Document>(std::move(pParser));
  CPDF_Parser::Error error =
      pDocument->GetParser()->StartParse(pFileAccess, pDocument.get());
  if (error != CPDF_Parser::SUCCESS) {
    ProcessParseError(error);
    return nullptr;
  }
  CheckUnSupportError(pDocument.get(), error);
  return FPDFDocumentFromCPDFDocument(pDocument.release());
}


DLLEXPORT FPDF_DOCUMENT STDCALL FPDF_LoadDocumentW(FPDF_WIDESTRING file_path,
                                                   FPDF_BYTESTRING password) {
  // NOTE: the creation of the file needs to be by the embedder on the
  // other side of this API.
  return LoadDocumentImpl(
      IFX_SeekableStream::CreateFromFilename((const wchar_t*)file_path, FX_FILEMODE_ReadOnly),
      password);
}
