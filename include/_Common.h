#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <common.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>

#include <FreeImage.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <chrono>

#include "ProjectPath.h"

using uchar = unsigned char;
using uint = unsigned int;

constexpr uint OPENGL_COLOR_BUFFER_BIT = 0x00004000u;
constexpr uint OPENGL_DEPTH_BUFFER_BIT = 0x00000100u;
constexpr uint OPENGL_STENCIL_BUFFER_BIT = 0x00000400u;