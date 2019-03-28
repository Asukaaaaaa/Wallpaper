cd $(dirname $0)
if [ -f $1'' ]; then
    `ffmpeg -i $1 -y ./electron_player/src/out.mp4`
fi
`./electron_player/node_modules/.bin/electron ./electron_player` &
sleep 2 && `./window_graber/main.exe`
wait