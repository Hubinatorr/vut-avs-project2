/**
 * @file    ref_mesh_builder.h
 *
 * @authors Filip Vaverka  <ivaverka@fit.vutbr.cz>
 *          Filip Kuklis   <ikuklis@fit.vutbr.cz>
 *          Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 *
 * @brief   Reference (sequential) implementation of "BaseMeshBuilder".
 *
 * @date    11 December 2019, 02:08
 **/


#ifndef REF_MESH_BUILDER_H
#define REF_MESH_BUILDER_H


#include <vector>

#include "base_mesh_builder.h"


/**
 * @brief The RefMeshBuilder class.
 */
class RefMeshBuilder : public BaseMeshBuilder
{
public:
	explicit RefMeshBuilder(unsigned gridEdgeSize);


protected:
	auto marchCubes(const ParametricScalarField &field) -> unsigned override;

	auto evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field) -> float override;

	void emitTriangle(const Triangle_t &triangle) override;

	auto getTrianglesArray() const -> const Triangle_t * override
	{
		return mTriangles.data();
	}


	std::vector<Triangle_t> mTriangles; ///< Temporary array of triangles.
};


#endif // REF_MESH_BUILDER_H
