/**
 * @file    loop_mesh_builder.cpp
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops.
 *
 * @date    10 December 2019, 23:55
 **/


#include <iostream>
#include <cmath>
#include <limits>

#include "loop_mesh_builder.h"


LoopMeshBuilder::LoopMeshBuilder(unsigned gridEdgeSize) : BaseMeshBuilder(gridEdgeSize, "OpenMP Loop")
{
}


auto LoopMeshBuilder::marchCubes(const ParametricScalarField &field) -> unsigned
{
	return 0;
}


auto LoopMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field) -> float
{
	return .0F;
}


void LoopMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
}
