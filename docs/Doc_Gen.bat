@echo off

rmdir /s /q "../Documentation"
doxygen Doxyfile
start "" "../Documentation\html\index.html"
