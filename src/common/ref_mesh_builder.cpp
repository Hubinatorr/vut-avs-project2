/**
 * @file    ref_mesh_builder.cpp
 *
 * @authors Filip Vaverka  <ivaverka@fit.vutbr.cz>
 *          Filip Kuklis   <ikuklis@fit.vutbr.cz>
 *          Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Reference (sequential) implementation of "BaseMeshBuilder".
 *
 * @date    11 December 2019, 02:12
 **/


#include <cmath>
#include <limits>

#include "ref_mesh_builder.h"


RefMeshBuilder::RefMeshBuilder(unsigned gridEdgeSize) : BaseMeshBuilder(gridEdgeSize, "Reference")
{
}


auto RefMeshBuilder::marchCubes(const ParametricScalarField &field) -> unsigned
{
	// 1. Compute total number of cubes in the grid.
	size_t totalCubesCount = mGridSize * mGridSize * mGridSize;

	unsigned totalTriangles = 0;

	// 2. Loop over each coordinate in the 3D grid.
	for (size_t i = 0; i < totalCubesCount; ++i)
	{
		// 3. Compute 3D position in the grid.
		Vec3_t<float> cubeOffset(
			i % mGridSize,
			(i / mGridSize) % mGridSize,
			i / (mGridSize * mGridSize)
		);

		// 4. Evaluate "Marching Cube" at given position in the grid and
		//    store the number of triangles generated.
		totalTriangles += buildCube(cubeOffset, field);
	}

	// 5. Return total number of triangles generated.
	return totalTriangles;
}


auto RefMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field) -> float
{
	// NOTE: This method is called from "buildCube(...)"!

	// 1. Store pointer to and number of 3D points in the field
	//    (to avoid "data()" and "size()" call in the loop).
	const Vec3_t<float> *pPoints = field.getPoints().data();
	const auto count = unsigned(field.getPoints().size());

	float value = std::numeric_limits<float>::max();

	// 2. Find minimum square distance from points "pos" to any point in the
	//    field.
	for (unsigned i = 0; i < count; ++i)
	{
		float distanceSquared = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
		distanceSquared += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
		distanceSquared += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

		// Comparing squares instead of real distance to avoid unnecessary
		// "sqrt"s in the loop.
		value = std::min(value, distanceSquared);
	}

	// 3. Finally, take square root of the minimal square distance to get the real distance.
	return sqrt(value);
}


void RefMeshBuilder::emitTriangle(const Triangle_t &triangle)
{
	// NOTE: This method is called from "buildCube(...)"!

	// Store generated triangle into vector (array) of generated triangles.
	// The pointer to data in this array is return by "getTrianglesArray(...)" call
	// after "marchCubes(...)" call ends.
	mTriangles.push_back(triangle);
}
