const ffmpeg = require('fluent-ffmpeg')
const { JSDOM } = require('jsdom')

const _http = require('http')
const _url = require('url')
const _fs = require('fs')
const _path = require('path')

JSDOM.fromFile(_path.resolve(__dirname, './src/index.html')).then(html => {
	const dom = html.window.document
	const videoPlayer = dom.getElementById('videoPlayer')
	if (videoPlayer) {
		const src = videoPlayer.getElementsByTagName('source')[0].src.replace('file:///', '')
		const videoType = src.split('.').pop().toLowerCase()
		if (!['webm', 'ogg', 'mp4'].find(val => val === videoType)) {
			/* todo 服务器输出视频流
			const videoServer = _http.createServer((req, res) => {
				let time = parseInt(_url.parse(req.url, true).query['time'])
				let ffmpegCmd = ffmpeg(src)
					.nativeFramerate()
					.format('mp4')
					.seekInput(time)
					.outputOptions('-movflags', 'frag_keyframe+empty_moov')
					.pipe()
					.pipe(res)
			}).listen(8888)
			*/
			/* 直接转码
			 */
			const stream = _fs.createWriteStream(_path.resolve(__dirname, './src/out.mp4'))
			new ffmpeg({ source: src })
				.nativeFramerate()
				.format('mp4')
				.on('error', e =>
					console.log(e)
				)
				.writeToStream(stream, { end: true })
		}
	}
})
