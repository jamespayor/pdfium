// Copyright 2015 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <limits>
#include <string>

#include "core/fxcrt/fx_coordinates.h"
#include "fpdfsdk/fpdfview_c_api_test.h"
#include "public/fpdfview.h"
#include "testing/embedder_test.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/utils/path_service.h"

namespace {

class MockDownloadHints : public FX_DOWNLOADHINTS {
 public:
  static void SAddSegment(FX_DOWNLOADHINTS* pThis, size_t offset, size_t size) {
  }

  MockDownloadHints() {
    FX_DOWNLOADHINTS::version = 1;
    FX_DOWNLOADHINTS::AddSegment = SAddSegment;
  }

  ~MockDownloadHints() {}
};

}  // namespace

TEST(fpdf, CApiTest) {
  EXPECT_TRUE(CheckPDFiumCApi());
}

class FPDFViewEmbeddertest : public EmbedderTest {};

TEST_F(FPDFViewEmbeddertest, Document) {
  EXPECT_TRUE(OpenDocument("about_blank.pdf"));
  EXPECT_EQ(1, GetPageCount());
  EXPECT_EQ(0, GetFirstPageNum());

  int version;
  EXPECT_TRUE(FPDF_GetFileVersion(document(), &version));
  EXPECT_EQ(14, version);

  EXPECT_EQ(0xFFFFFFFF, FPDF_GetDocPermissions(document()));
  EXPECT_EQ(-1, FPDF_GetSecurityHandlerRevision(document()));
}

TEST_F(FPDFViewEmbeddertest, LoadNonexistentDocument) {
  FPDF_DOCUMENT doc = FPDF_LoadDocument("nonexistent_document.pdf", "");
  ASSERT_FALSE(doc);
  EXPECT_EQ(static_cast<int>(FPDF_GetLastError()), FPDF_ERR_FILE);
}

// See bug 465.
TEST_F(FPDFViewEmbeddertest, EmptyDocument) {
  EXPECT_TRUE(CreateEmptyDocument());

  {
    int version = 42;
    EXPECT_FALSE(FPDF_GetFileVersion(document(), &version));
    EXPECT_EQ(0, version);
  }

  {
#ifndef PDF_ENABLE_XFA
    const unsigned long kExpected = 0;
#else
    const unsigned long kExpected = static_cast<uint32_t>(-1);
#endif
    EXPECT_EQ(kExpected, FPDF_GetDocPermissions(document()));
  }

  EXPECT_EQ(-1, FPDF_GetSecurityHandlerRevision(document()));

  EXPECT_EQ(0, FPDF_GetPageCount(document()));

  EXPECT_TRUE(FPDF_VIEWERREF_GetPrintScaling(document()));
  EXPECT_EQ(1, FPDF_VIEWERREF_GetNumCopies(document()));
  EXPECT_EQ(DuplexUndefined, FPDF_VIEWERREF_GetDuplex(document()));

  char buf[100];
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", nullptr, 0));
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", buf, sizeof(buf)));

  EXPECT_EQ(0u, FPDF_CountNamedDests(document()));
}

TEST_F(FPDFViewEmbeddertest, LinearizedDocument) {
  EXPECT_TRUE(OpenDocument("feature_linearized_loading.pdf", nullptr, true));
  int version;
  EXPECT_TRUE(FPDF_GetFileVersion(document(), &version));
  EXPECT_EQ(16, version);
}

TEST_F(FPDFViewEmbeddertest, Page) {
  EXPECT_TRUE(OpenDocument("about_blank.pdf"));
  FPDF_PAGE page = LoadPage(0);
  EXPECT_NE(nullptr, page);
  EXPECT_EQ(612.0, FPDF_GetPageWidth(page));
  EXPECT_EQ(792.0, FPDF_GetPageHeight(page));
  UnloadPage(page);
  EXPECT_EQ(nullptr, LoadPage(1));
}

TEST_F(FPDFViewEmbeddertest, ViewerRefDummy) {
  EXPECT_TRUE(OpenDocument("about_blank.pdf"));
  EXPECT_TRUE(FPDF_VIEWERREF_GetPrintScaling(document()));
  EXPECT_EQ(1, FPDF_VIEWERREF_GetNumCopies(document()));
  EXPECT_EQ(DuplexUndefined, FPDF_VIEWERREF_GetDuplex(document()));

  char buf[100];
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", nullptr, 0));
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", buf, sizeof(buf)));
}

