varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main() {
    vec2 uv = v_uv;
    uv -= 0.5;

    float angulo = u_time * 0.5; 
    float s = sin(angulo);
    float c = cos(angulo);

    vec2 uv_rotative;
    uv_rotative.x = (uv.x * c) - (uv.y * s);
    uv_rotative.y = (uv.x * s) + (uv.y * c);
    uv_rotative += 0.5;

    vec4 color = texture2D(u_texture, uv_rotative);
    gl_FragColor = vec4(1.0 - color.rgb, color.a);
}