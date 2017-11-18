ninja -C out\x64 pdfium
ninja -C out\x86 pdfium
ninja -C out-debug\x64 pdfium
ninja -C out-debug\x86 pdfium

cp out\x64\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\
cp out\x86\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\
cp out-debug\x64\pdfium.dll out-debug\x64\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\Debug\
cp out-debug\x86\pdfium.dll out-debug\x86\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\Debug\

