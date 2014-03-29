# @brief Add flags to enable vectorization for Eigen
# @note Credit due to Dr. John D. Yamakoski, Brian Wightman
function(common_add_vectorization_flags)
	message("common: Adding vectorization flags")
	add_definitions(-fopenmp -march=native -mtune=native)
endfunction(common_add_vectorization_flags)
