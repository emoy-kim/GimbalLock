/*
 * Author: Emoy Kim
 * E-mail: emoy.kim_AT_gmail.com
 * 
 * This code is a free software; it can be freely used, changed and redistributed.
 * If you use any version of the code, please reference the code.
 * 
 */

#pragma once

#include "_Common.h"

class ShaderGL
{
   void readShaderFile(string& shader_contents, const char* shader_path) const;
   bool checkCompileError(const GLuint& shader);

public:
   struct LightLocationSet
   {
      GLint LightSwitch, LightPosition;
      GLint LightAmbient, LightDiffuse, LightSpecular, LightAttenuationFactors;
      GLint SpotlightDirection, SpotlightExponent, SpotlightCutoffAngle;
      LightLocationSet() : LightSwitch( 0 ), LightPosition( 0 ), LightAmbient( 0 ), LightDiffuse( 0 ), LightSpecular( 0 ), 
      LightAttenuationFactors( 0 ), SpotlightDirection( 0 ), SpotlightExponent( 0 ), SpotlightCutoffAngle( 0 ) {}
   };

   struct LocationSet
   {
      GLint World, View, Projection, ModelViewProjection;
      GLint MaterialEmission, MaterialAmbient, MaterialDiffuse, MaterialSpecular, MaterialSpecularExponent;
      GLint TextureUnit, Texture;
      GLint UseLight, LightNum, GlobalAmbient;
      vector<LightLocationSet> Lights;
      LocationSet() : World( 0 ), View( 0 ), Projection( 0 ), ModelViewProjection( 0 ), MaterialEmission( 0 ),
      MaterialAmbient( 0 ), MaterialDiffuse( 0 ), MaterialSpecular( 0 ), MaterialSpecularExponent( 0 ), 
      TextureUnit( 0 ), Texture( 0 ), UseLight( 0 ), LightNum( 0 ), GlobalAmbient( 0 ) {}
   };
   
   LocationSet Location;
   GLuint ShaderProgram;

   unordered_map<string, GLint> CustomLocations;
   vector<GLuint> ComputeShaderPrograms;


   ShaderGL();

   void setShader(const char* vertex_shader_path, const char* fragment_shader_path);
   void setComputeShaders(const vector<const char*>& compute_shader_paths);
   void setUniformLocations(const uint& light_num);
   void addUniformLocation(const GLuint& program, const string& name);
};