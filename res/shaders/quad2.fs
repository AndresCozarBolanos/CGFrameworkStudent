varying vec2 v_uv;

void main()
{
	float d = distance(v_uv, vec2(0.5));
	vec3 col_b = vec3(d * 1);

	gl_FragColor = vec4(col_b, 1.0);
}
