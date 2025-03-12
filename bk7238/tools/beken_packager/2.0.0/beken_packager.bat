@echo off
echo pwd  %CD%
%1\beken_packager.exe  %1\config.json
copy all_2M.1220.bin %2