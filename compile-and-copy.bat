@echo off

ninja -C out\x64 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out\x86 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out-debug\x64 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out-debug\x86 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%

cp out\x64\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\
cp out\x86\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\
cp out-debug\x64\pdfium.dll out-debug\x64\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\Debug\
cp out-debug\x86\pdfium.dll out-debug\x86\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\Debug\

