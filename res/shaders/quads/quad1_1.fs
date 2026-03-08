varying vec2 v_uv;

void main()
{
	// We mix between red and blue based on the x coordinate of the UV
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 1.0);
	vec3 color = mix(blue, red, v_uv.x); // uv.x = 0 -> blue, uv.x = 1 -> red
	gl_FragColor = vec4(color, 1.0);
}
