varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	// we want to create a chessboard pattern
	// we alyways want to have 16 cells in the y direction, and the x direction will scale with the aspect ratio
	float aspect = u_resolution.x / u_resolution.y;
	float celdas_y = 16.0;
	float celdas_x = celdas_y * aspect;
	vec2 celdas = vec2(celdas_x, celdas_y);

	vec2 grid = floor(v_uv * celdas);
	float patron = mod(grid.x + grid.y, 2.0);
	vec3 color = mix(vec3(0.0), vec3(1.0), patron);	

	gl_FragColor = vec4(color, 1.0);
}