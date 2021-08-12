##
##	 Code generation portion of jterm's build setup
##


#
#	Variables
#

set(p_name ${PROJECT_NAME}-gen)
set(p_sources "main.cpp")
set(p_cxx 20)




#
#	CMake starts here
#

add_executable(${p_name} "${p_sources}")
target_compile_features(${p_name} PUBLIC cxx_std_${p_cxx})
