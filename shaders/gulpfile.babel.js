const gulp = require('gulp')
const $ = require('gulp-load-plugins')()
const osc = require('node-osc')
const through = require('through2')

//==================================================
let client = new osc.Client('127.0.0.1', 1234)

gulp.task('glslify', () => {
	gulp.src('.')
		.pipe($.shell([
			'/usr/local/bin/glslify ./src/of-passthru.vert -o ../bin/data/shaders/passthru.vert',

			'/usr/local/bin/glslify ./src/of-brush-pass.frag -o ../bin/data/shaders/brush-pass.frag',
			'/usr/local/bin/glslify ./src/of-disp-pass.frag -o ../bin/data/shaders/disp-pass.frag'
		]))
		.pipe(through.obj(() => {
			client.send('/update-shader', 0)
		}))
})

//==================================================
gulp.task('watch', () => {
	gulp.watch(['./src/*.frag', './src/*.vert', './src/*.glsl'], ['glslify'])
})
//==================================================

gulp.task('default', ['glslify', 'watch'])
