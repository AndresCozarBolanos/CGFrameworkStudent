varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;

uniform sampler2D u_texture;
uniform sampler2D u_normal_texture;
uniform mat4 u_model;

uniform int u_use_color;
uniform int u_use_specular;
uniform int u_use_normal;

void main()
{
    vec4 tex_color = texture2D(u_texture, v_uv);

    vec3 k_a = vec3(1.0);
    vec3 k_d = vec3(1.0);
    vec3 k_s = vec3(0.5);
    float S = 32.0;

    if (u_use_color == 1)
    {
        k_a = tex_color.rgb;
        k_d = tex_color.rgb;
    }

    if (u_use_specular == 1)
    {
        k_s = vec3(tex_color.a);
    }

    vec3 N = normalize(v_world_normal);

    if (u_use_normal == 1)
    {
        vec3 tex_normal = texture2D(u_normal_texture, v_uv).rgb;
        tex_normal = tex_normal * 2.0 - 1.0;
        vec3 world_tex_normal = normalize((u_model * vec4(tex_normal, 0.0)).xyz);
        N = normalize(mix(normalize(v_world_normal), world_tex_normal, 0.5));
    }

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = reflect(-L, N);

    vec3 ambient = k_a * u_ambient_light;
    vec3 diffuse = max(dot(L, N), 0.0) * k_d * u_light_color;
    vec3 specular = pow(max(dot(R, V), 0.0), S) * k_s * u_light_color;

    vec3 Ip = ambient + diffuse + specular;

    gl_FragColor = vec4(Ip, 1.0);
}