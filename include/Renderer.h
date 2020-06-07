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
#include "Light.h"
#include "Object.h"

class RendererGL
{
public:
   RendererGL(const RendererGL&) = delete;
   RendererGL(const RendererGL&&) = delete;
   RendererGL& operator=(const RendererGL&) = delete;
   RendererGL& operator=(const RendererGL&&) = delete;


   RendererGL();
   ~RendererGL();

   void play();

private:
   struct Animation
   {
      bool AnimationMode;
      double AnimationDuration;
      double TimePerSection;
      double StartTiming;
      double ElapsedTime;
      uint CurrentFrameIndex;
      Animation() : AnimationMode( false ), AnimationDuration( 10000.0 ), TimePerSection( 0.0 ),
      StartTiming( 0.0 ), ElapsedTime( 0.0 ), CurrentFrameIndex( 0 ) {}
   };

   inline static RendererGL* Renderer = nullptr;
   GLFWwindow* Window;
   int FrameWidth;
   int FrameHeight;
   int CapturedFrameIndex;
   std::vector<glm::vec3> CapturedEulerAngles;
   std::vector<glm::quat> CapturedQuaternions;
   glm::vec3 EulerAngle;
   Animation Animator;
   glm::ivec2 ClickedPoint;
   std::unique_ptr<CameraGL> MainCamera;
   std::unique_ptr<ShaderGL> ObjectShader;
   std::unique_ptr<ObjectGL> AxisObject;
   std::unique_ptr<ObjectGL> TeapotObject;
   std::unique_ptr<LightGL> Lights;
 
   void registerCallbacks() const;
   void initialize();

   static void printOpenGLInformation();

   void error(int error, const char* description) const;
   void cleanup(GLFWwindow* window);
   void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
   void cursor(GLFWwindow* window, double xpos, double ypos);
   void mouse(GLFWwindow* window, int button, int action, int mods);
   void mousewheel(GLFWwindow* window, double xoffset, double yoffset) const;
   void reshape(GLFWwindow* window, int width, int height) const;
   static void errorWrapper(int error, const char* description);
   static void cleanupWrapper(GLFWwindow* window);
   static void keyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
   static void cursorWrapper(GLFWwindow* window, double xpos, double ypos);
   static void mouseWrapper(GLFWwindow* window, int button, int action, int mods);
   static void mousewheelWrapper(GLFWwindow* window, double xoffset, double yoffset);
   static void reshapeWrapper(GLFWwindow* window, int width, int height);

   void captureFrame();

   void setLights() const;
   void setAxisObject() const;
   void setTeapotObject() const;
   void drawAxisObject(float scale_factor = 1.0f) const;
   void drawTeapotObject(const glm::mat4& to_world) const;
   void displayEulerAngleMode();
   void displayQuaternionMode();
   void displayCapturedFrames();
   void update();
   void render();
};