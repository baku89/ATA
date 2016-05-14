#version 120

#pragma glslify: snoise2 = require(glsl-noise/simplex/2d)
#pragma glslify: snoise3 = require(glsl-noise/simplex/3d)
#pragma glslify: PI = require(glsl-pi)

uniform vec2 resolution;
uniform vec2 texResolution;

// flow attribute

uniform float speed;
uniform float opacity;
uniform float threshold;
uniform float offset;

uniform sampler2DRect prevTex;
uniform sampler2DRect coatTex;

varying vec2 uv;
varying vec2 coord;


// 09
// const float WIGGLE_AMP = 7.0;
// const float ANGLE_AMP = 4.0;
// const float DIR_AMP = 400.0;

// f
const float WIGGLE_AMP = 7.0;
const float ANGLE_AMP = 3.5;
const float DIR_AMP = 300.0;

// i
// const float WIGGLE_AMP = 2.5;
// const float ANGLE_AMP = 4.0;
// const float DIR_AMP = 150.0;

// last
// const float WIGGLE_AMP = 1.8;
// const float ANGLE_AMP = 6.0;
// const float DIR_AMP = 30.0;

// k
// const float WIGGLE_AMP = 2.8;
// const float ANGLE_AMP = 8.0;
// const float DIR_AMP = 300.0;

float brightness (vec4 c) {
	return (c.r + c.g + c.b) / 3.0;
}

float brightness (vec3 c) {
	return (c.r + c.g + c.b) / 3.0;
}


vec2 earthworm(vec4 prev, vec4 coat, vec2 pos) {
	float dir = offset * PI * 2.0;

	float wiggle = snoise2( (pos + offset) * WIGGLE_AMP/* - vec2(12.34, 1.053)*/);
	float angle = wiggle * ANGLE_AMP * PI + (cos(dir) * pos.x + sin(dir) * pos.y) * DIR_AMP;


	float amp = speed;//mix(0.5, 1.0, brightness(coat)) * speed;

 	return vec2(cos(angle), sin(angle)) * amp * 0.5;
}

void main() {
	
	// get original color
	vec4 prev = texture2DRect(prevTex, coord);
	vec4 coat = texture2DRect(coatTex, coord);
	
	// get flow and offset point of prev
	vec2 duv = uv;
	duv += earthworm(prev, coat, uv / vec2(1.0, 1.5));

	// i: clamp
	duv = clamp(duv, 0.0, 1.0);   
	// duv = mod(duv, resolution);

	vec2 dcoord = duv * resolution;

	prev = texture2DRect(prevTex, dcoord);

	// mix
	vec3 c = prev.rgb;

	c.rgb = mix(c.rgb, coat.rgb, opacity);

	// normal
	float st = mod(brightness(coat), 0.3);
	if (threshold > 0.01 && 0.1 < st && st < 0.15 ) {
		c = vec3(0.733, 0.866, 0.901); 
	}

	// cut_h version

	// float st = 0.5 - mod(brightness(c), 0.5);
	// if (threshold > 0.01 && st < threshold * 0.1) {
	// 	c = mix(c, vec3(0.733, 0.866, 0.901), threshold);
	// }

  gl_FragColor = vec4(c.r, c.g, c.b, 1.0);
}