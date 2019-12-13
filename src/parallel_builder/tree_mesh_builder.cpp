/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree
 *          early elimination.
 *
 * @date    12 December 2019, 17:17
 **/


#include <cmath>
#include <limits>
#include <array>

#include "tree_mesh_builder.h"


TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize) :
	BaseMeshBuilder(gridEdgeSize, "Octree")
{
}


auto TreeMeshBuilder::marchCubes(const ParametricScalarField &field) -> unsigned
{
	unsigned trianglesCount = 0;

#pragma omp parallel default(none) shared(field, trianglesCount)
#pragma omp single nowait
	trianglesCount = decomposeSpace(mGridSize, Vec3_t<float>(), field);

	return trianglesCount;
}


auto TreeMeshBuilder::decomposeSpace(
	const unsigned gridSize,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> unsigned
{
	unsigned totalTrianglesCount = 0;
	const unsigned newGridSize = gridSize / 2;
	const auto edgeLength = float(newGridSize);
	std::array<const Vec3_t<float>, 8> newCubeOffsets{
		Vec3_t<float>(cubeOffset.x, cubeOffset.y, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x + edgeLength, cubeOffset.y, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x, cubeOffset.y + edgeLength, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x, cubeOffset.y, cubeOffset.z + edgeLength),
		Vec3_t<float>(
			cubeOffset.x + edgeLength, cubeOffset.y + edgeLength, cubeOffset.z
		),
		Vec3_t<float>(
			cubeOffset.x + edgeLength, cubeOffset.y, cubeOffset.z + edgeLength
		),
		Vec3_t<float>(
			cubeOffset.x, cubeOffset.y + edgeLength, cubeOffset.z + edgeLength
		),
		Vec3_t<float>(
			cubeOffset.x + edgeLength,
			cubeOffset.y + edgeLength,
			cubeOffset.z + edgeLength
		),
	};

	const size_t size = newCubeOffsets.size();
	for (size_t i = 0; i < size; i++)
	{
		const Vec3_t<float> newCubeOffset = newCubeOffsets[i];

		float edgeLengthFoo = edgeLength * mGridResolution;
		const float halfEdgeLength = edgeLength / 2.F;
		const Vec3_t<float> midPoint(
			cubeOffset.x * mGridResolution + halfEdgeLength,
			cubeOffset.y * mGridResolution + halfEdgeLength,
			cubeOffset.z * mGridResolution + halfEdgeLength
		);
		static const float expr = sqrtf(3.F) / 2.F;

		if (evaluateFieldAt(midPoint, field) <= mIsoLevel + expr * edgeLengthFoo)
		{
			if (newGridSize <= CUT_OFF)
			{
				totalTrianglesCount += buildCube(newCubeOffset, field);
			}
			else
			{
#pragma omp task default(none) \
shared(newGridSize, newCubeOffset, field, totalTrianglesCount)
				{
					unsigned trianglesCount =
						decomposeSpace(newGridSize, newCubeOffset, field);

#pragma omp atomic update
					totalTrianglesCount += trianglesCount;
				}
			}
		}
	}

#pragma omp taskwait
	return totalTrianglesCount;
}


auto TreeMeshBuilder::isBlockEmpty(
	float edgeLength,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> bool
{
	edgeLength *= mGridResolution;
	const float halfEdgeLength = edgeLength / 2.F;
	const Vec3_t<float> midPoint(
		cubeOffset.x * mGridResolution + halfEdgeLength,
		cubeOffset.y * mGridResolution + halfEdgeLength,
		cubeOffset.z * mGridResolution + halfEdgeLength
	);
	static const float expr = sqrtf(3.F) / 2.F;

	return evaluateFieldAt(midPoint, field) > mIsoLevel + expr * edgeLength;
}


auto TreeMeshBuilder::evaluateFieldAt(
	const Vec3_t<float> &pos, const ParametricScalarField &field
) -> float
{
	float minDistanceSquared = std::numeric_limits<float>::max();

	const size_t size = field.getPoints().size();
	const Vec3_t<float> *points = field.getPoints().data();
	for (size_t i = 0; i < size; i++)
	{
		const Vec3_t<float> point = points[i];
		const float distanceSquared = (pos.x - point.x) * (pos.x - point.x)
			+ (pos.y - point.y) * (pos.y - point.y)
			+ (pos.z - point.z) * (pos.z - point.z);
		minDistanceSquared = std::min(minDistanceSquared, distanceSquared);
	}

	return sqrtf(minDistanceSquared);
}


void TreeMeshBuilder::emitTriangle(const Triangle_t &triangle)
{
#pragma omp critical(tree_emitTriangle)
	triangles.push_back(triangle);
}
