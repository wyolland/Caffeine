#pragma once

#include "Tensor.h"


namespace CG
{
	struct Node
	{
		Tensor **inputs, **outputs;
		size_t input_length, output_length;
	};

	struct Matmul : Node
	{
		//Node to multiply matrix A*B = C if dimensions match
		Matmul(Tensor * A, Tensor * B)
		{
			input_length = 2;
			output_length = 1;
			inputs = (Tensor**)malloc(input_length * sizeof(Tensor*));
			outputs = (Tensor**)malloc(output_length * sizeof(Tensor*));
		}
	};
}
