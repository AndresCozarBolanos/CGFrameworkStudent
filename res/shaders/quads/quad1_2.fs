varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	float d = distance(v_uv, vec2(0.5, 0.5));
	vec3 color = mix(vec3(0.0), vec3(1.0), d);

	gl_FragColor = vec4(color, 1.0);
}
