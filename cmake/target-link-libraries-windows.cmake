target_link_libraries(GimbalLock glad glfw3dll)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
   target_link_libraries(GimbalLock FreeImaged)
else()
   target_link_libraries(GimbalLock FreeImage)
endif()