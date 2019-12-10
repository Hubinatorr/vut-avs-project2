CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

PROJECT(PMC CXX C)
SET(CMAKE_CXX_STANDARD 11)

IF(APPLE)
	SET(LLVM_PATH "/usr/local/Cellar/llvm/9.0.0_1")

	SET(CMAKE_C_COMPILER "${LLVM_PATH}/bin/clang")
	SET(CMAKE_CXX_COMPILER "${LLVM_PATH}/bin/clang++")

	SET(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
	SET(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")

	SET(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
	SET(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")

	SET(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
	SET(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
	SET(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
ENDIF()

FIND_PACKAGE(OpenMP REQUIRED)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

SET(SOURCE_FILES
	common/base_mesh_builder.h common/base_mesh_builder.cpp
	common/parametric_scalar_field.h common/parametric_scalar_field.cpp
	common/ref_mesh_builder.h common/ref_mesh_builder.cpp
	common/vector_helpers.h common/cxxopts.hpp

	parallel_builder/loop_mesh_builder.h parallel_builder/loop_mesh_builder.cpp
	parallel_builder/tree_mesh_builder.h parallel_builder/tree_mesh_builder.cpp

	main.cpp)

INCLUDE_DIRECTORIES(common)
INCLUDE_DIRECTORIES(parallel_builder)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
