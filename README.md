# ProcRuntime
[![Status](https://travis-ci.com/Waves-rgb/ProcRuntime.svg?branch=main)](https://travis-ci.com/github/Waves-rgb/ProcRuntime)

Command line utility to check how fast programs are running.

OS support:
 - Windows
 - Linux (arch tested)

TODO:
 - [ ] Add support for macOS
 - [ ] Add color coding to console
 - [ ] Find a better alternative to `system();`

Usage: `ProcRuntime.exe -c 1000 -o runtimelog.txt app.exe`
> this runs app.exe 1000 times and writes logs to runtimelog.txt

Compile: `c++ ./Main.cpp -o ./ProcRuntime.exe`
> this compiles Main.cpp into ProcRuntime.exe.
