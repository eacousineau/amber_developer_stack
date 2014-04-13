# Matlab MEX Toolchain by Eric Cousineau <eacousineau@gmail.com>
 
# Used cmake/mex.cmake from Edward T. Kaszubski as starting point
# http://robotics.usc.edu/~ekaszubski/projects/matlab/

# Got proper toolchain stuff working:
	# Reference: https://github.com/aldebaran/qibuild/blob/master/cmake/contrib/toolchain/toolchain-iphone.cmake

# cmake_minimum_required(VERSION 2.6.3)

set(MATLAB_DIR $ENV{MATLABROOT})
set(MATLAB_ARCH $ENV{MATLAB_ARCH})

set(MEX_LFLAGS)
set(MEX_CLFAGS)
set(MEX_32 0)
set(MEX_GCC_VERSION 4.6) ## CHANGE

if(MATLAB_ARCH STREQUAL glnx86)
    set(MEX_EXT glx)
    set(MEX_32 1)
elseif(MATLAB_ARCH STREQUAL glnxa64)
    set(MEX_EXT a64)
endif(MATLAB_ARCH STREQUAL glnx86)

if(MEX_32)
    set(MEX_LFLAGS "-m32")
    set(MEX_CFLAGS "-m32")
endif(MEX_32)

set(MEX_LIBS mx mex mat m)
set(MEX_ENGINE_LIBS eng)
set(MEX_LIB_DIR ${MATLAB_DIR}/bin/${MATLAB_ARCH})
set(MEX_INCLUDE ${MATLAB_DIR}/extern/include)

# These are specifically for Linux 64-bit, Matlab 32-bit and 64-bit
# Have to use quotes for arguments when setting the CMake variables.
#set(CMAKE_SYSTEM_NAME Linux)
set(MEX_CC "gcc-${MEX_GCC_VERSION}")
set(MEX_CXX "g++-${MEX_GCC_VERSION}")
# Separate actual MEX linker flags because otherwise it will conflict with normal shared library symbol stuff
# Can see the difference using `objdump -tC ${lib}` and `objdump -TC ${lib}` (-t is normal symbols, -T is for dynamic)
set(MEX_LINUX_LFLAGS "-Wl,--version-script,${MATLAB_DIR}/extern/lib/${MATLAB_ARCH}/mexFunction.map")
# -ansi flag conflicts with -std=c++0x. Whoops.
set(MEX_LINUX_CFLAGS "-DAMBER_EIGEN_ALIGN -DMATLAB_MEX_FILE -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -DMX_COMPAT_32") # -ansi -Wl,--no-undefined")

# Propogating
set(IS_MEX 1)


if(NOT MEX_NOT_TOOLCHAIN)	
	include(CMakeForceCompiler)

	CMAKE_FORCE_C_COMPILER("${MEX_CC}" GNU)  
	CMAKE_FORCE_CXX_COMPILER("${MEX_CXX}" GNU)
endif(NOT MEX_NOT_TOOLCHAIN)

set(_set_flags "CACHE;INTERNAL;'';FORCE")

set(CMAKE_C_FLAGS "${MEX_CFLAGS} ${MEX_LINUX_CFLAGS}" ${_set_flags})
# set(CMAKE_CXX_FLAGS "${MEX_CFLAGS} ${MEX_LINUX_CFLAGS}" ${_set_flags})

set(CMAKE_EXE_LINKER_FLAGS "${MEX_LFLAGS}" ${_set_flags})
set(CMAKE_SHARED_LINKER_FLAGS "${MEX_LFLAGS}" ${_set_flags})
#set(CMAKE_MODULE_LINKER_FLAGS "${MEX_LFLAGS}" ${_set_flags})

include_directories(${MEX_INCLUDE})
link_directories(${MEX_LIB_DIR})

if(MEX_32)
	link_directories($ENV{INSTALL_PREFIX_32}/lib)
endif(MEX_32)

macro(target_link_mex lib)
	target_link_libraries(${lib} ${MEX_LIBS})
endmacro(target_link_mex)

macro(target_link_mex_engine lib)
	target_link_libraries(${lib} ${MEX_LIBS} ${MEX_ENGINE_LIBS})
endmacro(target_link_mex_engine)

# Adjusted from Kaszubski's
macro(add_mex lib)
	if(ROSBUILD_init_called)
		rosbuild_add_library(${lib} SHARED ${ARGN})
	else(ROSBUILD_init_called)
		add_library(${lib} SHARED ${ARGN})
	endif(ROSBUILD_init_called)
	target_link_mex(${lib})
	get_property(target_file TARGET ${lib} PROPERTY LOCATION)

	# Wait... How will this work with catkin? How to get path?

	set(MEX_DIR ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_LIB_DESTINATION})
	set(${lib}_MEX ${lib}.mex${MEX_EXT})
	message("Mex file: ${MEX_DIR}/${${lib}_MEX}")
#	set(${lib}_MEX_PATH ${CMAKE_BINARY_DIR}/${${lib}_MEX})
	#set_target_properties(${lib} PROPERTIES LINK_FLAGS "${MEX_LINUX_LFLAGS}")
	add_custom_command(TARGET ${lib} POST_BUILD COMMAND
		${CMAKE_COMMAND} -E copy ${target_file} ${${lib}_MEX})
	if(MEX_DIR)
		add_custom_command(TARGET ${lib} POST_BUILD COMMAND
			${CMAKE_COMMAND} -E copy ${${lib}_MEX} ${MEX_DIR})
	endif(MEX_DIR)
endmacro(add_mex)

# @todo This won't work for 32-bit
list(APPEND CMAKE_LIBRARY_PATH /usr/lib/x86_64-linux-gnu)
