#include "Renderer.h"

RendererGL::RendererGL() : 
   Window( nullptr ), FrameWidth( 1920 ), FrameHeight( 1080 ),
   ObjectShader( std::make_unique<ShaderGL>() ), AxisObject( std::make_unique<ObjectGL>() ),
   TeapotObject( std::make_unique<ObjectGL>() )
{
   ClickedPoint = { -1, -1 };
   MainCamera = std::make_unique<CameraGL>();
   EulerAngle = {};
   CapturedFrameIndex = 0;
   CapturedEulerAngles.resize( 5 );
   CapturedQuaternions.resize( 5 );
   Animator = std::make_unique<Animation>();

   initialize();
   printOpenGLInformation();
}

void RendererGL::printOpenGLInformation()
{
   std::cout << "****************************************************************\n";
   std::cout << " - GLFW version supported: " << glfwGetVersionString() << "\n";
   std::cout << " - OpenGL renderer: " << glGetString( GL_RENDERER ) << "\n";
   std::cout << " - OpenGL version supported: " << glGetString( GL_VERSION ) << "\n";
   std::cout << " - OpenGL shader version supported: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";
   std::cout << "****************************************************************\n\n";
}

void RendererGL::initialize()
{
   if (!glfwInit()) {
      std::cout << "Cannot Initialize OpenGL...\n";
      return;
   }
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
   glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   Window = glfwCreateWindow( FrameWidth, FrameHeight, "Main Camera", nullptr, nullptr );
   glfwSetWindowUserPointer( Window, this );
   glfwMakeContextCurrent( Window );

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
   }
   
   registerCallbacks();
   
   glEnable( GL_DEPTH_TEST );
   glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

   MainCamera->updateWindowSize( FrameWidth, FrameHeight );

   const std::string shader_directory_path = std::string(CMAKE_SOURCE_DIR) + "/shaders";
   ObjectShader->setShader(
      std::string(shader_directory_path + "/BasicPipeline.vert").c_str(),
      std::string(shader_directory_path + "/BasicPipeline.frag").c_str()
   );
}

void RendererGL::cleanup(GLFWwindow* window)
{
   auto renderer = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer( window ));
   glfwSetWindowShouldClose( renderer->Window, GLFW_TRUE );
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
      case GLFW_KEY_P:
         if (!Animator->AnimationMode && CapturedFrameIndex == static_cast<int>(CapturedEulerAngles.size())) {
            Animator->StartTiming = glfwGetTime() * 1000.0;
            Animator->AnimationMode = true;
         }
         break;
      case GLFW_KEY_R:
         CapturedFrameIndex = 0;
         CapturedEulerAngles.clear();
         CapturedQuaternions.clear();
         CapturedEulerAngles.resize( 5 );
         CapturedQuaternions.resize( 5 );
         break;
      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
         cleanup( window );
         break;
      default:
         return;
   }
}

void RendererGL::cursor(GLFWwindow* window, double xpos, double ypos)
{
   if (MainCamera->getMovingState()) {
      const auto x = static_cast<int>(round( xpos ));
      const auto y = static_cast<int>(round( ypos ));
      const int dx = x - ClickedPoint.x;
      const int dy = y - ClickedPoint.y;

      auto renderer = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer( window ));
      if (glfwGetMouseButton( renderer->Window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS) {
         EulerAngle.y += static_cast<float>(dx) * 0.01f;
         if (EulerAngle.y >= 360.0f) EulerAngle.y -= 360.0f;
      }

      if (glfwGetMouseButton( renderer->Window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS) {
         EulerAngle.x += static_cast<float>(dy) * 0.01f;
         if (EulerAngle.x >= 360.0f) EulerAngle.x -= 360.0f;
      }

      ClickedPoint.x = x;
      ClickedPoint.y = y;
   }
}

void RendererGL::mouse(GLFWwindow* window, int button, int action, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_LEFT) {
      const bool moving_state = action == GLFW_PRESS;
      if (moving_state) {
         double x, y;
         auto renderer = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer( window ));
         glfwGetCursorPos( renderer->Window, &x, &y );
         ClickedPoint.x = static_cast<int>(round( x ));
         ClickedPoint.y = static_cast<int>(round( y ));
      }
      MainCamera->setMovingState( moving_state );
   }
}

void RendererGL::reshape(GLFWwindow* window, int width, int height)
{
   MainCamera->updateWindowSize( width, height );
   glViewport( 0, 0, width, height );
}

void RendererGL::registerCallbacks() const
{
   glfwSetWindowCloseCallback( Window, cleanup );
   glfwSetKeyCallback( Window, keyboard );
   glfwSetCursorPosCallback( Window, cursor );
   glfwSetMouseButtonCallback( Window, mouse );
   glfwSetFramebufferSizeCallback( Window, reshape );
}

void RendererGL::setAxisObject() const
{
   const std::vector<glm::vec3> axis_vertices = {
      { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } 
   };
   AxisObject->setObject( GL_LINES, axis_vertices );
}

void RendererGL::setTeapotObject() const
{
   std::vector<glm::vec3> teapot_vertices, teapot_normals;
   std::vector<glm::vec2> teapot_textures;
   const std::string sample_directory_path = std::string(CMAKE_SOURCE_DIR) + "/samples";
   TeapotObject->readObjectFile( 
      teapot_vertices, 
      teapot_normals, 
      teapot_textures, 
      std::string(sample_directory_path + "/teapot.obj")
   );
   
   TeapotObject->setObject( GL_TRIANGLES, teapot_vertices, teapot_normals );
}

