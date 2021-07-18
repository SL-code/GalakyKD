cmake_minimum_required(VERSION 3.0)

project(GalaxyKD)


add_executable(GalaxyKD main.cpp)
target_link_libraries(GalaxyKD glfw)
target_link_libraries(GalaxyKD GL)
target_link_libraries(GalaxyKD GLEW)
#install(TARGETS learnopengl RUNTIME DESTINATION bin)
