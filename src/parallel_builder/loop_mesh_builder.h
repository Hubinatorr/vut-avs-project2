/**
 * @file    loop_mesh_builder.h
 *
 * @author  Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops.
 *
 * @date    10 December 2019, 23:46
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
		return nullptr;
	}
};


#endif // LOOP_MESH_BUILDER_H
