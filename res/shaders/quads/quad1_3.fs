varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	float aspect = u_resolution.x / u_resolution.y;

	float line_v = sin(v_uv.x * 20.0 * aspect); 
    float line_h = sin(v_uv.y * 20.0 * aspect);

	vec3 col_v = mix(vec3(0.0), vec3(1.0, 0.0, 0.0), line_v);
    vec3 col_h = mix(vec3(0.0), vec3(0.0, 0.0, 1.0), line_h);

	vec3 color = col_v + col_h;
	gl_FragColor = vec4(color, 1.0);
}