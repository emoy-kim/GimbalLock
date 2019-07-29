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

class CameraGL
{
   const float ZoomSensitivity;
   const float MoveSensitivity;
   const float RotationSensitivity;
   bool IsMoving;
   float AspectRatio;
   float InitFOV;
   float NearPlane, FarPlane;
   vec3 InitCamPos, InitRefPos, InitUpVec;

public:
   float FOV;
   vec3 CamPos;
   mat4 ViewMatrix, ProjectionMatrix;

   CameraGL();
   CameraGL(
      const vec3& cam_position,
      const vec3& view_reference_position,
      const vec3& view_up_vector,
      float fov = 30.0f,
      float near_plane = 0.1f,
      float far_plane = 10000.0f
   );

   bool getMovingState() const;
   void setMovingState(const bool& is_moving);
   void updateCamera();
   void pitch(const int& angle);
   void yaw(const int& angle);
   void rotateAroundWorldY(const int& angle);
   void moveForward(const int& delta = 1);
   void moveBackward(const int& delta = 1);
   void moveLeft(const int& delta = 1);
   void moveRight(const int& delta = 1);
   void moveUp(const int& delta = 1);
   void moveDown(const int& delta = 1);
   void zoomIn();
   void zoomOut();
   void resetCamera();
   void updateWindowSize(const int& width, const int& height);
};