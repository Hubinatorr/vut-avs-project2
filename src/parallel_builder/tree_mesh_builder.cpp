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
	return decomposeSpace(mGridSize, Vec3_t<float>(), field);
}


auto TreeMeshBuilder::decomposeSpace(
	const unsigned gridSize,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> unsigned
{
	unsigned trianglesCount = 0;
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

	for (const Vec3_t<float> newCubeOffset : newCubeOffsets)
	{
		if (!isBlockEmpty(edgeLength, newCubeOffset, field))
		{
			if (newGridSize <= CUT_OFF)
			{
				trianglesCount += buildCube(newCubeOffset, field);
			}
			else
			{
				trianglesCount +=
					decomposeSpace(newGridSize, newCubeOffset, field);
			}
		}
	}

	return trianglesCount;
}


auto TreeMeshBuilder::isBlockEmpty(
	float edgeLength,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> bool
{
	edgeLength *= mGridResolution;
	const float halfEdgeLength = edgeLength / 2.F;
	const Vec3_t<float> middlePoint(
		cubeOffset.x * mGridResolution + halfEdgeLength,
		cubeOffset.y * mGridResolution + halfEdgeLength,
		cubeOffset.z * mGridResolution + halfEdgeLength
	);
	static const float exp = sqrtf(3.F) / 2.F;

	return evaluateFieldAt(middlePoint, field) > mIsoLevel + exp * edgeLength;
}


auto TreeMeshBuilder::evaluateFieldAt(
	const Vec3_t<float> &pos, const ParametricScalarField &field
) -> float
{
	float minDistanceSquared = std::numeric_limits<float>::max();

	for (const Vec3_t<float> point : field.getPoints())
	{
		const float distanceSquared = (pos.x - point.x) * (pos.x - point.x)
			+ (pos.y - point.y) * (pos.y - point.y)
			+ (pos.z - point.z) * (pos.z - point.z);
		minDistanceSquared = std::min(minDistanceSquared, distanceSquared);
	}

	return sqrtf(minDistanceSquared);
}


void TreeMeshBuilder::emitTriangle(const Triangle_t &triangle)
{
	triangles.push_back(triangle);
}
