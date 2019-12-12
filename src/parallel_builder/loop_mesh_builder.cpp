/**
 * @file    loop_mesh_builder.cpp
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops.
 *
 * @date    12 December 2019, 00:31
 **/


#include <cmath>
#include <limits>

#include "loop_mesh_builder.h"


LoopMeshBuilder::LoopMeshBuilder(unsigned gridEdgeSize) :
	BaseMeshBuilder(gridEdgeSize, "OpenMP Loop")
{
}


auto LoopMeshBuilder::marchCubes(const ParametricScalarField &field) -> unsigned
{
	size_t cubesCount = pow(mGridSize, 3);
	unsigned trianglesCount = 0;

//#pragma omp parallel for default(none) shared(cubesCount) \
//reduction(+:trianglesCount)
	for (size_t i = 0; i < cubesCount; i++)
	{
		Vec3_t<float> cubeOffset(
			i % mGridSize,
			(i / mGridSize) % mGridSize,
			i / (mGridSize * mGridSize)
		);
		trianglesCount += buildCube(cubeOffset, field);
	}

	return trianglesCount;
}


auto LoopMeshBuilder::evaluateFieldAt(
	const Vec3_t<float> &pos, const ParametricScalarField &field
) -> float
{
	const Vec3_t<float> *points = field.getPoints().data();
	const auto pointsCount = unsigned(field.getPoints().size());
	float minDistanceSquared = std::numeric_limits<float>::max();

//#pragma omp parallel for default(none) shared(pointsCount, points) \
//reduction(min:minDistanceSquared)
	for (unsigned i = 0; i < pointsCount; i++)
	{
		float distanceSquared = (pos.x - points[i].x) * (pos.x - points[i].x)
			+ (pos.y - points[i].y) * (pos.y - points[i].y)
			+ (pos.z - points[i].z) * (pos.z - points[i].z);
		minDistanceSquared = fminf(minDistanceSquared, distanceSquared);
	}

	return sqrtf(minDistanceSquared);
}


void LoopMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
//#pragma omp critical(loop_emitTriangle)
	triangles.push_back(triangle);
}
