#version 450 core

struct DirectionalLight 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;
};

struct PointLight
{
	vec3 position;

	float k_constant;
	float k_linear;
	float k_quadratic;
};

struct SpotLight 
{
	float cutoff;
	float outer_cutoff;
};

struct Material
{
	sampler2D diffuse_map;
	vec3 specular;
	float shininess;
};

float near = 0.3f;
float far = 10000.f;
float k_constant = 1.0f;
float k_linear = 0.09f;
float k_quadratic = 0.0032f;

vec3 directional_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 point_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction);
float LinearizeDepth(float depth);

#define NR_POINT_LIGHTS 4

layout (location = 0) out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 viewer_position;
uniform vec3 light_position;
uniform vec3 light_color;

uniform float bIsDirectionLightEnabled = 0.f;
uniform float bIsPointLightEnabled = 0.f;
uniform float bIsSpotLightEnabled = 0.f;

uniform DirectionalLight dir_light;
uniform PointLight point_light[NR_POINT_LIGHTS];
uniform Material material;

in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

void main()
{
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 result = (bIsDirectionLightEnabled * directional_light_calculations(normal, frag_position, viewer_direction));
	float depth = LinearizeDepth(frag_position.z);
	result += (bIsPointLightEnabled * point_light_calculations(normal, frag_position, viewer_direction));
	FragColor = vec4(result + light_color, 1.f);
}

float LinearizeDepth(float depth) 
{
	float z = depth * 2 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 directional_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(-light_position);
	float diff = max(dot(normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), 32);//material.shininess);
	vec3 ambient = vec3(texture(ourTexture, texCoord));
	vec3 diffuse = diff * vec3(texture(ourTexture, texCoord));
	vec3 specular = spec * vec3(texture(ourTexture, texCoord));
	return diffuse + specular + ambient;
}

vec3 point_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(light_position - frag_position);
	float diff = max(dot(normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), 32.f);
	float distance = length(light_position-frag_position);
	float attenuation = 1.0f/(k_constant + k_linear*distance + k_quadratic*(distance*distance));
	vec3 ambient = vec3(texture(ourTexture, texCoord));
	vec3 diffuse = diff * vec3(texture(ourTexture, texCoord));
	vec3 specular = spec * vec3(texture(ourTexture, texCoord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	return vec3(1.0);	
}