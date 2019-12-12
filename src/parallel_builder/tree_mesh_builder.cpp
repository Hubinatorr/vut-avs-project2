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
	return decomposeSpace(mGridResolution, Vec3_t<float>(), field);
}


auto TreeMeshBuilder::decomposeSpace(
	const float edgeLength,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> unsigned
{
	unsigned trianglesCount = 0;
	const float newEdgeLength = edgeLength / 2.F;
	std::array<const Vec3_t<float>, 8> newCubeOffsets{
		Vec3_t<float>(cubeOffset.x, cubeOffset.y, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x + newEdgeLength, cubeOffset.y, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x, cubeOffset.y + newEdgeLength, cubeOffset.z),
		Vec3_t<float>(cubeOffset.x, cubeOffset.y, cubeOffset.z + newEdgeLength),
		Vec3_t<float>(
			cubeOffset.x + newEdgeLength,
			cubeOffset.y + newEdgeLength,
			cubeOffset.z
		),
		Vec3_t<float>(
			cubeOffset.x + newEdgeLength,
			cubeOffset.y,
			cubeOffset.z + newEdgeLength
		),
		Vec3_t<float>(
			cubeOffset.x,
			cubeOffset.y + newEdgeLength,
			cubeOffset.z + newEdgeLength
		),
		Vec3_t<float>(
			cubeOffset.x + newEdgeLength,
			cubeOffset.y + newEdgeLength,
			cubeOffset.z + newEdgeLength
		),
	};

	for (const Vec3_t<float> newCubeOffset : newCubeOffsets)
	{
		if (!isBlockEmpty(newEdgeLength, newCubeOffset, field))
		{
			if (newEdgeLength < CUT_OFF)
			{
				trianglesCount += buildCube(newCubeOffset, field);
			}
			else
			{
				trianglesCount +=
					decomposeSpace(newEdgeLength, newCubeOffset, field);
			}
		}
	}

	return trianglesCount;
}


auto TreeMeshBuilder::isBlockEmpty(
	const float edgeLength,
	const Vec3_t<float> &cubeOffset,
	const ParametricScalarField &field
) -> bool
{
	const float halfEdgeLength = edgeLength / 2.F;
	const Vec3_t<float> middlePoint(
		cubeOffset.x + halfEdgeLength,
		cubeOffset.y + halfEdgeLength,
		cubeOffset.z + halfEdgeLength
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
