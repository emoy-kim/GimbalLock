#pragma once

#include "_Common.h"
#include "Camera.h"

class ShaderGL
{
public:
   struct LightLocationSet
   {
      GLint LightSwitch, LightPosition;
      GLint LightAmbient, LightDiffuse, LightSpecular, LightAttenuationFactors;
      GLint SpotlightDirection, SpotlightExponent, SpotlightCutoffAngle;

      LightLocationSet() : LightSwitch( 0 ), LightPosition( 0 ), LightAmbient( 0 ), LightDiffuse( 0 ),
      LightSpecular( 0 ), LightAttenuationFactors( 0 ), SpotlightDirection( 0 ), SpotlightExponent( 0 ),
      SpotlightCutoffAngle( 0 ) {}
   };

   struct LocationSet
   {
      GLint World, View, Projection, ModelViewProjection;
      GLint MaterialEmission, MaterialAmbient, MaterialDiffuse, MaterialSpecular, MaterialSpecularExponent;
      std::map<GLint, GLint> Texture; // <binding point, texture id>
      GLint UseTexture, UseLight, LightNum, GlobalAmbient;
      std::vector<LightLocationSet> Lights;

      LocationSet() : World( 0 ), View( 0 ), Projection( 0 ), ModelViewProjection( 0 ), MaterialEmission( 0 ),
      MaterialAmbient( 0 ), MaterialDiffuse( 0 ), MaterialSpecular( 0 ), MaterialSpecularExponent( 0 ),
      UseTexture( 0 ), UseLight( 0 ), LightNum( 0 ), GlobalAmbient( 0 ) {}
   };

   ShaderGL();
   virtual ~ShaderGL();

   void setShader(
      const char* vertex_shader_path,
      const char* fragment_shader_path,
      const char* geometry_shader_path = nullptr,
      const char* tessellation_control_shader_path = nullptr,
      const char* tessellation_evaluation_shader_path = nullptr
   );
   void setComputeShaders(const std::vector<const char*>& compute_shader_paths);
   void setUniformLocations(int light_num);
   void addUniformLocation(const std::string& name);
   void addUniformLocationToComputeShader(const std::string& name, int shader_index);
   void transferBasicTransformationUniforms(const glm::mat4& to_world, const CameraGL* camera, bool use_texture = false) const;
   [[nodiscard]] GLuint getShaderProgram() const { return ShaderProgram; }
   [[nodiscard]] GLint getLocation(const std::string& name) const { return CustomLocations.find( name )->second; }
   [[nodiscard]] GLint getMaterialEmissionLocation() const { return Location.MaterialEmission; }
   [[nodiscard]] GLint getMaterialAmbientLocation() const { return Location.MaterialAmbient; }
   [[nodiscard]] GLint getMaterialDiffuseLocation() const { return Location.MaterialDiffuse; }
   [[nodiscard]] GLint getMaterialSpecularLocation() const { return Location.MaterialSpecular; }
   [[nodiscard]] GLint getMaterialSpecularExponentLocation() const { return Location.MaterialSpecularExponent; }
   [[nodiscard]] GLint getLightAvailabilityLocation() const { return Location.UseLight; }
   [[nodiscard]] GLint getLightNumLocation() const { return Location.LightNum; }
   [[nodiscard]] GLint getGlobalAmbientLocation() const { return Location.GlobalAmbient; }
   [[nodiscard]] GLint getLightSwitchLocation(int light_index) const
   {
      return Location.Lights[light_index].LightSwitch;
   }
   [[nodiscard]] GLint getLightPositionLocation(int light_index) const
   {
      return Location.Lights[light_index].LightPosition;
   }
   [[nodiscard]] GLint getLightAmbientLocation(int light_index) const
   {
      return Location.Lights[light_index].LightAmbient;
   }
   [[nodiscard]] GLint getLightDiffuseLocation(int light_index) const
   {
      return Location.Lights[light_index].LightDiffuse;
   }
   [[nodiscard]] GLint getLightSpecularLocation(int light_index) const
   {
      return Location.Lights[light_index].LightSpecular;
   }
   [[nodiscard]] GLint getLightSpotlightDirectionLocation(int light_index) const
   {
      return Location.Lights[light_index].SpotlightDirection;
   }
   [[nodiscard]] GLint getLightSpotlightExponentLocation(int light_index) const
   {
      return Location.Lights[light_index].SpotlightExponent;
   }
   [[nodiscard]] GLint getLightSpotlightCutoffAngleLocation(int light_index) const
   {
      return Location.Lights[light_index].SpotlightCutoffAngle;
   }
   [[nodiscard]] GLint getLightAttenuationFactorsLocation(int light_index) const
   {
      return Location.Lights[light_index].LightAttenuationFactors;
   }

protected:
   GLuint ShaderProgram;
   LocationSet Location;
   std::unordered_map<std::string, GLint> CustomLocations;
   std::vector<GLuint> ComputeShaderPrograms;

   static void readShaderFile(std::string& shader_contents, const char* shader_path);
   [[nodiscard]] static std::string getShaderTypeString(GLenum shader_type);
   [[nodiscard]] static bool checkCompileError(GLenum shader_type, const GLuint& shader);
   [[nodiscard]] static GLuint getCompiledShader(GLenum shader_type, const char* shader_path);
   void setBasicTransformationUniforms();
};