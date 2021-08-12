cmake_minimum_required (VERSION 3.8)

project ("Terminal")


include("CMakeUtils.cmake")

add_executable (Terminal "Terminal.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC jterm jclib)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

ADD_CMAKE_SUBDIRS_HERE()
