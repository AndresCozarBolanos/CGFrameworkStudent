varying vec2 v_uv;
uniform vec2 u_resolution;

void main()
{
    float aspect = u_resolution.x / u_resolution.y;

    // We want to create a wave that moves up and down, 
    // and we want to use the aspect ratio to make it look good on different screen sizes

    float wave = sin(v_uv.x * 3.14 * aspect) * 0.2 + 0.5;

    // check if we are below the wave or above the wave
    float masc = step(v_uv.y, wave);

    vec3 below = mix(vec3(0.0), vec3(0.0, 1.0, 0.0), v_uv.y);
    vec3 above = mix(1-vec3(0.0, 1.0, 0.0), vec3(1.0), v_uv.y);

    vec3 color = mix(1-above, below, masc);

    gl_FragColor = vec4(color, 1.0);
}