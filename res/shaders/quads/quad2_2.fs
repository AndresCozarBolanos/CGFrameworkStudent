varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
    // We want to create a negative version of the texture
    vec4 color = texture2D(u_texture, v_uv);
    gl_FragColor = vec4(1.0 - color.rgb, color.a);
}