TEST_F(FPDFViewEmbeddertest, ViewerRef) {
  EXPECT_TRUE(OpenDocument("viewer_ref.pdf"));
  EXPECT_TRUE(FPDF_VIEWERREF_GetPrintScaling(document()));
  EXPECT_EQ(5, FPDF_VIEWERREF_GetNumCopies(document()));
  EXPECT_EQ(DuplexUndefined, FPDF_VIEWERREF_GetDuplex(document()));

  // Test some corner cases.
  char buf[100];
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "", buf, sizeof(buf)));
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", nullptr, 0));
  EXPECT_EQ(0U, FPDF_VIEWERREF_GetName(document(), "foo", buf, sizeof(buf)));

  // Make sure |buf| does not get written into when it appears to be too small.
  // NOLINTNEXTLINE(runtime/printf)
  strcpy(buf, "ABCD");
  EXPECT_EQ(4U, FPDF_VIEWERREF_GetName(document(), "Foo", buf, 1));
  EXPECT_STREQ("ABCD", buf);

  // Note "Foo" is a different key from "foo".
  EXPECT_EQ(4U,
            FPDF_VIEWERREF_GetName(document(), "Foo", nullptr, sizeof(buf)));
  ASSERT_EQ(4U, FPDF_VIEWERREF_GetName(document(), "Foo", buf, sizeof(buf)));
  EXPECT_STREQ("foo", buf);

  // Try to retrieve a boolean and an integer.
  EXPECT_EQ(
      0U, FPDF_VIEWERREF_GetName(document(), "HideToolbar", buf, sizeof(buf)));
  EXPECT_EQ(0U,
            FPDF_VIEWERREF_GetName(document(), "NumCopies", buf, sizeof(buf)));

  // Try more valid cases.
  ASSERT_EQ(4U,
            FPDF_VIEWERREF_GetName(document(), "Direction", buf, sizeof(buf)));
  EXPECT_STREQ("R2L", buf);
  ASSERT_EQ(8U,
            FPDF_VIEWERREF_GetName(document(), "ViewArea", buf, sizeof(buf)));
  EXPECT_STREQ("CropBox", buf);
}

