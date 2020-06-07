#include "Camera.h"

CameraGL::CameraGL() :
   CameraGL(
      glm::vec3(0.0f, 30.0f, 50.0f), 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(0.0f, 1.0f, 0.0f)
   )
{
}

CameraGL::CameraGL(
   const glm::vec3& cam_position,
   const glm::vec3& view_reference_position,
   const glm::vec3& view_up_vector,
   float fov,
   float near_plane,
   float far_plane
) : 
   IsMoving( false ), Width( 0 ), Height( 0 ), FOV( fov ), InitFOV( fov ), NearPlane( near_plane ), FarPlane( far_plane ),
   AspectRatio( 0.0f ), ZoomSensitivity( 1.0f ), MoveSensitivity( 0.05f ), RotationSensitivity( 0.005f ),  
   InitCamPos( cam_position ), InitRefPos( view_reference_position ), InitUpVec( view_up_vector ), CamPos( cam_position ),
   ViewMatrix( lookAt( InitCamPos, InitRefPos, InitUpVec ) ), ProjectionMatrix(glm::mat4(1.0f) )
{
}

void CameraGL::updateCamera()
{
   const glm::mat4 inverse_view = inverse( ViewMatrix );
   CamPos.x = inverse_view[3][0];
   CamPos.y = inverse_view[3][1];
   CamPos.z = inverse_view[3][2];
}

void CameraGL::pitch(int angle)
{
   const glm::vec3 u_axis(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
   ViewMatrix = glm::rotate( ViewMatrix, static_cast<float>(-angle) * RotationSensitivity, u_axis );
   updateCamera();
}

void CameraGL::yaw(int angle)
{
   const glm::vec3 v_axis(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
   ViewMatrix = glm::rotate( ViewMatrix, static_cast<float>(-angle) * RotationSensitivity, v_axis );
   updateCamera();
}

void CameraGL::rotateAroundWorldY(int angle)
{
   const glm::vec3 world_y(0.0f, 1.0f, 0.0f);
   ViewMatrix = glm::rotate(glm::mat4(1.0f), static_cast<float>(-angle) * RotationSensitivity, world_y ) * ViewMatrix;
   updateCamera();
}

void CameraGL::moveForward(int delta)
{
   const glm::vec3 n_axis(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) *  -n_axis );
   updateCamera();
}

void CameraGL::moveBackward(int delta)
{
   const glm::vec3 n_axis(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) * n_axis );
   updateCamera();
}

void CameraGL::moveLeft(int delta)
{
   const glm::vec3 u_axis(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) * -u_axis );
   updateCamera();
}

void CameraGL::moveRight(int delta)
{
   const glm::vec3 u_axis(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) * u_axis );
   updateCamera();
}

void CameraGL::moveUp(int delta)
{
   const glm::vec3 v_axis(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) * v_axis );
   updateCamera();
}

void CameraGL::moveDown(int delta)
{
   const glm::vec3 v_axis(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
   ViewMatrix = translate( ViewMatrix, MoveSensitivity * static_cast<float>(-delta) * -v_axis );
   updateCamera();
}

void CameraGL::zoomIn()
{
   if (FOV > 0.0f) {
      FOV -= ZoomSensitivity;
      ProjectionMatrix = glm::perspective( glm::radians( FOV ), AspectRatio, NearPlane, FarPlane );
   }
}

void CameraGL::zoomOut()
{
   if (FOV < 90.0f) {
      FOV += ZoomSensitivity;
      ProjectionMatrix = glm::perspective( glm::radians( FOV ), AspectRatio, NearPlane, FarPlane );
   }
}

void CameraGL::resetCamera()
{
   CamPos = InitCamPos; 
   ViewMatrix = lookAt( InitCamPos, InitRefPos, InitUpVec );
   ProjectionMatrix = glm::perspective( glm::radians( InitFOV ), AspectRatio, NearPlane, FarPlane );
}

void CameraGL::updateWindowSize(int width, int height)
{
   Width = width;
   Height = height;
   AspectRatio = static_cast<float>(width) / static_cast<float>(height);
   ProjectionMatrix = glm::perspective( glm::radians( FOV ), AspectRatio, NearPlane, FarPlane );
}