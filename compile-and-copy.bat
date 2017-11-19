@echo off

ninja -C out\x64 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out\x86 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out-debug\x64 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%
ninja -C out-debug\x86 pdfium
if %errorlevel% neq 0 exit /b %errorlevel%

copy /Y out\x64\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\ > nul
copy /Y out\x86\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\ > nul
copy /Y out-debug\x64\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\Debug\ > nul
copy /Y out-debug\x64\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x64\Debug\ > nul
copy /Y out-debug\x86\pdfium.dll E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\Debug\ > nul
copy /Y out-debug\x86\pdfium.dll.pdb E:\Git\dot-net-draftable\src\Draftable.PDF\PDFium\x86\Debug\ > nul

