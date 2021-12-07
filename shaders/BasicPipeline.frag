#version 460

uniform vec4 Color;
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 position_in_ec;
in vec3 normal_in_ec;

layout (location = 0) out vec4 final_color;

const float zero = 0.0f;
const float one = 1.0f;

void main()
{
   vec4 light_position_in_ec = ViewMatrix * vec4(10.0f, 150.0f, 10.0f, 1.0f);
   vec3 light_vector = normalize( light_position_in_ec.xyz );
   float diffuse_intensity = max( dot( normal_in_ec, light_vector ), zero ) + 0.8f;
   final_color = diffuse_intensity * Color;
}