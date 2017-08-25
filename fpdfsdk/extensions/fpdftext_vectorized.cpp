#include "public/fpdfview.h"

#include "fpdfsdk/fsdk_define.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fpdftext/cpdf_textpage.h"


// Copied from elsewhere, because it's not exposed in the headers...
static inline CPDF_TextPage* CPDFTextPageFromFPDFTextPage(FPDF_TEXTPAGE text_page) {
  return static_cast<CPDF_TextPage*>(text_page);
}


typedef struct _FS_POINTF_ {
  float x;
  float y;
} FS_POINTF;

// We need to disable struct padding here, or else sad things can happen on the C# side...
#pragma pack(push, 1)
typedef struct _FPDF_EXT_CHARINFO_ {
  FS_MATRIX matrix;
  FS_RECTF box;
  FS_POINTF origin;
  float font_size;
  unsigned int unicode;
  unsigned int charcode;
} FPDF_EXT_CHARINFO;
#pragma pack(pop)


extern "C" {
  FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV FPDFText_GetCharInfo(FPDF_TEXTPAGE text_page, int start_index, int count, FPDF_EXT_CHARINFO* char_info);
}


FPDF_EXPORT FPDF_BOOL FPDF_CALLCONV FPDFText_GetCharInfo(FPDF_TEXTPAGE text_page, int start_index, int count, FPDF_EXT_CHARINFO* char_info) {
  if (!text_page)
    return 0;

  if (count == 0)
    return 1;

  if (count < 0)
    return 0;

  CPDF_TextPage* textpage = CPDFTextPageFromFPDFTextPage(text_page);
  if (start_index < 0 || start_index + count > textpage->CountChars())
    return 0;


  FPDF_CHAR_INFO charinfo;

  for (int offset = 0; offset < count; ++offset) {

    textpage->GetCharInfo(start_index + offset, &charinfo);

    FPDF_EXT_CHARINFO& out_char_info = char_info[offset];
    
    out_char_info.unicode = charinfo.m_Unicode;
    out_char_info.charcode = charinfo.m_Charcode;
    out_char_info.font_size = charinfo.m_FontSize;

    const CFX_PointF& origin = charinfo.m_Origin;
    out_char_info.origin.x = origin.x;
    out_char_info.origin.y = origin.y;

    const CFX_FloatRect& box = charinfo.m_CharBox;
    out_char_info.box.left = box.left;
    out_char_info.box.bottom = box.bottom;
    out_char_info.box.right = box.right;
    out_char_info.box.top = box.top;

    const CFX_Matrix matrix = charinfo.m_Matrix;
    out_char_info.matrix.a = matrix.a;
    out_char_info.matrix.b = matrix.b;
    out_char_info.matrix.c = matrix.c;
    out_char_info.matrix.d = matrix.d;
    out_char_info.matrix.e = matrix.e;
    out_char_info.matrix.f = matrix.f;

  }

  return 1;
}
