varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	vec3 black    = vec3(0.0, 0.0, 0.0);
    vec3 red     = vec3(1.0, 0.0, 0.0);
    vec3 green    = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);

	float aspect = u_resolution.x / u_resolution.y;
	float cell_y = 16.0;
	float cell_x = cell_y * aspect;
	vec2 cells = vec2(cell_x, cell_y);
	vec2 c_uv = floor(v_uv * cells) / cells;

	vec3 bottom = mix(black, red, c_uv.x);
	vec3 top = mix(green, yellow, c_uv.x);
	vec3 color = mix(bottom, top, c_uv.y);	

	gl_FragColor = vec4(color, 1.0);
}