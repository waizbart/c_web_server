@echo off

start threaded_server.exe

timeout /t 1 /nobreak >nul

start no_threaded_server.exe

pause
