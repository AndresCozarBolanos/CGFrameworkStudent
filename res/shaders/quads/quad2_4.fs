varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
    // We want to create a binary version of the texture
    vec4 color = texture2D(u_texture, v_uv);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    
    float umbral = step(0.5, gray); // if gray is greater than 0.5, umbral will be 1.0, otherwise it will be 0.0
    
    gl_FragColor = vec4(vec3(umbral), color.a);
}