TEST_F(FPDFViewEmbeddertest, NamedDests) {
  EXPECT_TRUE(OpenDocument("named_dests.pdf"));
  long buffer_size;
  char fixed_buffer[512];
  FPDF_DEST dest;

  // Query the size of the first item.
  buffer_size = 2000000;  // Absurdly large, check not used for this case.
  dest = FPDF_GetNamedDest(document(), 0, nullptr, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(12, buffer_size);

  // Try to retrieve the first item with too small a buffer.
  buffer_size = 10;
  dest = FPDF_GetNamedDest(document(), 0, fixed_buffer, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(-1, buffer_size);

  // Try to retrieve the first item with correctly sized buffer. Item is
  // taken from Dests NameTree in named_dests.pdf.
  buffer_size = 12;
  dest = FPDF_GetNamedDest(document(), 0, fixed_buffer, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(12, buffer_size);
  EXPECT_EQ(std::string("F\0i\0r\0s\0t\0\0\0", 12),
            std::string(fixed_buffer, buffer_size));

  // Try to retrieve the second item with ample buffer. Item is taken
  // from Dests NameTree but has a sub-dictionary in named_dests.pdf.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 1, fixed_buffer, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(10, buffer_size);
  EXPECT_EQ(std::string("N\0e\0x\0t\0\0\0", 10),
            std::string(fixed_buffer, buffer_size));

  // Try to retrieve third item with ample buffer. Item is taken
  // from Dests NameTree but has a bad sub-dictionary in named_dests.pdf.
  // in named_dests.pdf).
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 2, fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.

  // Try to retrieve the forth item with ample buffer. Item is taken
  // from Dests NameTree but has a vale of the wrong type in named_dests.pdf.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 3, fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.

  // Try to retrieve fifth item with ample buffer. Item taken from the
  // old-style Dests dictionary object in named_dests.pdf.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 4, fixed_buffer, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(30, buffer_size);
  EXPECT_EQ(std::string("F\0i\0r\0s\0t\0A\0l\0t\0e\0r\0n\0a\0t\0e\0\0\0", 30),
            std::string(fixed_buffer, buffer_size));

  // Try to retrieve sixth item with ample buffer. Item istaken from the
  // old-style Dests dictionary object but has a sub-dictionary in
  // named_dests.pdf.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 5, fixed_buffer, &buffer_size);
  EXPECT_NE(nullptr, dest);
  EXPECT_EQ(28, buffer_size);
  EXPECT_EQ(std::string("L\0a\0s\0t\0A\0l\0t\0e\0r\0n\0a\0t\0e\0\0\0", 28),
            std::string(fixed_buffer, buffer_size));

  // Try to retrieve non-existent item with ample buffer.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), 6, fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.

  // Try to underflow/overflow the integer index.
  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), std::numeric_limits<int>::max(),
                           fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.

  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), std::numeric_limits<int>::min(),
                           fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.

  buffer_size = sizeof(fixed_buffer);
  dest = FPDF_GetNamedDest(document(), -1, fixed_buffer, &buffer_size);
  EXPECT_EQ(nullptr, dest);
  EXPECT_EQ(sizeof(fixed_buffer),
            static_cast<size_t>(buffer_size));  // unmodified.
}

TEST_F(FPDFViewEmbeddertest, NamedDestsByName) {
  EXPECT_TRUE(OpenDocument("named_dests.pdf"));

  // Null pointer returns nullptr.
  FPDF_DEST dest = FPDF_GetNamedDestByName(document(), nullptr);
  EXPECT_EQ(nullptr, dest);

  // Empty string returns nullptr.
  dest = FPDF_GetNamedDestByName(document(), "");
  EXPECT_EQ(nullptr, dest);

  // Item from Dests NameTree.
  dest = FPDF_GetNamedDestByName(document(), "First");
  EXPECT_NE(nullptr, dest);

  long ignore_len = 0;
  FPDF_DEST dest_by_index =
      FPDF_GetNamedDest(document(), 0, nullptr, &ignore_len);
  EXPECT_EQ(dest_by_index, dest);

  // Item from Dests dictionary.
  dest = FPDF_GetNamedDestByName(document(), "FirstAlternate");
  EXPECT_NE(nullptr, dest);

  ignore_len = 0;
  dest_by_index = FPDF_GetNamedDest(document(), 4, nullptr, &ignore_len);
  EXPECT_EQ(dest_by_index, dest);

  // Bad value type for item from Dests NameTree array.
  dest = FPDF_GetNamedDestByName(document(), "WrongType");
  EXPECT_EQ(nullptr, dest);

  // No such destination in either Dest NameTree or dictionary.
  dest = FPDF_GetNamedDestByName(document(), "Bogus");
  EXPECT_EQ(nullptr, dest);
}

// The following tests pass if the document opens without crashing.
TEST_F(FPDFViewEmbeddertest, Crasher_113) {
  EXPECT_TRUE(OpenDocument("bug_113.pdf"));
}

TEST_F(FPDFViewEmbeddertest, Crasher_451830) {
  // Document is damaged and can't be opened.
  EXPECT_FALSE(OpenDocument("bug_451830.pdf"));
}

TEST_F(FPDFViewEmbeddertest, Crasher_452455) {
  EXPECT_TRUE(OpenDocument("bug_452455.pdf"));
  FPDF_PAGE page = LoadPage(0);
  EXPECT_NE(nullptr, page);
  UnloadPage(page);
}

TEST_F(FPDFViewEmbeddertest, Crasher_454695) {
  // Document is damaged and can't be opened.
  EXPECT_FALSE(OpenDocument("bug_454695.pdf"));
}

TEST_F(FPDFViewEmbeddertest, Crasher_572871) {
  EXPECT_TRUE(OpenDocument("bug_572871.pdf"));
}

// It tests that document can still be loaded even the trailer has no 'Size'
// field if other information is right.
TEST_F(FPDFViewEmbeddertest, Failed_213) {
  EXPECT_TRUE(OpenDocument("bug_213.pdf"));
}

// The following tests pass if the document opens without infinite looping.
TEST_F(FPDFViewEmbeddertest, Hang_298) {
  EXPECT_FALSE(OpenDocument("bug_298.pdf"));
}

// Test if the document opens without infinite looping.
// Previously this test will hang in a loop inside LoadAllCrossRefV4. After
// the fix, LoadAllCrossRefV4 will return false after detecting a cross
// reference loop. Cross references will be rebuilt successfully.
TEST_F(FPDFViewEmbeddertest, CrossRefV4Loop) {
  EXPECT_TRUE(OpenDocument("bug_xrefv4_loop.pdf"));
  MockDownloadHints hints;

  // Make sure calling FPDFAvail_IsDocAvail() on this file does not infinite
  // loop either. See bug 875.
  int ret = PDF_DATA_NOTAVAIL;
  while (ret == PDF_DATA_NOTAVAIL)
    ret = FPDFAvail_IsDocAvail(avail_, &hints);
  EXPECT_EQ(PDF_DATA_AVAIL, ret);
}

// The test should pass when circular references to ParseIndirectObject will not
// cause infinite loop.
TEST_F(FPDFViewEmbeddertest, Hang_343) {
  EXPECT_FALSE(OpenDocument("bug_343.pdf"));
}

// The test should pass when the absence of 'Contents' field in a signature
// dictionary will not cause an infinite loop in CPDF_SyntaxParser::GetObject().
TEST_F(FPDFViewEmbeddertest, Hang_344) {
  EXPECT_FALSE(OpenDocument("bug_344.pdf"));
}

// The test should pass when there is no infinite recursion in
// CPDF_SyntaxParser::GetString().
TEST_F(FPDFViewEmbeddertest, Hang_355) {
  EXPECT_FALSE(OpenDocument("bug_355.pdf"));
}
// The test should pass even when the file has circular references to pages.
TEST_F(FPDFViewEmbeddertest, Hang_360) {
  EXPECT_FALSE(OpenDocument("bug_360.pdf"));
}

TEST_F(FPDFViewEmbeddertest, FPDF_RenderPageBitmapWithMatrix) {
  const char* const kRotatedMD5[4] = {
      "0a90de37f52127619c3dfb642b5fa2fe", "d599429574ff0dcad3bc898ea8b874ca",
      "0113386bb0bd45125bacc6dee78bfe78", "051fcfa4c1f9de28765705633a8ef3a9"};
  const char kTopLeftQuarterMD5[] = "4982be08db3f6d2e6409186ebbced9eb";
  const char kHoriStretchedMD5[] = "004bf38f3c5c76a644e6fca204747f21";
  const char kRotateandStretchMD5[] = "0ea95cacc716d003cf063a2c5ed6c8d7";

  EXPECT_TRUE(OpenDocument("rectangles.pdf"));
  FPDF_PAGE page = LoadPage(0);
  EXPECT_NE(nullptr, page);
  const int initial_width = static_cast<int>(FPDF_GetPageWidth(page));
  const int initial_height = static_cast<int>(FPDF_GetPageHeight(page));
  EXPECT_EQ(200, initial_width);
  EXPECT_EQ(300, initial_height);

  FPDF_BITMAP bitmap = RenderPage(page);
  CompareBitmap(bitmap, initial_width, initial_height, kRotatedMD5[0]);
  FPDFBitmap_Destroy(bitmap);

  // Try the easy rotations: 0, 90, 180, 270 clockwise. The output should be the
  // same as FPDF_RenderPageBitmap with the appropriate rotation flag. Per PDF
  // spec section 4.2.2, a t degree rotation is represented by [cos(t) sin(t)
  // -sin(t) cos(t) 0 0] (matrix goes on the right in the multiplication).
  FS_RECTF rect = {0, 0, initial_width, initial_height};
  CFX_Matrix rot_matrices[4] = {
      CFX_Matrix(1, 0, 0, 1, 0, 0), CFX_Matrix(0, -1, 1, 0, 0, 0),
      CFX_Matrix(-1, 0, 0, -1, 0, 0), CFX_Matrix(0, 1, -1, 0, 0, 0)};
  for (int rot = 0; rot < 4; ++rot) {
    int width;
    int height;
    FS_MATRIX matrix;
    matrix.a = rot_matrices[rot].a;
    matrix.b = rot_matrices[rot].b;
    matrix.c = rot_matrices[rot].c;
    matrix.d = rot_matrices[rot].d;
    matrix.e = rot_matrices[rot].e;
    matrix.f = rot_matrices[rot].f;
    if (rot % 2 == 0) {
      width = initial_width;
      height = initial_height;
    } else {
      width = initial_height;
      height = initial_width;
    }
    rect.right = width;
    rect.bottom = height;

    bitmap = FPDFBitmap_Create(width, height, 0);
    FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
    FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, rot, 0);
    CompareBitmap(bitmap, width, height, kRotatedMD5[rot]);
    FPDFBitmap_Destroy(bitmap);

    bitmap = FPDFBitmap_Create(width, height, 0);
    FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
    FPDF_RenderPageBitmapWithMatrix(bitmap, page, &matrix, &rect, 0);
    CompareBitmap(bitmap, width, height, kRotatedMD5[rot]);
    FPDFBitmap_Destroy(bitmap);
  }
  // TODO(npm): what to do with transformations that do not align the page with
  // the axis, like a 45 degree rotation (currently, part of the page gets cut
  // out). pdfium:849

  // Now render again with the image scaled smaller.
  int width = initial_width / 2;
  int height = initial_height / 2;
  FS_MATRIX matrix = {0.5, 0, 0, 0.5, 0, 0};

  rect.right = width;
  rect.bottom = height;

  bitmap = FPDFBitmap_Create(width, height, 0);
  FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
  FPDF_RenderPageBitmapWithMatrix(bitmap, page, &matrix, &rect, 0);
  CompareBitmap(bitmap, width, height, kTopLeftQuarterMD5);
  FPDFBitmap_Destroy(bitmap);

  // Now render again with the image scaled larger horizontally.
  width = initial_width * 2;
  height = initial_height;
  matrix.a = 2;
  matrix.d = 1;
  rect.right = width;
  rect.bottom = height;
  bitmap = FPDFBitmap_Create(width, height, 0);
  FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
  FPDF_RenderPageBitmapWithMatrix(bitmap, page, &matrix, &rect, 0);
  CompareBitmap(bitmap, width, height, kHoriStretchedMD5);
  FPDFBitmap_Destroy(bitmap);

  // Test a rotation followed by a stretch.
  width = initial_height * 2;
  height = initial_width;
  matrix.a = 0;
  matrix.b = -1;
  matrix.c = 2;
  matrix.d = 0;
  matrix.e = 0;
  matrix.f = 0;
  rect.right = width;
  rect.bottom = height;
  bitmap = FPDFBitmap_Create(width, height, 0);
  FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
  FPDF_RenderPageBitmapWithMatrix(bitmap, page, &matrix, &rect, 0);
  CompareBitmap(bitmap, width, height, kRotateandStretchMD5);
  FPDFBitmap_Destroy(bitmap);

  UnloadPage(page);
}

