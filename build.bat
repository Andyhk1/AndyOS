cd "C:\Users\Andyhk\Documents\visual studio 2017\Projects\AndyOS\"

@SET Rebase="Rebase\bin\debug\Rebase.exe"
@SET FileAdder="FileAdder\bin\debug\FileAdder.exe"
@SET OS="Debug\andyos.exe"
@SET BootBinary="Root\boot\andyos.bin"
@SET ISOName="AndyOS.iso"

IF exist %windir%\sysnative\bash.exe (%windir%\sysnative\bash.exe -c "grub-mkrescue -o %ISOName% Root/") ELSE (bash.exe -c "grub-mkrescue -o %ISOName% Root/" & pause)