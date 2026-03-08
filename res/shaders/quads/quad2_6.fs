varying vec2 v_uv;
uniform sampler2D u_texture;
uniform vec2 u_resolution; 

void main() {
    vec2 texel = 1.0 / u_resolution; 
    
    float radio = 5.0; 
    vec2 offset = texel * radio;
    
    vec4 color = vec4(0.0);
    
    color += texture2D(u_texture, v_uv);
    
    color += texture2D(u_texture, v_uv + vec2(offset.x, 0.0));
    color += texture2D(u_texture, v_uv + vec2(-offset.x, 0.0));
    color += texture2D(u_texture, v_uv + vec2(0.0, offset.y));
    color += texture2D(u_texture, v_uv + vec2(0.0, -offset.y));
    
    color += texture2D(u_texture, v_uv + vec2(offset.x, offset.y));
    color += texture2D(u_texture, v_uv + vec2(-offset.x, offset.y));
    color += texture2D(u_texture, v_uv + vec2(offset.x, -offset.y));
    color += texture2D(u_texture, v_uv + vec2(-offset.x, -offset.y));

    gl_FragColor = color / 9.0;
}