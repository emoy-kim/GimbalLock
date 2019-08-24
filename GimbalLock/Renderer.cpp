#include "Renderer.h"

RendererGL* RendererGL::Renderer = nullptr;
RendererGL::RendererGL() : 
   Window( nullptr ), ClickedPoint( -1, -1 ), CapturedFrameIndex( 0 ), CapturedEulerAngles( 5 ), 
   CapturedQuaternions( 5 ), EulerAngle( 0.0f, 0.0f, 0.0f )
{
   Renderer = this;

   initialize();
   printOpenGLInformation();
}

RendererGL::~RendererGL()
{
   glfwTerminate();
}

void RendererGL::printOpenGLInformation() const
{
   cout << "****************************************************************" << endl;
   cout << " - GLFW version supported: " << glfwGetVersionString() << endl;
   cout << " - GLEW version supported: " << glewGetString( GLEW_VERSION ) << endl;
   cout << " - OpenGL renderer: " << glGetString( GL_RENDERER ) << endl;
   cout << " - OpenGL version supported: " << glGetString( GL_VERSION ) << endl;
   cout << " - OpenGL shader version supported: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << endl  ;
   cout << "****************************************************************" << endl << endl;
}

void RendererGL::initialize()
{
   if (!glfwInit()) {
      cout << "Cannot Initialize OpenGL..." << endl;
      return;
   }
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
   glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   const int width = 1920;
   const int height = 1080;
   Window = glfwCreateWindow( width, height, "Euler Angle (Blue) VS. Quaternion (Red)", nullptr, nullptr );
   glfwMakeContextCurrent( Window );
   glewInit();
   
   registerCallbacks();
   
   glEnable( GL_DEPTH_TEST );
   glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

   MainCamera.updateWindowSize( width, height );
   ObjectShader.setShader(
      "Shaders/VertexShaderForObject.glsl",
      "Shaders/FragmentShaderForObject.glsl"
   );
}

void RendererGL::error(int error, const char* description) const
{
   puts( description );
}

void RendererGL::errorWrapper(int error, const char* description)
{
   Renderer->error( error, description );
}

void RendererGL::cleanup(GLFWwindow* window)
{
   glDeleteProgram( ObjectShader.ShaderProgram );
   glDeleteVertexArrays( 1, &AxisObject.ObjVAO );
   glDeleteVertexArrays( 1, &TeapotObject.ObjVAO );
   glDeleteBuffers( 1, &AxisObject.ObjVBO );
   glDeleteBuffers( 1, &TeapotObject.ObjVBO );

   glfwSetWindowShouldClose( window, GLFW_TRUE );
}

void RendererGL::cleanupWrapper(GLFWwindow* window)
{
   Renderer->cleanup( window );
}

void RendererGL::captureFrame()
{
   if (CapturedFrameIndex < 5) {
      CapturedEulerAngles[CapturedFrameIndex] = EulerAngle;
      CapturedQuaternions[CapturedFrameIndex] = toQuat( orientate3( EulerAngle ) );
      CapturedFrameIndex++;
   }
}

void RendererGL::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action != GLFW_PRESS) return;

   switch (key) {
      case GLFW_KEY_C:
         captureFrame();
         break;
      case GLFW_KEY_E:
         if (!Animator.AnimationMode && CapturedFrameIndex == CapturedEulerAngles.size()) {
            Animator.StartTiming = glfwGetTime() * 1000.0;
            Animator.AnimationMode = true;
         }
         break;
      case GLFW_KEY_R:
         CapturedFrameIndex = 0;
         CapturedEulerAngles.clear();
         CapturedQuaternions.clear();
         CapturedEulerAngles.resize( 5 );
         CapturedQuaternions.resize( 5 );
         break;
      case GLFW_KEY_L:
         Lights.toggleLightSwitch();
         cout << "Light Turned " << (Lights.isLightOn() ? "On!" : "Off!") << endl;
         break;
      case GLFW_KEY_P:
         cout << "Camera Position: " << 
            MainCamera.CamPos.x << ", " << MainCamera.CamPos.y << ", " << MainCamera.CamPos.z << endl;
         break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
         cleanupWrapper( window );
         break;
      default:
         return;
   }
}

