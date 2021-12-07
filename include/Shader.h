#pragma once

#include "_Common.h"
#include "Camera.h"

class ShaderGL
{
public:
   struct LocationSet
   {
      GLint World, View, Projection, ModelViewProjection, Color;

      LocationSet() : World( 0 ), View( 0 ), Projection( 0 ), ModelViewProjection( 0 ), Color( 0 ) {}
   };

   ShaderGL();
   virtual ~ShaderGL();

   void setShader(const char* vertex_shader_path, const char* fragment_shader_path);
   void setBasicTransformationUniforms();
   void transferBasicTransformationUniforms(const glm::mat4& to_world, const CameraGL* camera, const glm::vec4& color) const;
   [[nodiscard]] GLuint getShaderProgram() const { return ShaderProgram; }

protected:
   GLuint ShaderProgram;
   LocationSet Location;

   static void readShaderFile(std::string& shader_contents, const char* shader_path);
   [[nodiscard]] static std::string getShaderTypeString(GLenum shader_type);
   [[nodiscard]] static bool checkCompileError(GLenum shader_type, const GLuint& shader);
   [[nodiscard]] static GLuint getCompiledShader(GLenum shader_type, const char* shader_path);
};