void RendererGL::drawAxisObject(float scale_factor) const
{
   glUseProgram( ObjectShader->getShaderProgram() );
   glLineWidth( 5.0f );

   const glm::mat4 scale_matrix = scale(glm::mat4(1.0f), glm::vec3(scale_factor) );
   glm::mat4 to_world = scale_matrix;
   ObjectShader->transferBasicTransformationUniforms( to_world, MainCamera.get(), { 1.0f, 0.0f, 0.0f, 1.0f } );

   glBindVertexArray( AxisObject->getVAO() );
   glDrawArrays( AxisObject->getDrawMode(), 0, AxisObject->getVertexNum() );

   to_world = scale_matrix * glm::rotate( glm::mat4(1.0f), glm::radians( 90.0f ), glm::vec3(0.0f, 0.0f, 1.0f) );
   ObjectShader->transferBasicTransformationUniforms( to_world, MainCamera.get(), { 0.0f, 1.0f, 0.0f, 1.0f } );
   glDrawArrays( AxisObject->getDrawMode(), 0, AxisObject->getVertexNum() );

   to_world = scale_matrix * glm::rotate( glm::mat4(1.0f), glm::radians( -90.0f ), glm::vec3(0.0f, 1.0f, 0.0f) );
   ObjectShader->transferBasicTransformationUniforms( to_world, MainCamera.get(), { 0.0f, 0.0f, 1.0f, 1.0f } );
   glDrawArrays( AxisObject->getDrawMode(), 0, AxisObject->getVertexNum() );

   glLineWidth( 1.0f );
}

void RendererGL::drawTeapotObject(const glm::mat4& to_world) const
{
   glUseProgram( ObjectShader->getShaderProgram() );
   ObjectShader->transferBasicTransformationUniforms( to_world, MainCamera.get(), TeapotObject->getColor() );
   glBindVertexArray( TeapotObject->getVAO() );
   glDrawArrays( TeapotObject->getDrawMode(), 0, TeapotObject->getVertexNum() );
}

void RendererGL::displayEulerAngleMode()
{
   glViewport( 0, 216, 980, 864 );
   drawAxisObject( 15.0f );

   if (Animator->AnimationMode) {
      const uint curr = Animator->CurrentFrameIndex;
      const uint next = (Animator->CurrentFrameIndex + 1) % CapturedEulerAngles.size();
      const auto t = static_cast<float>(Animator->ElapsedTime / Animator->TimePerSection - curr);
      EulerAngle = (1 - t) * CapturedEulerAngles[curr] + t * CapturedEulerAngles[next];
   }
   TeapotObject->setDiffuseReflectionColor( { 0.0f, 0.47f, 0.75f, 1.0f } );

   const glm::mat4 to_world = orientate4( EulerAngle );
   drawTeapotObject( to_world );
}

void RendererGL::displayQuaternionMode()
{
   glViewport( 980, 216, 980, 864 );
   drawAxisObject( 15.0f );

   glm::mat4 to_world;
   if (Animator->AnimationMode) {
      const uint curr = Animator->CurrentFrameIndex;
      const uint next = (Animator->CurrentFrameIndex + 1) % CapturedQuaternions.size();
      const auto t = static_cast<float>(Animator->ElapsedTime / Animator->TimePerSection - curr);
      to_world = toMat4( slerp( CapturedQuaternions[curr], CapturedQuaternions[next], t ) );
   }
   else to_world = orientate4( EulerAngle );

   TeapotObject->setDiffuseReflectionColor( { 1.0f, 0.37f, 0.37f, 1.0f } );
   drawTeapotObject( to_world );
}

void RendererGL::displayCapturedFrames()
{
   for (int i = 0; i < 5; ++i) {
      glViewport( 384 * i, 0, 384, 216 );
      drawAxisObject( 15.0f );

      if (i < CapturedFrameIndex) {
         if (Animator->AnimationMode && i == Animator->CurrentFrameIndex) {
            TeapotObject->setDiffuseReflectionColor( { 1.0f, 0.7f, 0.0f, 1.0f } );
         }
         else TeapotObject->setDiffuseReflectionColor( { 0.7f, 0.7f, 1.0f, 1.0f } );

         glm::mat4 to_world = toMat4( CapturedQuaternions[i] );
         drawTeapotObject( to_world );
      }
   }
}

void RendererGL::render()
{
   glClear( OPENGL_COLOR_BUFFER_BIT | OPENGL_DEPTH_BUFFER_BIT );

   displayEulerAngleMode();
   displayQuaternionMode();
   displayCapturedFrames();

   glBindVertexArray( 0 );
   glUseProgram( 0 );
}

void RendererGL::update()
{
   if (Animator->AnimationMode) {
      const double now = glfwGetTime() * 1000.0;
      Animator->ElapsedTime = now - Animator->StartTiming;
      if (Animator->ElapsedTime >= Animator->AnimationDuration) {
         Animator->StartTiming = now;
         Animator->ElapsedTime = 0.0;
      }
      Animator->CurrentFrameIndex = static_cast<int>(std::floor( Animator->ElapsedTime / Animator->TimePerSection ));
   }
}

void RendererGL::play()
{
   if (glfwWindowShouldClose( Window )) initialize();

   setAxisObject();
   setTeapotObject();
   ObjectShader->setBasicTransformationUniforms();

   Animator->TimePerSection = Animator->AnimationDuration / static_cast<double>(CapturedEulerAngles.size());
   while (!glfwWindowShouldClose( Window )) {
      update();
      render();
      glfwSwapBuffers( Window );
      glfwPollEvents();
   }
   glfwDestroyWindow( Window );
}