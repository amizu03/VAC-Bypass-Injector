@echo off
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
    ) else (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
    )
)
set compilerflags=/Od /Zi /EHsc /std:c++latest /I include
set linkerflags=/OUT:bin\main.exe
set libs=kernel32.lib user32.lib advapi32.lib
cl.exe %compilerflags% src\*.cpp %libs% /link %linkerflags%
del bin\*.ilk *.obj *.pdb