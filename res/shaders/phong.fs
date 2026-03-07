varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform sampler2D u_texture;

void main()
{
    vec3 N = normalize(v_world_normal);

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = reflect(-L, N);

    vec3 k_a = vec3(1.0);
    vec3 k_d = vec3(1.0);
    vec3 k_s = vec3(0.5);
    float S = 32.0;

    float dist = length(u_light_position - v_world_position);
    float attenuation = 1.0 / (1.0 + (dist * dist));
    vec3 I = u_light_color * 100.0; 

    vec3 ambient = k_a * u_ambient_light;
    vec3 diffuse = max(dot(L, N), 0.0) * k_d * I;
    vec3 specular = pow(max(dot(R, V), 0.0), S) * k_s * I;

    vec3 Ip = ambient + attenuation * (diffuse + specular);

    vec4 tex_color = texture2D(u_texture, v_uv);

    gl_FragColor = vec4(Ip, 1.0) * tex_color;
}