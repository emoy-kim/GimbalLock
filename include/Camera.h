#pragma once

#include "_Common.h"

class CameraGL
{
public:
   CameraGL();
   CameraGL(
      const glm::vec3& cam_position,
      const glm::vec3& view_reference_position,
      const glm::vec3& view_up_vector,
      float fov = 30.0f,
      float near_plane = 0.1f,
      float far_plane = 10000.0f
   );

   [[nodiscard]] bool getMovingState() const { return IsMoving; }
   [[nodiscard]] glm::vec3 getCameraPosition() const { return CamPos; }
   [[nodiscard]] const glm::mat4& getViewMatrix() const { return ViewMatrix; }
   [[nodiscard]] const glm::mat4& getProjectionMatrix() const { return ProjectionMatrix; }
   void setMovingState(bool is_moving) { IsMoving = is_moving; }
   void updateCamera();
   void pitch(int angle);
   void yaw(int angle);
   void rotateAroundWorldY(int angle);
   void moveForward(int delta = 1);
   void moveBackward(int delta = 1);
   void moveLeft(int delta = 1);
   void moveRight(int delta = 1);
   void moveUp(int delta = 1);
   void moveDown(int delta = 1);
   void zoomIn();
   void zoomOut();
   void resetCamera();
   void updateWindowSize(int width, int height);

private:
   bool IsMoving;
   int Width;
   int Height;
   float FOV;
   float InitFOV;
   float NearPlane;
   float FarPlane;
   float AspectRatio;
   const float ZoomSensitivity;
   const float MoveSensitivity;
   const float RotationSensitivity;
   glm::vec3 InitCamPos;
   glm::vec3 InitRefPos;
   glm::vec3 InitUpVec;
   glm::vec3 CamPos;
   glm::mat4 ViewMatrix;
   glm::mat4 ProjectionMatrix;
};