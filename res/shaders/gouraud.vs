uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;

varying vec2 v_uv;
varying vec4 v_color; 

void main()
{  
    v_uv = gl_MultiTexCoord0.xy;

    vec3 world_position = (u_model * gl_Vertex).xyz;
    vec3 world_normal = normalize((u_model * vec4(gl_Normal, 0.0)).xyz);

    vec3 L = normalize(u_light_position - world_position);
    vec3 V = normalize(u_camera_position - world_position);
    vec3 R = reflect(-L, world_normal);

    vec3 k_a = vec3(1.0);
    vec3 k_d = vec3(1.0);
    vec3 k_s = vec3(0.5);
    float S = 32.0;

    float dist = length(u_light_position - world_position);
    float attenuation = 1.0;

    vec3 ambient = k_a * u_ambient_light;
    vec3 diffuse = max(dot(L, world_normal), 0.0) * k_d * u_light_color;
    vec3 specular = pow(max(dot(R, V), 0.0), S) * k_s * u_light_color;

    vec3 Ip = ambient + attenuation * (diffuse + specular);

    v_color = vec4(Ip, 1.0);

    gl_Position = u_viewprojection * vec4(world_position, 1.0);
}