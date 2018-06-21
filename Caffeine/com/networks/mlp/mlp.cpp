#include "mlp.h"

AI::ML::NN::MLP::PerzeptronLayer::PerzeptronLayer()
{
}

AI::ML::NN::MLP::PerzeptronLayerHeavy::PerzeptronLayerHeavy(LayerArgument argument)
{
	//copy the layer argument
	memcpy(&arg, &argument, sizeof(argument));
	std::random_device r;
	std::mt19937 e2(r());

	/*Allocate memory*/
	_activation = (float*)malloc(sizeof(float)*arg.OUTPUTS);
	_outputs = (float*)malloc(sizeof(float)*arg.OUTPUTS);
	_weights = (float*)malloc(sizeof(float)*(arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS));
	_weightsdelta = (float*)malloc(sizeof(float)*(arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS));
	_delta = (float*)malloc(sizeof(float)*arg.OUTPUTS);

	/*Initialize the weights*/
	switch (arg.distribution)
	{
		case ProbabilityDistributionNone:
		{
			for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
				_weights[i] = 0.0f;
			break;
		}
		case ProbabilityDistributionUniform:
		{
			std::uniform_real_distribution<float> distUni(arg.lower, arg.upper);
			for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
				_weights[i] = distUni(e2);
			break;
		}
		case ProbabilityDistributionNormal:
		{
			std::normal_distribution<float> distNorm((arg.lower + arg.upper)*0.5f, sqrt(0.5f*(powf(arg.lower - (arg.lower + arg.upper)*0.5f, 2) + 0.5f*powf(arg.upper - (arg.lower + arg.upper)*0.5f, 2))));
			for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
				_weights[i] = distNorm(e2);
			break;
		}
		case ProbabilityDistributionBoolean:
		{
			std::extreme_value_distribution<float> distBool(arg.lower, arg.upper);
			for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
				_weights[i] = distBool(e2);
			break;
		}
		case ProbabilityDistributionPoisson:
		{
			std::poisson_distribution<int> distPoiss((arg.lower + arg.upper)*0.5f);
			for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
				_weights[i] = distPoiss(e2);
			break;
		}
	}
}

AI::ML::NN::MLP::PerzeptronLayerHeavy::~PerzeptronLayerHeavy()
{
}

inline void AI::ML::NN::MLP::PerzeptronLayerHeavy::calculate()
{
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		_activation[i] = _weights[i*(arg.INPUTS + 1) + arg.INPUTS]; //Bias value
		for (int j = 0; j < arg.INPUTS; j++)
			_activation[i] += _inputs[j] * _weights[i*(arg.INPUTS + 1) + j];

		//Transfer the activation
		switch (arg.FUNCTION_ACTIVATION)
		{
		case ActivationFunctionIdentity:
			_outputs[i] = _activation[i];
			break;
		case ActivationFunctionAllOrNone:
			_outputs[i] = _activation[i] > 0 ? 1.0f : 0.0f;
			break;
		case ActivationFunctionSigmoid:
			_outputs[i] = 1.0f / (1.0f + expf(-_activation[i]));
			break;
		case ActivationFunctionTanH:
			_outputs[i] = tanhf(_activation[i]);
			break;
		case ActivationFunctionArcTan:
			_outputs[i] = atanf(_activation[i]);
			break;
		case ActivationFunctionSoftSign:
			_outputs[i] = _activation[i] / (1.0f + abs(_activation[i]));
			break;
		case ActivationFunctionInverseSquareRootU:
			_outputs[i] = _activation[i] / sqrtf(1 + powf(_activation[i], 2));
			break;
		case ActivationFunctionReLU:
			_outputs[i] = _activation[i] > 0 ? _activation[i] : 0.0f;
			break;
		case ActivationFunctionLeakyReLU:
			_outputs[i] = _activation[i] > 0 ? _activation[i] : 0.01f*_activation[i];
			break;
		case ActivationFunctionExpLU:
			_outputs[i] = _activation[i] > 0 ? _activation[i] : expf(_activation[i]) - 1.0f;
			break;
		case ActivationFunctionInverseSquareRootLU:
			_outputs[i] = _activation[i] < 0 ? _activation[i] / sqrtf(1.0f + powf(_activation[i], 2)) : _activation[i];
			break;
		case ActivationFunctionSoftPlus:
			_outputs[i] = logf(1.0f + expf(_activation[i]));
			break;
		case ActivationFunctionBentIdentity:
			_outputs[i] = (sqrtf(powf(_activation[i], 2) + 1.0f) - 1.0f) / 2.0f + _activation[i];
			break;
		case ActivationFunctionSinusoid:
			_outputs[i] = sinf(_activation[i]);
			break;
		case ActivationFunctionSinc:
			_outputs[i] = _activation[i] == 0.0f ? 1.0f : sinf(_activation[i]) / _activation[i];
			break;
		case ActivationFunctionGaussian:
			_outputs[i] = expf(-powf(_activation[i], 2));
			break;
		case ActivationFunctionSignum:
			_outputs[i] = _activation[i] > 0 ? 1.0f : -1.0f;
			break;
		default:
			_outputs[i] = 0.0f;
			break;
		}		
	}
	if (arg.FUNCTION_ACTIVATION == ActivationFunctionSoftmax)
	{
		float sum = 0.0f;
		for (int j = 0; j < arg.OUTPUTS; j++)
			sum += expf(_activation[j]);
		for (int j = 0; j < arg.OUTPUTS; j++)
			_outputs[j] = expf(_activation[j]) / sum;
	}
}

