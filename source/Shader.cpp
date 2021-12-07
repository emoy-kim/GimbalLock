#include "Shader.h"

ShaderGL::ShaderGL() : ShaderProgram( 0 )
{
}

ShaderGL::~ShaderGL()
{
   if (ShaderProgram != 0) glDeleteProgram( ShaderProgram );
}

void ShaderGL::readShaderFile(std::string& shader_contents, const char* shader_path)
{
   std::ifstream file( shader_path, std::ios::in );
   if (!file.is_open()) {
      std::cerr << "Cannot open shader file: " << shader_path << "\n";
      return;
   }

   std::string line;
   while (!file.eof()) {
      getline( file, line );
      shader_contents.append( line + "\n" );
   }
   file.close();
}

std::string ShaderGL::getShaderTypeString(GLenum shader_type)
{
   switch (shader_type) {
      case GL_VERTEX_SHADER: return "Vertex Shader";
      case GL_FRAGMENT_SHADER: return "Fragment Shader";
      default: return "";
   }
}

bool ShaderGL::checkCompileError(GLenum shader_type, const GLuint& shader)
{
   GLint compiled = 0;
   glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

   if (compiled == GL_FALSE) {
      GLint max_length = 0;
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &max_length );

      std::cerr << " ======= " << getShaderTypeString( shader_type ) << " log ======= \n";
      std::vector<GLchar> error_log(max_length);
      glGetShaderInfoLog( shader, max_length, &max_length, &error_log[0] );
      for (const auto& c : error_log) std::cerr << c;
      std::cerr << "\n";
      glDeleteShader( shader );
   }
   return compiled == GL_TRUE;
}

GLuint ShaderGL::getCompiledShader(GLenum shader_type, const char* shader_path)
{
   if (shader_path == nullptr) return 0;

   std::string shader_contents;
   readShaderFile( shader_contents, shader_path );

   const GLuint shader = glCreateShader( shader_type );
   const char* shader_source = shader_contents.c_str();
   glShaderSource( shader, 1, &shader_source, nullptr );
   glCompileShader( shader );
   if (!checkCompileError( shader_type, shader )) {
      std::cerr << "Could not compile shader\n";
      return 0;
   }
   return shader;
}

void ShaderGL::setShader(const char* vertex_shader_path, const char* fragment_shader_path)
{
   const GLuint vertex_shader = getCompiledShader( GL_VERTEX_SHADER, vertex_shader_path );
   const GLuint fragment_shader = getCompiledShader( GL_FRAGMENT_SHADER, fragment_shader_path );
   ShaderProgram = glCreateProgram();
   glAttachShader( ShaderProgram, vertex_shader );
   glAttachShader( ShaderProgram, fragment_shader );
   glLinkProgram( ShaderProgram );
   glDeleteShader( vertex_shader );
   glDeleteShader( fragment_shader );
}

void ShaderGL::setBasicTransformationUniforms()
{
   Location.World = glGetUniformLocation( ShaderProgram, "WorldMatrix" );
   Location.View = glGetUniformLocation( ShaderProgram, "ViewMatrix" );
   Location.Projection = glGetUniformLocation( ShaderProgram, "ProjectionMatrix" );
   Location.ModelViewProjection = glGetUniformLocation( ShaderProgram, "ModelViewProjectionMatrix" );
   Location.Color = glGetUniformLocation( ShaderProgram, "Color" );
}

void ShaderGL::transferBasicTransformationUniforms(const glm::mat4& to_world, const CameraGL* camera, const glm::vec4& color) const
{
   const glm::mat4 view = camera->getViewMatrix();
   const glm::mat4 projection = camera->getProjectionMatrix();
   const glm::mat4 model_view_projection = projection * view * to_world;
   glUniformMatrix4fv( Location.World, 1, GL_FALSE, &to_world[0][0] );
   glUniformMatrix4fv( Location.View, 1, GL_FALSE, &view[0][0] );
   glUniformMatrix4fv( Location.Projection, 1, GL_FALSE, &projection[0][0] );
   glUniformMatrix4fv( Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );
   glUniform4fv( Location.Color, 1, &color[0] );
}