void RendererGL::keyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   Renderer->keyboard( window, key, scancode, action, mods );
}

void RendererGL::cursor(GLFWwindow* window, double xpos, double ypos)
{
   if (MainCamera.getMovingState()) {
      const auto x = static_cast<int>(round( xpos ));
      const auto y = static_cast<int>(round( ypos ));
      const int dx = x - ClickedPoint.x;
      const int dy = y - ClickedPoint.y;

      if (glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS) {
         EulerAngle.y += static_cast<float>(dx) * 0.01f;
         if (EulerAngle.y >= 360.0f) EulerAngle.y -= 360.0f;
      }

      if (glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS) {
         EulerAngle.x += static_cast<float>(dy) * 0.01f;
         if (EulerAngle.x >= 360.0f) EulerAngle.x -= 360.0f;
      }

      ClickedPoint.x = x;
      ClickedPoint.y = y;
   }
}

void RendererGL::cursorWrapper(GLFWwindow* window, double xpos, double ypos)
{
   Renderer->cursor( window, xpos, ypos );
}

void RendererGL::mouse(GLFWwindow* window, int button, int action, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) {
      const bool moving_state = action == GLFW_PRESS;
      if (moving_state) {
         double x, y;
         glfwGetCursorPos( window, &x, &y );
         ClickedPoint.x = static_cast<int>(round( x ));
         ClickedPoint.y = static_cast<int>(round( y ));
      }
      MainCamera.setMovingState( moving_state );
   }
}

void RendererGL::mouseWrapper(GLFWwindow* window, int button, int action, int mods)
{
   Renderer->mouse( window, button, action, mods );
}

void RendererGL::mousewheel(GLFWwindow* window, double xoffset, double yoffset)
{
   if (yoffset >= 0.0) {
      MainCamera.zoomIn();
   }
   else {
      MainCamera.zoomOut();
   }
}

void RendererGL::mousewheelWrapper(GLFWwindow* window, double xoffset, double yoffset)
{
   Renderer->mousewheel( window, xoffset, yoffset );
}

void RendererGL::reshape(GLFWwindow* window, int width, int height)
{
   MainCamera.updateWindowSize( width, height );
   glViewport( 0, 0, width, height );
}

void RendererGL::reshapeWrapper(GLFWwindow* window, int width, int height)
{
   Renderer->reshape( window, width, height );
}

void RendererGL::registerCallbacks() const
{
   glfwSetErrorCallback( errorWrapper );
   glfwSetWindowCloseCallback( Window, cleanupWrapper );
   glfwSetKeyCallback( Window, keyboardWrapper );
   glfwSetCursorPosCallback( Window, cursorWrapper );
   glfwSetMouseButtonCallback( Window, mouseWrapper );
   glfwSetScrollCallback( Window, mousewheelWrapper );
   glfwSetFramebufferSizeCallback( Window, reshapeWrapper );
}

void RendererGL::setLights()
{  
   vec4 light_position(10.0f, 150.0f, 10.0f, 1.0f);
   vec4 ambient_color(0.9f, 0.9f, 0.9f, 1.0f);
   vec4 diffuse_color(0.9f, 0.9f, 0.9f, 1.0f);
   vec4 specular_color(0.9f, 0.9f, 0.9f, 1.0f);
   Lights.addLight( light_position, ambient_color, diffuse_color, specular_color );

   light_position = vec4(7.0f, 100.0f, 7.0f, 1.0f);
   ambient_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
   diffuse_color = vec4(0.0f, 0.47f, 0.75f, 1.0f);
   specular_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   vec3 spotlight_direction(0.0f, -1.0f, 0.0f);
   float spotlight_exponent = 128;
   float spotlight_cutoff_angle_in_degree = 7.0f;
   Lights.addLight( 
      light_position, 
      ambient_color, 
      diffuse_color, 
      specular_color,
      spotlight_direction,
      spotlight_exponent,
      spotlight_cutoff_angle_in_degree
   );  
}

