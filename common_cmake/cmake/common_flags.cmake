# @brief Add flags to enable vectorization for Eigen
# @author Eric Cousineau <eacousineau@gmail.com>
# @author Dr. John D. Yamakoski
# @author Brian Wightman

function(common_add_vectorization_flags)
	message("common: Adding vectorization flags")
	add_definitions(-fopenmp -march=native -mtune=native)
endfunction(common_add_vectorization_flags)