class UnSupRecordDelegate : public EmbedderTest::Delegate {
 public:
  UnSupRecordDelegate() : type_(-1) {}
  ~UnSupRecordDelegate() override {}

  void UnsupportedHandler(int type) override { type_ = type; }

  int type_;
};

TEST_F(FPDFViewEmbeddertest, UnSupportedOperations_NotFound) {
  UnSupRecordDelegate delegate;
  SetDelegate(&delegate);
  ASSERT_TRUE(OpenDocument("hello_world.pdf"));
  EXPECT_EQ(delegate.type_, -1);
  SetDelegate(nullptr);
}

TEST_F(FPDFViewEmbeddertest, UnSupportedOperations_LoadCustomDocument) {
  UnSupRecordDelegate delegate;
  SetDelegate(&delegate);
  ASSERT_TRUE(OpenDocument("unsupported_feature.pdf"));
  EXPECT_EQ(FPDF_UNSP_DOC_PORTABLECOLLECTION, delegate.type_);
  SetDelegate(nullptr);
}

TEST_F(FPDFViewEmbeddertest, UnSupportedOperations_LoadDocument) {
  std::string file_path;
  ASSERT_TRUE(
      PathService::GetTestFilePath("unsupported_feature.pdf", &file_path));

  UnSupRecordDelegate delegate;
  SetDelegate(&delegate);
  FPDF_DOCUMENT doc = FPDF_LoadDocument(file_path.c_str(), "");
  EXPECT_TRUE(doc != nullptr);
  EXPECT_EQ(FPDF_UNSP_DOC_PORTABLECOLLECTION, delegate.type_);
  FPDF_CloseDocument(doc);
  SetDelegate(nullptr);
}
