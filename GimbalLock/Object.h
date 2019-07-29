/*
 * Author: Emoy Kim
 * E-mail: emoy.kim_AT_gmail.com
 * 
 * This code is a free software; it can be freely used, changed and redistributed.
 * If you use any version of the code, please reference the code.
 * 
 */

#pragma once

#include "Shader.h"

class LightGL
{
   bool TurnLightOn;

   vec4 GlobalAmbientColor;

   vector<bool> IsActivated;
   vector<vec4> Positions;
   
   vector<vec4> AmbientColors;
   vector<vec4> DiffuseColors;
   vector<vec4> SpecularColors;

   vector<vec3> SpotlightDirections;
   vector<float> SpotlightExponents;
   vector<float> SpotlightCutoffAngles;

   vector<vec3> AttenuationFactors;

public:
   uint TotalLightNum;

   LightGL();

   bool isLightOn() const;
   void toggleLightSwitch();

   void addLight(
      const vec4& light_position,
      const vec4& ambient_color = vec4(0.0f, 0.0f, 0.0f, 1.0f),
      const vec4& diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f),
      const vec4& specular_color = vec4(1.0f, 1.0f, 1.0f, 1.0f),
      const vec3& spotlight_direction = vec3(0.0f, 0.0f, -1.0f),
      const float& spotlight_exponent = 0.0f,
      const float& spotlight_cutoff_angle_in_degree = 180.0f,
      const vec3& attenuation_factor = vec3(1.0f, 0.0f, 0.0f)
   );

   void setGlobalAmbientColor(const vec4& global_ambient_color);
   void setAmbientColor(const vec4& ambient_color, const uint& light_index);
   void setDiffuseColor(const vec4& diffuse_color, const uint& light_index);
   void setSpecularColor(const vec4& specular_color, const uint& light_index);
   void setSpotlightDirection(const vec3& spotlight_direction, const uint& light_index);
   void setSpotlightExponent(const float& spotlight_exponent, const uint& light_index);
   void setSpotlightCutoffAngle(const float& spotlight_cutoff_angle_in_degree, const uint& light_index);
   void setAttenuationFactor(const vec3& attenuation_factor, const uint& light_index);
   void setLightPosition(const vec4& light_position, const uint& light_index);
   void activateLight(const uint& light_index);
   void deactivateLight(const uint& light_index);

   void transferUniformsToShader(ShaderGL& shader);
};


class ObjectGL
{
   vector<GLfloat> DataBuffer; // 3 for vertex, 3 for normal, and 2 for texture

   void prepareTexture2DFromMat(const Mat& texture) const;
   void prepareTexture(
      const int& n_bytes_per_vertex, 
      const Mat& texture, 
      const bool& normals_exist
   );
   
   void prepareTexture2DFromFile(const string& file_name) const;
   void prepareTexture(
      const int& n_bytes_per_vertex, 
      const string& texture_file_name, 
      const bool& normals_exist
   );

   void prepareVertexBuffer(const int& n_bytes_per_vertex);
   void prepareNormal(const int& n_bytes_per_vertex) const;
   GLvoid* bufferOffset(uint offset) const { return reinterpret_cast<GLvoid *>(offset); }

public:
   enum LayoutLocation { VertexLoc=0, NormalLoc, TextureLoc };

   GLuint ObjVAO, ObjVBO;
   vector<GLuint> ShaderStorageBufferObjects;
   GLenum DrawMode;
   GLuint TextureID;
   GLsizei VerticesCount;
   vec4 EmissionColor;
   vec4 AmbientReflectionColor; // It is usually set to the same color with DiffuseReflectionColor.
                                // Otherwise, it should be in balance with DiffuseReflectionColor.
   vec4 DiffuseReflectionColor; // the intrinsic color
   vec4 SpecularReflectionColor;
   float SpecularReflectionExponent;


   ObjectGL();

   void setEmissionColor(const vec4& emission_color);
   void setAmbientReflectionColor(const vec4& ambient_reflection_color);
   void setDiffuseReflectionColor(const vec4& diffuse_reflection_color);
   void setSpecularReflectionColor(const vec4& specular_reflection_color);
   void setSpecularReflectionExponent(const float& specular_reflection_exponent);

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices
   );

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices,
      const vector<vec3>& normals
   );

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices,
      const vector<vec2>& textures,
      const string& texture_file_name
   );

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices,
      const vector<vec2>& textures,
      const Mat& texture
   );

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices, 
      const vector<vec3>& normals, 
      const vector<vec2>& textures,
      const string& texture_file_name
   );

   void setObject(
      const GLenum& draw_mode, 
      const vector<vec3>& vertices, 
      const vector<vec3>& normals, 
      const vector<vec2>& textures,
      const Mat& texture
   );

   bool readObjectFile(vector<vec3>& vertices, vector<vec3>& normals, vector<vec2>& textures, const path& file_path) const;
   void transferUniformsToShader(ShaderGL& shader);
   void prepareShaderStorageBuffer();
   
   template<typename T>
   void addBufferObject(const GLenum& target, const vector<T>& data, const GLenum& usage)
   {
      GLuint Buffer;
      glGenBuffers( 1, &Buffer );
      glBindBuffer( target, Buffer );
      glBufferData( target, sizeof T * data.size(), data.data(), usage );
   }
};