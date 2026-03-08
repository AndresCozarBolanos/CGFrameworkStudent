varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
	// We want to create a radial gradient, so we will use the distance from the center
	vec2 st= v_uv - 0.5;

	// We want the gradient to be always circular
	float aspect = u_resolution.x / u_resolution.y;
	st.x *= aspect;

	float d = distance(st, vec2(0.0));
	vec3 color = mix(vec3(0.0), vec3(1.0), d);

	gl_FragColor = vec4(color, 1.0);
}
