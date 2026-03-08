varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main() {
    vec2 uv = v_uv;
    uv.x += sin(v_uv.y * 10.0 + u_time * 3.0) * 0.03;
    uv.y += cos(v_uv.x * 10.0 + u_time * 3.0) * 0.03;
    vec4 color = texture2D(u_texture, uv);

    vec3 inverted = 1.0 - color.rgb;
    float factor = 0.5 + 0.5 * sin(u_time * 3.0);
    vec3 finalColor = mix(color.rgb, inverted, factor);

    gl_FragColor = vec4(finalColor, color.a);
}