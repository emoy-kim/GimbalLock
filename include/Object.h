#pragma once

#include "Shader.h"

class ObjectGL
{
public:
   enum LayoutLocation { VertexLoc = 0, NormalLoc, TextureLoc };

   ObjectGL();
   ~ObjectGL();

   void setDiffuseReflectionColor(const glm::vec4& diffuse_reflection_color);
   void setObject(GLenum draw_mode, const std::vector<glm::vec3>& vertices);
   void setObject(
      GLenum draw_mode,
      const std::vector<glm::vec3>& vertices,
      const std::vector<glm::vec3>& normals
   );
   void setObject(
      GLenum draw_mode,
      const std::vector<glm::vec3>& vertices,
      const std::vector<glm::vec2>& textures,
      const std::string& texture_file_path,
      bool is_grayscale = false
   );
   void setObject(
      GLenum draw_mode,
      const std::vector<glm::vec3>& vertices,
      const std::vector<glm::vec3>& normals,
      const std::vector<glm::vec2>& textures
   );
   void setObject(
      GLenum draw_mode,
      const std::vector<glm::vec3>& vertices,
      const std::vector<glm::vec3>& normals,
      const std::vector<glm::vec2>& textures,
      const std::string& texture_file_path,
      bool is_grayscale = false
   );
   void setSquareObject(GLenum draw_mode, bool use_texture = true);
   void setSquareObject(
      GLenum draw_mode,
      const std::string& texture_file_path,
      bool is_grayscale = false
   );
   int addTexture(const std::string& texture_file_path, bool is_grayscale = false);
   void addTexture(int width, int height, bool is_grayscale = false);
   int addTexture(const uint8_t* image_buffer, int width, int height, bool is_grayscale = false);
   void updateDataBuffer(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals);
   void updateDataBuffer(
      const std::vector<glm::vec3>& vertices,
      const std::vector<glm::vec3>& normals,
      const std::vector<glm::vec2>& textures
   );
   void replaceVertices(const std::vector<glm::vec3>& vertices, bool normals_exist, bool textures_exist);
   void replaceVertices(const std::vector<float>& vertices, bool normals_exist, bool textures_exist);
   bool readObjectFile(
      std::vector<glm::vec3>& vertices, 
      std::vector<glm::vec3>& normals, 
      std::vector<glm::vec2>& textures, 
      const std::string& file_path
   ) const;
   [[nodiscard]] GLuint getVAO() const { return VAO; }
   [[nodiscard]] GLenum getDrawMode() const { return DrawMode; }
   [[nodiscard]] GLsizei getVertexNum() const { return VerticesCount; }
   [[nodiscard]] glm::vec4 getColor() const { return DiffuseReflectionColor; }

private:
   uint8_t* ImageBuffer;
   std::vector<GLfloat> DataBuffer;
   GLuint VAO;
   GLuint VBO;
   GLenum DrawMode;
   std::vector<GLuint> TextureID;
   std::map<std::string, GLuint> CustomBuffers;
   GLsizei VerticesCount;
   glm::vec4 DiffuseReflectionColor;

   [[nodiscard]] bool prepareTexture2DUsingFreeImage(const std::string& file_path, bool is_grayscale) const;
   void prepareTexture(bool normals_exist) const;
   void prepareVertexBuffer(int n_bytes_per_vertex);
   void prepareNormal() const;
   static void getSquareObject(
      std::vector<glm::vec3>& vertices,
      std::vector<glm::vec3>& normals,
      std::vector<glm::vec2>& textures
   );
};