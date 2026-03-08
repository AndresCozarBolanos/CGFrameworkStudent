varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
    // 
    vec4 color = texture2D(u_texture, v_uv);
    
    float dist = distance(v_uv, vec2(0.5, 0.5));
    
    float vignette = 1.0 - dist;
    
    gl_FragColor = vec4(color.rgb * vignette, color.a);
}