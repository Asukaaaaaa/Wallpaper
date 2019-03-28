const {app, BrowserWindow} = require('electron')

function createWindow() {
	// 创建浏览器窗口
	let win = new BrowserWindow({
		fullscreen: true,
		frame: false
	})

	// 然后加载 app 的 index.html.
	win.loadFile('./src/index.html')
}
// showVideo 时设置播放策略
app.commandLine.appendSwitch('autoplay-policy', 'no-user-gesture-required');
app.on('ready', createWindow)