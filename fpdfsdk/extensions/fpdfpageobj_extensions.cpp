#include "public/fpdfview.h"
#include "public/fpdf_edit.h"

#include "fpdfsdk/fsdk_define.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxge/cfx_pathdata.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/page/cpdf_textobject.h"
#include "core/fpdfapi/page/cpdf_imageobject.h"

#include <memory>
#include <algorithm>
#include <vector>

#include "core/fpdfapi/page/cpdf_docpagedata.h"
#include "core/fpdfapi/page/cpdf_image.h"


extern "C" {
  typedef struct _FPDF_PATHPOINT_ {
    float x;
    float y;
    int type; // actually an FXPT_TYPE
    int closeFigure; // actually a boolean
  } FPDF_PATHPOINT;


  DLLEXPORT int STDCALL FPDFPageObj_GetBlendMode(FPDF_PAGEOBJECT page_object, int bufferLength, char* buffer);

  DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetMatrix(FPDF_PAGEOBJECT path_page_object, FS_MATRIX* matrix);
  DLLEXPORT FPDF_BOOL STDCALL FPDFText_GetMatrix(FPDF_PAGEOBJECT text_page_object, FS_MATRIX* matrix);
  DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_GetMatrix(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix);

  DLLEXPORT FPDF_BOOL STDCALL FPDFPath_SetMatrix(FPDF_PAGEOBJECT path_page_object, FS_MATRIX* matrix);
  DLLEXPORT FPDF_BOOL STDCALL FPDFText_SetMatrix(FPDF_PAGEOBJECT text_page_object, FS_MATRIX* matrix);
  DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_SetMatrix_Draftable(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix);

  DLLEXPORT int STDCALL FPDFPath_GetPointCount(FPDF_PAGEOBJECT path_page_object);
  DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetPoints(FPDF_PAGEOBJECT path_page_object, FPDF_PATHPOINT* points);

  DLLEXPORT int STDCALL FPDFPath_GetFillMode(FPDF_PAGEOBJECT path_page_object);
  DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetStrokeEnabled(FPDF_PAGEOBJECT path_page_object);

  DLLEXPORT float STDCALL FPDFPath_GetLineWidth(FPDF_PAGEOBJECT path_page_object);
  DLLEXPORT int STDCALL FPDFPath_GetLineCap(FPDF_PAGEOBJECT path_page_object);
  DLLEXPORT int STDCALL FPDFPath_GetLineJoin(FPDF_PAGEOBJECT path_page_object);
}


DLLEXPORT int STDCALL FPDFPageObj_GetBlendMode(FPDF_PAGEOBJECT page_object, int bufferLength, char* buffer) {
  if (!page_object)
    return 0;

  CPDF_PageObject* pPageObject = static_cast<CPDF_PageObject*>(page_object);
  const CFX_ByteString blendMode = pPageObject->m_GeneralState.GetBlendMode();

  const int length = static_cast<int>(blendMode.GetStringLength());
  if (length > bufferLength || !buffer) {
    return length;
  }

  std::copy(blendMode.begin(), blendMode.begin() + length, buffer);

  return length;
}


DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetMatrix(FPDF_PAGEOBJECT path_page_object, FS_MATRIX* matrix) {
  if (!path_page_object)
    return false;

  CPDF_PathObject* pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  const CFX_Matrix& pathMatrix = pPathObject->m_Matrix;

  matrix->a = pathMatrix.a;
  matrix->b = pathMatrix.b;
  matrix->c = pathMatrix.c;
  matrix->d = pathMatrix.d;
  matrix->e = pathMatrix.e;
  matrix->f = pathMatrix.f;

  return true;
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

  return true;
}

DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_GetMatrix(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix) {
  if (!image_page_object)
    return false;

  CPDF_ImageObject* pImageObject = static_cast<CPDF_ImageObject*>(image_page_object);
  const CFX_Matrix& imageMatrix = pImageObject->matrix();

  matrix->a = imageMatrix.a;
  matrix->b = imageMatrix.b;
  matrix->c = imageMatrix.c;
  matrix->d = imageMatrix.d;
  matrix->e = imageMatrix.e;
  matrix->f = imageMatrix.f;

  return true;
}


DLLEXPORT FPDF_BOOL STDCALL FPDFPath_SetMatrix(FPDF_PAGEOBJECT path_page_object, FS_MATRIX* matrix) {
  if (!path_page_object)
    return false;

  CPDF_PathObject* pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  pPathObject->m_Matrix = CFX_Matrix(matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);

  pPathObject->CalcBoundingBox();
  pPathObject->SetDirty(true);

  return true;
}

DLLEXPORT FPDF_BOOL STDCALL FPDFText_SetMatrix(FPDF_PAGEOBJECT text_page_object, FS_MATRIX* matrix) {
  if (!text_page_object)
    return false;

  CPDF_TextObject* pTextObject = static_cast<CPDF_TextObject*>(text_page_object);
  
  float* pTextMatrix = pTextObject->m_TextState.GetMutableMatrix();
  pTextMatrix[0] = matrix->a;
  pTextMatrix[1] = matrix->c;
  pTextMatrix[2] = matrix->b;
  pTextMatrix[3] = matrix->d;
  pTextObject->SetPosition(matrix->e, matrix->f);
  
  pTextObject->RecalcPositionData();
  pTextObject->SetDirty(true);

  return true;
}

DLLEXPORT FPDF_BOOL STDCALL FPDFImageObj_SetMatrix_Draftable(FPDF_PAGEOBJECT image_page_object, FS_MATRIX* matrix) {
  if (!image_page_object)
    return false;

  CPDF_ImageObject* pImageObject = static_cast<CPDF_ImageObject*>(image_page_object);

  pImageObject->set_matrix(CFX_Matrix(matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f));

  pImageObject->CalcBoundingBox();
  pImageObject->SetDirty(true);

  return true;
}


DLLEXPORT int STDCALL FPDFPath_GetPointCount(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return 0;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  const std::vector<FX_PATHPOINT>& pathPoints = pPathObject->m_Path.GetPoints();

  return pathPoints.size();
}

DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetPoints(FPDF_PAGEOBJECT path_page_object, FPDF_PATHPOINT* points) {
  if (!path_page_object)
    return false;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  const std::vector<FX_PATHPOINT>& pathPoints = pPathObject->m_Path.GetPoints();

  for(int index = 0, size = (int) pathPoints.size(); index < size; ++index) {
    FPDF_PATHPOINT& point = points[index];
    const FX_PATHPOINT& pathPoint = pathPoints[index];

    point.x = pathPoint.m_Point.x;
    point.y = pathPoint.m_Point.y;
    point.type = static_cast<int>(pathPoint.m_Type);
    point.closeFigure = static_cast<int>(pathPoint.m_CloseFigure);
  }

  return true;
}


DLLEXPORT int STDCALL FPDFPath_GetFillMode(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return 0;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  return pPathObject->m_FillType;
}

DLLEXPORT FPDF_BOOL STDCALL FPDFPath_GetStrokeEnabled(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return 0;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  return pPathObject->m_bStroke;
}


DLLEXPORT float STDCALL FPDFPath_GetLineWidth(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return 0;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  return pPathObject->m_GraphState.GetLineWidth();
}

DLLEXPORT int STDCALL FPDFPath_GetLineCap(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return -1;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  return static_cast<int>(pPathObject->m_GraphState.GetLineCap());
}

DLLEXPORT int STDCALL FPDFPath_GetLineJoin(FPDF_PAGEOBJECT path_page_object) {
  if (!path_page_object)
    return -1;

  CPDF_PathObject * pPathObject = static_cast<CPDF_PathObject*>(path_page_object);
  return static_cast<int>(pPathObject->m_GraphState.GetLineJoin());
}
