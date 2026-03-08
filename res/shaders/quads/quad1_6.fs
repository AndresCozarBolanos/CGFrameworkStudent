varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
    float aspect = u_resolution.x / u_resolution.y;

    float wave = sin(v_uv.x * 3.14 * aspect) * 0.2 + 0.5;

    float masc = step(v_uv.y, wave);

    vec3 color = mix(vec3(0.0), vec3(0.0, 1.0, 0.0), v_uv.y);
    vec3 color_sky = mix(1-vec3(0.0, 1.0, 0.0), vec3(1.0), v_uv.y);

    vec3 col_f = mix(1-color_sky, color, masc);

    gl_FragColor = vec4(col_f, 1.0);
}