inline float AI::ML::NN::MLP::PerzeptronLayerHeavy::backpropagate(float * expected)
{
	//Adds up the errors and takes the mean
	float error_sum = 0.0f;

	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		//Calculate the derivative of the errorfunction with respect to the activation function
		//Also add the error to the sum
		switch (arg.FUNCTION_ERROR)
		{
		case ErrorFunctionMeanSquared:
			_delta[i] = expected[i] - _outputs[i];
			error_sum += powf(expected[i] - _outputs[i], 2);
			break;
		case ErrorFunctionCrossEntropy:
			_delta[i] = expected[i] / _outputs[i];
			error_sum += expected[i] * log(_outputs[i]);
			break;
		case ErrorFunctionHinge:
			_delta[i] = (1.0f - expected[i] * _outputs[i] > 0.0f) ? -expected[i] : 0.0f;
			error_sum += (1.0f - expected[i] * _outputs[i] > 0.0f) ? 1.0f - expected[i] * _outputs[i] : 0.0f;
			break;
		case ErrorFunctionHuber:
			_delta[i] = (abs(expected[i] - _outputs[i]) <= 1) ? expected[i] - _outputs[i] : 1.0f;
			error_sum += (abs(expected[i] - _outputs[i])<=1) ? 0.5f*powf(expected[i]-_outputs[i],2): abs(expected[i]-_outputs[i]) - 0.5f;
			break;
		case ErrorFunctionKullbackLeibler:
			_delta[i] = -expected[i] / _outputs[i];
			error_sum += expected[i] * log(expected[i] / _outputs[i]);
			break;
		case ErrorFunctionL1:

			break;
		case ErrorFunctionMaximumLikelihood:

			break;
		}

		//Calculate the derivative of the activation function with respect to the activation
		switch (arg.FUNCTION_ACTIVATION)
		{
		case ActivationFunctionIdentity:
			_delta[i] *= 1.0f;
			break;
		case ActivationFunctionAllOrNone:
			_delta[i] *= 0.0f;
			break;
		case ActivationFunctionSigmoid:
			_delta[i] *= 1.0f / (1.0f + expf(-_activation[i]))*(1.0f - 1.0f / (1.0f + expf(-_activation[i])));
			break;
		case ActivationFunctionTanH:
			_delta[i] *= 1.0f - powf(tanhf(_activation[i]), 2);
			break;
		case ActivationFunctionArcTan:
			_delta[i] *= 1.0f / (powf(_activation[i], 2) + 1.0f);
			break;
		case ActivationFunctionSoftSign:
			_delta[i] *= 1.0f / powf(1 + abs(_activation[i]), 2);
			break;
		case ActivationFunctionInverseSquareRootU:
			_delta[i] *= powf(1.0f / sqrtf(1.0f + powf(_activation[i], 2)), 3.0f);
			break;
		case ActivationFunctionReLU:
			_delta[i] *= _activation[i] > 0 ? 1.0f : 0.0f;
			break;
		case ActivationFunctionLeakyReLU:
			_delta[i] *= _activation[i] > 0 ? 1.0f : 0.01f;
			break;
		case ActivationFunctionExpLU:
			_delta[i] *= _activation[i] < 0 ? expf(_activation[i]) : 1.0f;
			break;
		case ActivationFunctionInverseSquareRootLU:
			_delta[i] *= _activation[i] < 0 ? powf(1.0f / sqrtf(1.0f + powf(_activation[i], 2)), 3.0f) : 1.0f;
			break;
		case ActivationFunctionSoftPlus:
			_delta[i] *= 1.0f / (1.0f + expf(-_activation[i]));
			break;
		case ActivationFunctionBentIdentity:
			_delta[i] *= _activation[i] / (2.0f*sqrtf(powf(_activation[i], 2) + 1.0f)) + 1.0f;
			break;
		case ActivationFunctionSinusoid:
			_delta[i] *= cosf(_activation[i]);
			break;
		case ActivationFunctionSinc:
			_delta[i] *= _activation[i] == 0.0f ? 0.0f : cosf(_activation[i]) / _activation[i] - sinf(_activation[i]) / powf(_activation[i], 2);
			break;
		case ActivationFunctionGaussian:
			_delta[i] *= -2.0f*_activation[i] * expf(-powf(_activation[i], 2));
			break;
		case ActivationFunctionSignum:
			_delta[i] *= 0.0f;
			break;
		default:
			_delta[i] *= 0.0f;
			break;
		}

		//Compute the gradient change for each weight
		for (int j = 0; j < arg.INPUTS; j++) 
			_weightsdelta[i*(arg.INPUTS + 1) + j] = _delta[i] * _inputs[j];
		//Compute the gradient change for each bias
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] = _delta[i];
	}
	//return the MeanL2
	return error_sum/arg.OUTPUTS;
}

