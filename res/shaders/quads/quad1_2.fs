varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	float aspect = u_resolution.x / u_resolution.y;

	vec2 st= v_uv - 0.5;
	st.x *= aspect;

	float d = distance(st, vec2(0.0));
	vec3 col_b = mix(vec3(0.0), vec3(1.0), d);

	gl_FragColor = vec4(col_b, 1.0);
}
