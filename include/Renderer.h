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
#include "Object.h"

class RendererGL
{
public:
   RendererGL(const RendererGL&) = delete;
   RendererGL(const RendererGL&&) = delete;
   RendererGL& operator=(const RendererGL&) = delete;
   RendererGL& operator=(const RendererGL&&) = delete;


   RendererGL();
   ~RendererGL() = default;

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

   inline static glm::ivec2 ClickedPoint;
   inline static std::unique_ptr<CameraGL> MainCamera;
   inline static glm::vec3 EulerAngle;
   inline static int CapturedFrameIndex;
   inline static std::vector<glm::vec3> CapturedEulerAngles;
   inline static std::vector<glm::quat> CapturedQuaternions;
   inline static std::unique_ptr<Animation> Animator;

   GLFWwindow* Window;
   int FrameWidth;
   int FrameHeight;
   std::unique_ptr<ShaderGL> ObjectShader;
   std::unique_ptr<ObjectGL> AxisObject;
   std::unique_ptr<ObjectGL> TeapotObject;
 
   void registerCallbacks() const;
   void initialize();

   static void printOpenGLInformation();

   static void cleanup(GLFWwindow* window);
   static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
   static void cursor(GLFWwindow* window, double xpos, double ypos);
   static void mouse(GLFWwindow* window, int button, int action, int mods);
   static void reshape(GLFWwindow* window, int width, int height);

   static void captureFrame();

   void setAxisObject() const;
   void setTeapotObject() const;
   void drawAxisObject(float scale_factor = 1.0f) const;
   void drawTeapotObject(const glm::mat4& to_world) const;
   void displayEulerAngleMode();
   void displayQuaternionMode();
   void displayCapturedFrames();
   static void update();
   void render();
};