inline void AI::ML::NN::MLP::PerzeptronLayerHeavy::backpropagate()
{
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		//Gather the errors (deltas) of all the neurons in the next layer connected to the current neuron
		_delta[i] = 0.0f;
		for (int j = 0; j < outputsforwardlength; j++)
			_delta[i] += weightsforward[j*(arg.OUTPUTS + 1) + i]*deltaforward[j];

		//Calculate the derivative of the activation function w.r.t. the activation
		switch (arg.FUNCTION_ACTIVATION)
		{
		case ActivationFunctionIdentity:
			_delta[i] *= 1.0f;
			break;
		case ActivationFunctionAllOrNone:
			_delta[i] *= 0.0f;
			break;
		case ActivationFunctionSigmoid:
			_delta[i] *= 1.0f / (1.0f + expf(-_activation[i]))*(1.0f - 1.0f / (1.0f + expf(-_activation[i])));
			break;
		case ActivationFunctionTanH:
			_delta[i] *= 1.0f - powf(tanhf(_activation[i]), 2);
			break;
		case ActivationFunctionArcTan:
			_delta[i] *= 1.0f / (powf(_activation[i], 2) + 1.0f);
			break;
		case ActivationFunctionSoftSign:
			_delta[i] *= 1.0f / powf(1 + abs(_activation[i]), 2);
			break;
		case ActivationFunctionInverseSquareRootU:
			_delta[i] *= powf(1.0f / sqrtf(1.0f + powf(_activation[i], 2)), 3.0f);
			break;
		case ActivationFunctionReLU:
			_delta[i] *= _activation[i] > 0 ? 1.0f : 0.0f;
			break;
		case ActivationFunctionLeakyReLU:
			_delta[i] *= _activation[i] > 0 ? 1.0f : 0.01f;
			break;
		case ActivationFunctionExpLU:
			_delta[i] *= _activation[i] < 0 ? expf(_activation[i]) : 1.0f;
			break;
		case ActivationFunctionInverseSquareRootLU:
			_delta[i] *= _activation[i] < 0 ? powf(1.0f / sqrtf(1.0f + powf(_activation[i], 2)), 3.0f) : 1.0f;
			break;
		case ActivationFunctionSoftPlus:
			_delta[i] *= 1.0f / (1.0f + expf(-_activation[i]));
			break;
		case ActivationFunctionBentIdentity:
			_delta[i] *= _activation[i] / (2.0f*sqrtf(powf(_activation[i], 2) + 1.0f)) + 1.0f;
			break;
		case ActivationFunctionSinusoid:
			_delta[i] *= cosf(_activation[i]);
			break;
		case ActivationFunctionSinc:
			_delta[i] *= _activation[i] == 0.0f ? 0.0f : cosf(_activation[i]) / _activation[i] - sinf(_activation[i]) / powf(_activation[i], 2);
			break;
		case ActivationFunctionGaussian:
			_delta[i] *= -2.0f*_activation[i] * expf(-powf(_activation[i], 2));
			break;
		case ActivationFunctionSignum:
			_delta[i] *= 0.0f;
			break;
		default:
			_delta[i] *= 0.0f;
			break;
		}

		//Calculate the change of each weight 
		for (int j = 0; j < arg.INPUTS; j++)
			_weightsdelta[i*(arg.INPUTS + 1) + j] = _delta[i] * _inputs[j];
		//Calculate the change of each bias
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] = _delta[i];
	}
}

