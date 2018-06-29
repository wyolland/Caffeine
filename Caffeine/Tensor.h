#pragma once
#include <cstdlib>


namespace CG
{
	struct Tensor
	{
		float * data;
		const size_t length;
		const size_t dimX, dimY;

		Tensor(size_t dim_X, size_t dim_Y)
			:dimX(dim_X), dimY(dim_Y), length(dim_X*dim_Y)
		{
			data = (float*)malloc(length * sizeof(float));
		}
		~Tensor()
		{
			free(data);
		}
	};

	template<typename T> struct AnyTensor
	{
		T * data;
		size_t len;

	};
}
