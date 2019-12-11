/**
 * @file    tree_mesh_builder.h
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree
 *          early elimination.
 *
 * @date    10 December 2019, 23:58
 **/


#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H


#include "base_mesh_builder.h"


class TreeMeshBuilder : public BaseMeshBuilder
{
public:
	explicit TreeMeshBuilder(unsigned gridEdgeSize);


protected:
	auto marchCubes(const ParametricScalarField &field) -> unsigned override;

	auto evaluateFieldAt(
		const Vec3_t<float> &pos, const ParametricScalarField &field
	) -> float override;

	void emitTriangle(const Triangle_t &triangle) override;

	auto getTrianglesArray() const -> const Triangle_t * override
	{
		return nullptr;
	}
};


#endif // TREE_MESH_BUILDER_H
