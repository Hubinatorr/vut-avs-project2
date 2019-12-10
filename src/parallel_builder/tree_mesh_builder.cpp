/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination.
 *
 * @date    11 December 2019, 00:02
 **/


#include <iostream>
#include <cmath>
#include <limits>

#include "tree_mesh_builder.h"


TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize) : BaseMeshBuilder(gridEdgeSize, "Octree")
{
}


auto TreeMeshBuilder::marchCubes(const ParametricScalarField &field) -> unsigned
{
	// Suggested approach to tackle this problem is to add new method to
	// this class. This method will call itself to process the children.
	// It is also strongly suggested to first implement Octree as sequential
	// code and only when that works add OpenMP tasks to achieve parallelism.

	return 0;
}


auto TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field) -> float
{
	return .0F;
}


void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
}
