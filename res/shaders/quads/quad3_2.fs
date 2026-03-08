varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main() {
    vec2 uv = v_uv - 0.5;

    float alpha = u_time * 0.2;
    float s = sin(alpha);
    float c = cos(alpha);

    vec2 uv_rot;
    uv_rot.x = uv.x * c - uv.y * s;
    uv_rot.y = uv.x * s + uv.y * c;
    uv_rot += 0.5;

    vec4 color = texture2D(u_texture, uv_rot);


    float state = mod(floor(u_time), 3.0);

    float r = step(0.5, state);
    float g = 1.0 - step(0.5, state) + step(1.5, state);

    vec3 semaforo = vec3(r, g, 0.0);

    gl_FragColor = vec4(color.rgb * semaforo, color.a);
}