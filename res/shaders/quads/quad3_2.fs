varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main() {
    // We want to create a rotating version of the texture
    vec2 uv = v_uv - 0.5;

    float alpha = u_time * 0.2;
    float s = sin(alpha);
    float c = cos(alpha);

    vec2 uv_rot;
    uv_rot.x = uv.x * c - uv.y * s;
    uv_rot.y = uv.x * s + uv.y * c;
    uv_rot += 0.5;

    vec4 color = texture2D(u_texture, uv_rot);

    // One we have the rotated color, we want to create a traffic light effect 
    // by multiplying the color with a vector that changes over time
    float state = mod(floor(u_time), 3.0);

    float r = step(0.5, state);
    float g = 1.0 - step(0.5, state) + step(1.5, state);

    vec3 semaforo = vec3(r, g, 0.0);

    gl_FragColor = vec4(color.rgb * semaforo, color.a);
}