void RendererGL::setAxisObject()
{
   if (AxisObject.ObjVAO != 0) {
      glDeleteVertexArrays( 1, &AxisObject.ObjVAO );
      glDeleteBuffers( 1, &AxisObject.ObjVBO );
   }

   const vector<vec3> axis_vertices = {
      { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } 
   };
   AxisObject.setObject( GL_LINES, axis_vertices );
}

void RendererGL::setTeapotObject()
{
   if (TeapotObject.ObjVAO != 0) {
      glDeleteVertexArrays( 1, &TeapotObject.ObjVAO );
      glDeleteBuffers( 1, &TeapotObject.ObjVBO );
   }

   vector<vec3> teapot_vertices, teapot_normals;
   vector<vec2> teapot_textures;
   TeapotObject.readObjectFile( teapot_vertices, teapot_normals, teapot_textures, "Object/teapot.obj" );
   
   TeapotObject.setObject( GL_TRIANGLES, teapot_vertices, teapot_normals );
}

void RendererGL::drawAxisObject(const float& scale_factor)
{
   const bool origin_light_status = Lights.isLightOn();
   if (origin_light_status) Lights.toggleLightSwitch();
   
   glUseProgram( ObjectShader.ShaderProgram );
   glLineWidth( 5.0f );

   mat4 to_world = mat4(1.0f);
   const mat4 scale_matrix = scale( mat4(1.0f), vec3(scale_factor) );
   mat4 model_view_projection = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * scale_matrix * to_world;
   glUniformMatrix4fv( ObjectShader.Location.World, 1, GL_FALSE, &to_world[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.View, 1, GL_FALSE, &MainCamera.ViewMatrix[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.Projection, 1, GL_FALSE, &MainCamera.ProjectionMatrix[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );
   AxisObject.setDiffuseReflectionColor( { 1.0f, 0.0f, 0.0f, 1.0f } ); 
   AxisObject.transferUniformsToShader( ObjectShader );
   Lights.transferUniformsToShader( ObjectShader );

   glBindVertexArray( AxisObject.ObjVAO );
   glDrawArrays( AxisObject.DrawMode, 0, AxisObject.VerticesCount );

   to_world = glm::rotate( mat4(1.0f), radians( 90.0f ), vec3(0.0f, 0.0f, 1.0f) );
   model_view_projection = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * scale_matrix * to_world;
   glUniformMatrix4fv( ObjectShader.Location.World, 1, GL_FALSE, &to_world[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );
   AxisObject.setDiffuseReflectionColor( { 0.0f, 1.0f, 0.0f, 1.0f } ); 
   AxisObject.transferUniformsToShader( ObjectShader );

   glDrawArrays( AxisObject.DrawMode, 0, AxisObject.VerticesCount );

   to_world = glm::rotate( mat4(1.0f), radians( -90.0f ), vec3(0.0f, 1.0f, 0.0f) );
   model_view_projection = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * scale_matrix * to_world;
   glUniformMatrix4fv( ObjectShader.Location.World, 1, GL_FALSE, &to_world[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );
   AxisObject.setDiffuseReflectionColor( { 0.0f, 0.0f, 1.0f, 1.0f } ); 
   AxisObject.transferUniformsToShader( ObjectShader );

   glDrawArrays( AxisObject.DrawMode, 0, AxisObject.VerticesCount );
   glLineWidth( 1.0f );

   if (origin_light_status) Lights.toggleLightSwitch();
}

void RendererGL::drawTeapotObject(const mat4& to_world)
{
   glUseProgram( ObjectShader.ShaderProgram );

   const mat4 model_view_projection = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * to_world;
   glUniformMatrix4fv( ObjectShader.Location.World, 1, GL_FALSE, &to_world[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.View, 1, GL_FALSE, &MainCamera.ViewMatrix[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.Projection, 1, GL_FALSE, &MainCamera.ProjectionMatrix[0][0] );
   glUniformMatrix4fv( ObjectShader.Location.ModelViewProjection, 1, GL_FALSE, &model_view_projection[0][0] );

   TeapotObject.transferUniformsToShader( ObjectShader );
   Lights.transferUniformsToShader( ObjectShader );
   
   glBindVertexArray( TeapotObject.ObjVAO );
   glDrawArrays( TeapotObject.DrawMode, 0, TeapotObject.VerticesCount );
}

void RendererGL::displayEulerAngleMode()
{
   glViewport( 0, 216, 980, 864 );
   drawAxisObject( 15.0f );

   if (Animator.AnimationMode) {
      const uint curr = Animator.CurrentFrameIndex;
      const uint next = (Animator.CurrentFrameIndex + 1) % CapturedEulerAngles.size();
      const auto t = static_cast<float>(Animator.ElapsedTime / Animator.TimePerSection - curr);
      EulerAngle = (1 - t) * CapturedEulerAngles[curr] + t * CapturedEulerAngles[next];
   }
   const mat4 to_world = orientate4( EulerAngle );

   TeapotObject.setDiffuseReflectionColor( { 0.0f, 0.47f, 0.75f, 1.0f } );
   drawTeapotObject( to_world );
}

void RendererGL::displayQuaternionMode()
{
   glViewport( 980, 216, 980, 864 );
   drawAxisObject( 15.0f );

   mat4 to_world;
   if (Animator.AnimationMode) {
      const uint curr = Animator.CurrentFrameIndex;
      const uint next = (Animator.CurrentFrameIndex + 1) % CapturedQuaternions.size();
      const auto t = static_cast<float>(Animator.ElapsedTime / Animator.TimePerSection - curr);
      to_world = toMat4( slerp( CapturedQuaternions[curr], CapturedQuaternions[next], t ) );
   }
   else to_world = orientate4( EulerAngle );

   TeapotObject.setDiffuseReflectionColor( { 1.0f, 0.37f, 0.37f, 1.0f } );
   drawTeapotObject( to_world );
}

void RendererGL::displayCapturedFrames()
{
   for (uint i = 0; i < 5; ++i) {
      glViewport( 384 * i, 0, 384, 216 );
      drawAxisObject( 15.0f );

      if (i < CapturedFrameIndex) {
         if (Animator.AnimationMode && i == Animator.CurrentFrameIndex) {
            TeapotObject.setDiffuseReflectionColor( { 1.0f, 0.7f, 0.0f, 1.0f } );
         }
         else TeapotObject.setDiffuseReflectionColor( { 0.7f, 0.7f, 1.0f, 1.0f } );

         mat4 to_world = toMat4( CapturedQuaternions[i] );
         drawTeapotObject( to_world );
      }
   }
}

void RendererGL::render()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   displayEulerAngleMode();
   displayQuaternionMode();
   displayCapturedFrames();

   glBindVertexArray( 0 );
   glUseProgram( 0 );
}

void RendererGL::update()
{
   if (Animator.AnimationMode) {
      const double now = glfwGetTime() * 1000.0;
      Animator.ElapsedTime = now - Animator.StartTiming;
      if (Animator.ElapsedTime >= Animator.AnimationDuration) {
         Animator.StartTiming = now;
         Animator.ElapsedTime = 0.0;
      }

      Animator.CurrentFrameIndex = static_cast<int>(floor( Animator.ElapsedTime / Animator.TimePerSection ));
   }
}

void RendererGL::play()
{
   if (glfwWindowShouldClose( Window )) initialize();

   setLights();
   setAxisObject();
   setTeapotObject();
   ObjectShader.setUniformLocations( Lights.TotalLightNum );

   Animator.TimePerSection = Animator.AnimationDuration / CapturedEulerAngles.size();

   while (!glfwWindowShouldClose( Window )) {
      update();
      render();
      
      glfwSwapBuffers( Window );
      glfwPollEvents();
   }
   glfwDestroyWindow( Window );
}