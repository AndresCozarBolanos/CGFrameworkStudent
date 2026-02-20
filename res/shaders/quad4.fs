varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	float aspect = u_resolution.x / u_resolution.y;

	float celdas_y = 16.0;
	float celdas_x = celdas_y * aspect;
	vec2 celdas = vec2(celdas_x, celdas_y);
	vec2 uv_c = floor(v_uv * celdas) / celdas;

	vec3 black    = vec3(0.0, 0.0, 0.0);
    vec3 red     = vec3(1.0, 0.0, 0.0);
    vec3 green    = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);

	vec3 inferior = mix(black, red, uv_c.x);
	vec3 top = mix(green, yellow, uv_c.x);

	vec3 col_d = mix(inferior, top, uv_c.y);	

	gl_FragColor = vec4(col_d, 1.0);
}