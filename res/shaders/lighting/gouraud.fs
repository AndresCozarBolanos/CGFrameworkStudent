varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D u_texture;

void main()
{
    vec4 tex_color = texture2D(u_texture, v_uv);
    gl_FragColor = tex_color * v_color;
}