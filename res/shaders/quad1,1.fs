varying vec2 v_uv;

void main()
{
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 1.0);
	vec3 color_final = mix(blue, red, v_uv.x);
	gl_FragColor = vec4(color_final, 1.0);
}
