# http://www.cmake.org/Wiki/CMake_Useful_Variables
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR})

macro(mex_init)
	# Include non-toolchain toolchain (for include directories, libraries, etc)
	# This way a toolchain is not explicitly needed
	include(toolchain-mex)
endmacro()
