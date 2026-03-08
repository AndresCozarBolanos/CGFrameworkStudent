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

void main()
{
    vec4 tex_color = texture2D(u_texture, v_uv);
    
    vec3 k_a = tex_color.rgb;     
    vec3 k_d = tex_color.rgb;     
    vec3 k_s = vec3(tex_color.a); 
    float S = 32.0;

    vec3 tex_normal = texture2D(u_normal_texture, v_uv).rgb;
    
    tex_normal = tex_normal * 2.0 - 1.0;
    
    vec3 world_tex_normal = normalize((u_model * vec4(tex_normal, 0.0)).xyz);
    
    vec3 N = normalize(mix(normalize(v_world_normal), world_tex_normal, 0.5));

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = reflect(-L, N);

    float dist = length(u_light_position - v_world_position);
    float attenuation = 1.0 / (1.0 + (dist * dist));
    vec3 I = u_light_color * 100.0; 

    vec3 ambient = k_a * u_ambient_light;
    vec3 diffuse = max(dot(L, N), 0.0) * k_d * I;
    vec3 specular = pow(max(dot(R, V), 0.0), S) * k_s * I;

    vec3 Ip = ambient + attenuation * (diffuse + specular);

    gl_FragColor = vec4(Ip, 1.0);
}