inline void AI::ML::NN::MLP::PerzeptronLayerHeavy::update_weights()
{
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		for (int j = 0; j < arg.INPUTS; j++)
		{
			switch (arg.OPTIMIZER)
			{
			case OptimizerGD:
				//maybe there is an error... this is gradient ascend
				//Change each weight with learningrate alpha in such a way, that it will reduce the error
				_weights[i*(arg.INPUTS + 1) + j] += _weightsdelta[i*(arg.INPUTS + 1) + j] * arg.ALPHA;
				break;
			default:
				break;
			}
		}
		//Change the bias in with GradientDescent method
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] += _weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS]*arg.ALPHA;
	}
}


AI::ML::NN::MLP::MultilayerPerzeptron::MultilayerPerzeptron(int layer_count, LayerArgument* arguments)
{
	//There is one more argument then layers
	//The XOR has two layers, in this implementation you need 3 arguments
	layers = new PerzeptronLayer*[layer_count - 1];

	//Length of layers array
	layercount = layer_count - 1;
	for (int i = 0; i < layercount; i++)
	{
		switch (arguments[i].style)
		{
		case ArchitectureStyleDefault:
			//Create new perceptron layer
			layers[i] = new PerzeptronLayerHeavy(arguments[i]);
			//Connect the inputs of the current layer with the outputs of the previous layer (ignore at layer 0)
			if (i>0)	layers[i]->_inputs = layers[i - 1]->_outputs;
			break;
		case ArchitectureStyleMemoryLight:

			break;
		default:
			break;
		}
	}

	for (int i = layercount - 1; i >= 1; i--)
	{
		//pass reference to the deltas to the previous layer
		layers[i - 1]->deltaforward = layers[i]->_delta;
		//same fow weights
		layers[i - 1]->weightsforward = layers[i]->_weights;
		//and the number of outputs
		layers[i - 1]->outputsforwardlength = layers[i]->arg.OUTPUTS;
	}
}

AI::ML::NN::MLP::MultilayerPerzeptron::~MultilayerPerzeptron()
{
}

inline float * AI::ML::NN::MLP::MultilayerPerzeptron::feedForward(float * input)
{
	//Change the reference to the current input
	layers[0]->_inputs = input;
	//Calculate each layer. Because the inputs of a layer point to the ouputs of the previous layer, no further statements are needed
	for (int i = 0; i < layercount; i++)
		layers[i]->calculate();
	//return the result of the net
	return layers[layercount-1]->_outputs;

	//TODO maybe change it and return nothing, as ouput pointer never changes
}

float AI::ML::NN::MLP::MultilayerPerzeptron::feedBackward(float * expected)
{
	//The ouput layer gets the expected values and returns the error
	error = layers[layercount - 1]->backpropagate(expected);
	//From the second topmost layer to the input layer, the error is backpropagated through the net
	for (int i = layercount - 2; i >= 0; i--)
		layers[i]->backpropagate();
	//After each layer has calculated its weight changes, the layers get updated.
	for (int i = 0; i < layercount; i++)
		layers[i]->update_weights();
	//TODO In further versions change this so it also accepts batch learning
	return error;
}

bool AI::ML::NN::MLP::MultilayerPerzeptron::save(std::string filename)
{
	std::ofstream o_file(filename.c_str(), std::ofstream::binary);
	
	
	//o_file.write(static_cast<char*>(0.1f), sizeof(float));
	return false;
}
