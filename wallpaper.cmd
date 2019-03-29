@echo off
cd %~dp0
if exist %1 ffmpeg -i %1 -y electron_player\src\out.mp4
start wallpaper2.cmd
electron_player\node_modules\.bin\electron electron_player\