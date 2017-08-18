#include "public/fpdfview.h"
#include "public/fpdf_edit.h"

#include "fpdfsdk/fsdk_define.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fpdfapi/cpdf_pageobject.h"
#include "core/fpdfapi/cpdf_pathobject.h"
#include "core/fpdfapi/cpdf_textobject.h"
#include "core/fpdfapi/cpdf_imageobject.h"


extern "C" {
  DLLEXPORT FPDF_BOOL STDCALL FPDFText_GetMatrix(FPDF_PAGEOBJECT text_page_object, FS_MATRIX* matrix);
  DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_GetMatrix(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix);
}


DLLEXPORT FPDF_BOOL STDCALL FPDFText_GetMatrix(FPDF_PAGEOBJECT text_page_object, FS_MATRIX* matrix) {
  if (!text_page_object)
    return false;

  CPDF_TextObject* pTextObject = static_cast<CPDF_TextObject*>(text_page_object);
  CFX_Matrix textMatrix = pTextObject->GetTextMatrix();

  matrix->a = textMatrix.a;
  matrix->b = textMatrix.b;
  matrix->c = textMatrix.c;
  matrix->d = textMatrix.d;
  matrix->e = textMatrix.e;
  matrix->f = textMatrix.f;
}

DLLEXPORT FPDF_BOOL STDCALL FPDFText_GetMatrix(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix) {
  if (!image_page_object)
    return false;

  CPDF_ImageObject* pImageObject = static_cast<CPDF_ImageObject*>(image_page_object);
  CFX_Matrix& imageMatrix = pImageObject->matrix();

  matrix->a = imageMatrix.a;
  matrix->b = imageMatrix.b;
  matrix->c = imageMatrix.c;
  matrix->d = imageMatrix.d;
  matrix->e = imageMatrix.e;
  matrix->f = imageMatrix.f;
}
