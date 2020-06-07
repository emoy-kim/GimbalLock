#version 460

#define MAX_LIGHTS 32

struct LightInfo
{
   int LightSwitch;
   vec4 Position;
   vec4 AmbientColor;
   vec4 DiffuseColor;
   vec4 SpecularColor;
   vec3 SpotlightDirection;
   float SpotlightExponent;
   float SpotlightCutoffAngle;
   vec3 AttenuationFactors;
};
uniform LightInfo Lights[MAX_LIGHTS];

struct MateralInfo {
   vec4 EmissionColor;
   vec4 AmbientColor;
   vec4 DiffuseColor;
   vec4 SpecularColor;
   float SpecularExponent;
};
uniform MateralInfo Material;

layout (binding = 0) uniform sampler2D BaseTexture;
uniform int UseTexture;

uniform int UseLight;
uniform int LightNum;
uniform vec4 GlobalAmbient;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 position_in_ec;
in vec3 normal_in_ec;
in vec2 tex_coord;

layout (location = 0) out vec4 final_color;

const float zero = 0.0f;
const float one = 1.0f;

bool IsPointLight(in vec4 light_position)
{
   return light_position.w != zero;
}

float getAttenuation(in vec3 light_vector, in int light_index)
{
   vec3 distance_scale;
   distance_scale.x = one;
   distance_scale.z = dot( light_vector, light_vector );
   distance_scale.y = sqrt( distance_scale.z );
   return one / dot( distance_scale, Lights[light_index].AttenuationFactors );
}

float getSpotlightFactor(in vec3 normalized_light_vector, in int light_index)
{
   if (Lights[light_index].SpotlightCutoffAngle >= 180.0f) return one;

   vec4 direction_in_ec = 
      transpose( inverse( ViewMatrix * WorldMatrix ) ) * 
      vec4(Lights[light_index].SpotlightDirection, 1.0f);
   vec3 normalized_direction = normalize( direction_in_ec.xyz );
   float cutoff_angle = clamp( Lights[light_index].SpotlightCutoffAngle, zero, 90.0f );
   float factor = dot( -normalized_light_vector, normalized_direction );
   return factor >= cos( radians( cutoff_angle ) ) ? pow( factor, Lights[light_index].SpotlightExponent ) : zero;
}

vec4 calculateLightingEquation()
{
   vec4 color = Material.EmissionColor + GlobalAmbient * Material.AmbientColor;

   for (int i = 0; i < LightNum; ++i) {
      if (Lights[i].LightSwitch == 0) continue;
      
      vec4 light_position_in_ec = ViewMatrix * Lights[i].Position;
      
      float final_effect_factor = one;
      vec3 light_vector = light_position_in_ec.xyz - position_in_ec;
      if (IsPointLight( light_position_in_ec )) {
         float attenuation = getAttenuation( light_vector, i );

         light_vector = normalize( light_vector );
         float spotlight_factor = getSpotlightFactor( light_vector, i );
         final_effect_factor = attenuation * spotlight_factor;
      }
      else light_vector = normalize( light_position_in_ec.xyz );
   
      if (final_effect_factor <= zero) continue;

      vec4 local_color = Lights[i].AmbientColor * Material.AmbientColor;

      float diffuse_intensity = max( dot( normal_in_ec, light_vector ), zero );
      local_color += diffuse_intensity * Lights[i].DiffuseColor * Material.DiffuseColor;

      vec3 halfway_vector = normalize( light_vector - normalize( position_in_ec ) );
      float specular_intensity = max( dot( normal_in_ec, halfway_vector ), zero );
      local_color += 
         pow( specular_intensity, Material.SpecularExponent ) * 
         Lights[i].SpecularColor * Material.SpecularColor;

      color += local_color;
   }
   return color;
}

void main()
{
   if (UseTexture == 0) final_color = vec4(one);
   else final_color = texture( BaseTexture, tex_coord );

   if (UseLight != 0) {
      final_color *= calculateLightingEquation();
   }
   else final_color *= Material.DiffuseColor;
}