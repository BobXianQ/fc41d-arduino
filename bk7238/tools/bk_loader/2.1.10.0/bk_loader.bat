@echo off
for /f "tokens=2 delims=M" %%a in ('echo %2') do (
    set "port_number=%%a"
    call %1\bk_loader.exe download -p %%port_number%% -b %3 -i %4 --fast-link 1
)
pause