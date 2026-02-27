varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_time;

void main() {
    vec2 uv = v_uv;
    uv.x += sin(uv.y * 10.0 + u_time * 3.0) * 0.05;
    vec4 color = texture2D(u_texture, uv);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    
    gl_FragColor = vec4(vec3(gray), color.a);
}