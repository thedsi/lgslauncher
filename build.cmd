@echo off
mkdir obj
mkdir bin
cl -c "%~dp0\dll.cpp" -Fo:obj\dll.o -nologo -std:c++latest -Os -permissive- -GS- -GL -Gy -GR- -Oy -Oi -MT -Zc:wchar_t -O1 -Zc:inline -Zc:forScope -D "_WINDLL" -D "_UNICODE" -D "UNICODE"
cl -c "%~dp0\exe.cpp" -Fo:obj\exe.o -nologo -std:c++latest -Os -permissive- -GS- -GL -Gy -GR- -Oy -Oi -MT -Zc:wchar_t -O1 -Zc:inline -Zc:forScope -D "_UNICODE" -D "UNICODE"
link "obj\dll.o" -OUT:"bin\lgslauncher.dll" -MANIFEST:NO -LTCG -NXCOMPAT -DYNAMICBASE "kernel32.lib" "shell32.lib" "libvcruntime.lib" -DLL -MACHINE:X64 -ENTRY:"DllMain" -OPT:REF -SUBSYSTEM:WINDOWS -OPT:ICF -NOLOGO -ALIGN:16 -MERGE:.pdata=.data -MERGE:.rdata=.data -IGNORE:4254,4108
link "obj\exe.o" -OUT:"bin\lgslauncher.exe" -MANIFEST:NO -LTCG -NXCOMPAT -DYNAMICBASE:NO "kernel32.lib" "libvcruntime.lib" -FIXED -MACHINE:X64 -ENTRY:"Start" -OPT:REF -SUBSYSTEM:WINDOWS -OPT:ICF -NOLOGO -ALIGN:16 -MERGE:.pdata=.data -MERGE:.rdata=.data -IGNORE:4254,4108