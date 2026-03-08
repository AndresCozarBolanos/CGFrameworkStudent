varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
    // We want to create a yellow tint version of the texture
    vec4 color = texture2D(u_texture, v_uv);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // convert to grayscale
    vec3 yellow_tint = vec3(gray, gray, 0.0); // add yellow tint by keeping red and green channels and setting blue to 0
    
    gl_FragColor = vec4(yellow_tint, color.a);
}