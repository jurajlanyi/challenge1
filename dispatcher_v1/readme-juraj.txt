--------


Windows UAC problem for dispatcher*.exe

QTCreator cannot run dispatcher*.exe

Solution:

put dispatcher_v1.exe.manifest to the executable drirectory and make sure .exe is newer than .manifest file

C:\work\challenge1\build-dispatcher_v1-Desktop_Qt_5_8_0_MinGW_32bit-Debug\debug\

https://blogs.msdn.microsoft.com/winsdk/2013/08/27/why-am-i-receiving-the-error-code-740-the-requested-operation-requires-elevation-when-launchingstarting-an-application/

https://answers.microsoft.com/en-us/windows/forum/windows_7-security/uac-prompts-on-any-program-with-the-word-patch-or/c5359497-d16e-43c6-99f2-db3d8eecc9c0

https://github.com/bmatzelle/gow/issues/156

https://math.nist.gov/oommf/software-patchsets/patch_on_Windows7.html
