/**
 * @file    tree_mesh_builder.h
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree
 *          early elimination.
 *
 * @date    12 December 2019, 17:16
 **/


#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include <vector>

#include "base_mesh_builder.h"


class TreeMeshBuilder : public BaseMeshBuilder
{
public:
	explicit TreeMeshBuilder(unsigned gridEdgeSize);


private:
	auto marchCubes(const ParametricScalarField &field) -> unsigned override;

	auto decomposeSpace(
		float edgeLength,
		const Vec3_t<float> &cubeOffset,
		const ParametricScalarField &field
	) -> unsigned;

	auto isBlockEmpty(
		float edgeLength,
		const Vec3_t<float> &cubeOffset,
		const ParametricScalarField &field
	) -> bool;

	auto evaluateFieldAt(
		const Vec3_t<float> &pos, const ParametricScalarField &field
	) -> float override;

	void emitTriangle(const Triangle_t &triangle) override;

	auto getTrianglesArray() const -> const Triangle_t * override
	{
		return triangles.data();
	}


	constexpr static const float CUT_OFF = 1.F;
	std::vector<Triangle_t> triangles;
};


#endif // TREE_MESH_BUILDER_H
