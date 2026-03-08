varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
    float aspect = u_resolution.x / u_resolution.y;

    float wave = sin(v_uv.x * 3.14 * aspect) * 0.2 + 0.5;

    float masc = step(v_uv.y, wave);

    vec3 below = mix(vec3(0.0), vec3(0.0, 1.0, 0.0), v_uv.y);
    vec3 above = mix(1-vec3(0.0, 1.0, 0.0), vec3(1.0), v_uv.y);

    vec3 color = mix(1-above, below, masc);

    gl_FragColor = vec4(color, 1.0);
}