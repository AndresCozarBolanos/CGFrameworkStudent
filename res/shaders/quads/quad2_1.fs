varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
    // We want to create a grayscale version of the texture
    vec4 color = texture2D(u_texture, v_uv);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    
    gl_FragColor = vec4(vec3(gray), color.a);
}