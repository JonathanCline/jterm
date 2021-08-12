cmake_minimum_required(VERSION 3.8)






#
#	Project variables
#

set(p_name jterm)

set(p_version_major 0)
set(p_version_minor 0)
set(p_version_patch 1)

set(p_languages CXX)
set(p_description "Terminal terminal terminal.")

set(p_include_public "include")
set(p_include_private "source")

set(p_link_public )
set(p_link_private jclib)

set(p_compfeatures_public cxx_std_20)
set(p_compfeatures_private )

set(p_libsource "source/jterm.cpp")









#
#	Intermediate variable calculations
#

set(p_version "${p_version_major}.${p_version_minor}.${p_version_patch}")



#
#	Actual cmake begins here
#

project(${p_name}
	LANGUAGES ${p_languages}
	VERSION ${p_version}
	DESCRIPTION ${p_description})



add_library(${PROJECT_NAME} STATIC "${p_libsource}")


target_include_directories(${PROJECT_NAME}
	PUBLIC "${p_include_public}"
	PRIVATE "${p_include_private}")

target_link_libraries(${PROJECT_NAME}
	PUBLIC "${p_link_public}"
	PRIVATE "${p_link_private}")

target_compile_features(${PROJECT_NAME}
	PUBLIC ${p_compfeatures_public}
	PRIVATE ${p_compfeatures_private})


ADD_CMAKE_SUBDIRS_HERE()


