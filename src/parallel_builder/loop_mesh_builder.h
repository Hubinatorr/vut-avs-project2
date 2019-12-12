/**
 * @file    loop_mesh_builder.h
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops.
 *
 * @date    12 December 2019, 00:29
 **/


#ifndef LOOP_MESH_BUILDER_H
#define LOOP_MESH_BUILDER_H


#include <vector>

#include "base_mesh_builder.h"


class LoopMeshBuilder : public BaseMeshBuilder
{
public:
	explicit LoopMeshBuilder(unsigned gridEdgeSize);


protected:
	auto marchCubes(const ParametricScalarField &field) -> unsigned override;

	auto evaluateFieldAt(
		const Vec3_t<float> &pos, const ParametricScalarField &field
	) -> float override;

	void emitTriangle(const Triangle_t &triangle) override;

	auto getTrianglesArray() const -> const Triangle_t * override
	{
		return triangles.data();
	}


	std::vector<Triangle_t> triangles;
};


#endif // LOOP_MESH_BUILDER_H
