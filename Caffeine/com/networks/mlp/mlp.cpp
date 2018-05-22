#include "mlp.h"

PerzeptronLayerHeavy::PerzeptronLayerHeavy(LayerArgument argument)
{
	arg = argument;
	std::random_device r;
	std::mt19937 e2(r());


	_activation = (float*)malloc(sizeof(float)*arg.OUTPUTS);
	_outputs = (float*)malloc(sizeof(float)*arg.OUTPUTS);
	_weights = (float*)malloc(sizeof(float)*(arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS));
	_weightsdelta = (float*)malloc(sizeof(float)*(arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS));
	_delta = (float*)malloc(sizeof(float)*arg.OUTPUTS);

	switch (arg.distribution)
	{
	case ProbabilityDistributionNone:
	{
		for (int i = 0; i < (arg.OUTPUTS*arg.INPUTS + arg.OUTPUTS); i++)
			_weights[i] = 1.0f;
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

PerzeptronLayerHeavy::~PerzeptronLayerHeavy()
{
}

inline void PerzeptronLayerHeavy::calculate()
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

inline float PerzeptronLayerHeavy::backpropagate(float * expected)
{
	float error_sum = 0.0f;
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		//Calculate the derivative of the errorfunction with respect to the activation function
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

		for (int j = 0; j < arg.INPUTS; j++) //Compute the gradient change for each weight
			_weightsdelta[i*(arg.INPUTS + 1) + j] = _delta[i] * _inputs[j];
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] = _delta[i];
	}
	return error_sum/arg.OUTPUTS;
}

inline void PerzeptronLayerHeavy::backpropagate()
{
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		_delta[i] = 0.0f;
		for (int j = 0; j < outputsforwardlength; j++)
			_delta[i] += weightsforward[j*(arg.OUTPUTS + 1) + i]*deltaforward[j];

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

		for (int j = 0; j < arg.INPUTS; j++)
			_weightsdelta[i*(arg.INPUTS + 1) + j] = _delta[i] * _inputs[j];
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] = _delta[i];
	}
}

inline void PerzeptronLayerHeavy::update_weights()
{
	for (int i = 0; i < arg.OUTPUTS; i++)
	{
		for (int j = 0; j < arg.INPUTS; j++)
		{
			switch (arg.OPTIMIZER)
			{
			case OptimizerGD:
				_weights[i*(arg.INPUTS + 1) + j] += _weightsdelta[i*(arg.INPUTS + 1) + j] * arg.ALPHA;
				break;
			default:
				break;
			}
		}
		_weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS] += _weightsdelta[i*(arg.INPUTS + 1) + arg.INPUTS]*arg.ALPHA;
	}
}


MultilayerPerzeptron::MultilayerPerzeptron(int layer_count, LayerArgument* arguments)
{
	layers = new PerzeptronLayer*[layer_count - 1];
	layercount = layer_count - 1;
	for (int i = 0; i < layercount; i++)
	{
		switch (arguments[i].style)
		{
		case ArchitectureStyleDefault:
			layers[i] = new PerzeptronLayerHeavy(arguments[i]);
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
		layers[i - 1]->deltaforward = layers[i]->_delta;
		layers[i - 1]->weightsforward = layers[i]->_weights;
		layers[i - 1]->outputsforwardlength = layers[i]->arg.OUTPUTS;
	}
}

MultilayerPerzeptron::~MultilayerPerzeptron()
{
}

inline float * MultilayerPerzeptron::feedForward(float * input)
{
	layers[0]->_inputs = input;
	for (int i = 0; i < layercount; i++)
		layers[i]->calculate();
	return layers[layercount-1]->_outputs;
}

float MultilayerPerzeptron::feedBackward(float * expected)
{
	error = layers[layercount - 1]->backpropagate(expected);
	for (int i = layercount - 2; i >= 0; i--)
		layers[i]->backpropagate();
	for (int i = 0; i < layercount; i++)
		layers[i]->update_weights();
	return error;
}

bool MultilayerPerzeptron::save(std::string filename)
{
	std::ofstream o_file(filename.c_str(), std::ofstream::binary);
	
	
	//o_file.write(static_cast<char*>(0.1f), sizeof(float));
	return false;
}
