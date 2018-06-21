#pragma once
#include <cstdlib>
#include <cmath>
#include <random>
#include <fstream>


namespace AI
{
	namespace ML
	{
		namespace NN
		{
			//TODO maybe put those things in a own header file for neural networks like NN::utils

			// Different types of activation functions common for deep learning applications
			enum ActivationFunction
			{
				ActivationFunctionIdentity,
				ActivationFunctionAllOrNone,
				ActivationFunctionSigmoid,
				ActivationFunctionTanH,
				ActivationFunctionArcTan,
				ActivationFunctionSoftSign,
				ActivationFunctionInverseSquareRootU,
				ActivationFunctionReLU,
				ActivationFunctionLeakyReLU,
				ActivationFunctionExpLU,
				ActivationFunctionSshapedReLU,
				ActivationFunctionInverseSquareRootLU,
				ActivationFunctionSoftPlus,
				ActivationFunctionBentIdentity,
				ActivationFunctionSinusoid,
				ActivationFunctionSinc,
				ActivationFunctionGaussian,
				ActivationFunctionSignum,
				ActivationFunctionSoftmax
			};

			// Provides different error functions for backpropagation
			enum ErrorFunction
			{
				ErrorFunctionMeanSquared,
				ErrorFunctionCrossEntropy,
				ErrorFunctionHinge,
				ErrorFunctionHuber,
				ErrorFunctionKullbackLeibler,
				ErrorFunctionL1,
				ErrorFunctionL2,
				ErrorFunctionMaximumLikelihood
			};

			//Diffrent types of optimization methods for backpropagation
			enum Optimizer
			{
				OptimizerGD,
				OptimizerAdadelta,
				OptimizerAdagrad,
				OptimizerAdam,
				OptimizerConjugateGradients,
				OptimizerBFGS,
				OptimizerMomentum,
				OptimizerNesterovMomentum,
				OptimizerNewtonMethod,
				OptimizerRMSProp,
				OptimizerSGD
			};

			// Different types of functions used in CNN pooling layers 
			enum PoolingFunction
			{
				PoolingFunctionMax,
				PoolingFunctionMean
			};

			// Choose between the devices you desire running your code on
			enum TargetDevice
			{
				TargetDeviceCPU,
				TargetDeviceParallelCPU,
				TargetDeviceGPU,
				TargetDeviceParallelGPU
			};

			// Choose how you want to initialize the weight matrices of your network
			enum ProbabilityDistribution
			{
				ProbabilityDistributionNone,
				ProbabilityDistributionNormal,
				ProbabilityDistributionBoolean,
				ProbabilityDistributionUniform,
				ProbabilityDistributionPoisson,
				ProbabilityDistributionXavier
			};

			// Provides different data-preprocessing
			enum Preprocessing
			{
				PreprocessingGaussianSmoother,
				PreprocessingMeanSmoother
			};

			enum ArchitectureStyle
			{
				ArchitectureStyleDefault,
				ArchitectureStyleMemoryLight
			};

			namespace MLP
			{
				//A struct for initializing a perceptron layer
				struct LayerArgument
				{
					const uint32_t OUTPUTS;
					const uint32_t INPUTS;
					const int FUNCTION_ACTIVATION;
					const int FUNCTION_ERROR;
					const int OPTIMIZER;
					float ALPHA;
					int style;
					int distribution;
					float lower;
					float upper;


					//outputs: The number of neurons in the layer.
					//inputs: The number of neurons in the previous layer.
					//activation_function: The transfer funciton of the layer.
					//error_function: Only neccesary for the output layer. Calculates the error.
					//optimizer: Describes the way the weights are adapted.
					LayerArgument(uint32_t outputs, uint32_t inputs, int activation_function, int error_function, int optimizer) :
						OUTPUTS(outputs), INPUTS(inputs), FUNCTION_ACTIVATION(activation_function), FUNCTION_ERROR(error_function), OPTIMIZER(optimizer)
					{
							
					}

					//The default constructor to avoid compiler errors.
					LayerArgument() :
						OUTPUTS(1), INPUTS(2), FUNCTION_ACTIVATION(ActivationFunctionSigmoid), FUNCTION_ERROR(ErrorFunctionL2), OPTIMIZER(OptimizerGD)
					{

					}

				};

				class PerzeptronLayer
				{
				public:
					//The default constructor to avoid compiler errors.
					//Do not use it, as it will result in errors.
					//Instead use the derived classes, PerzeptronLayerHeavy, etc.
					PerzeptronLayer();

					virtual void calculate() = 0;
					virtual float backpropagate(float*) = 0;
					virtual void backpropagate() = 0;

					virtual void update_weights() = 0;
					//Information about the layer
					LayerArgument arg;
					//The input to the layer
					float* _inputs;
					//The result of w*x+b while w being the weights, x the inputs and b the bias
					float* _activation;
					//The result of trans(a) while trans() being the activation-/transfer function and a the activation
					float* _outputs;
					float* _weights;
					//In case this is a output layer, delta contains the derivative of the error function multiplied with the derivative of the activation function
					//In case this is a hidden layer, delta is calculated by the sum of the weightsforward * deltaforward (see members deltaforward and weightsforward for detail)
					float* _delta;
					float* _error;

					//If not the output layer, this points to the _delta array of the next layer
					float* deltaforward;
					//If not the output layer, this points to the _weights of the next layer
					float* weightsforward;
					//Is equal to the output length of the next layer
					int outputsforwardlength;
				private:

				};

				class PerzeptronLayerHeavy : public PerzeptronLayer
				{
				public:
					//Creates a new instance of a perceptron layer wich has more memory usage, but less computation
					PerzeptronLayerHeavy(LayerArgument);
					PerzeptronLayerHeavy() = default;
					~PerzeptronLayerHeavy();

					//calculates a = w*x+b
					//a: _activation
					//w: _weights
					//x: _inputs
					//b: bias (contained in weights)
					inline void calculate();
					//The output layer recieves a target array.
					//It calculates the _delta values (see member for details), as well as its own raw weight change
					inline float backpropagate(float*);
					//The hidden layer calculates its _delta values (see member for details), as well as its own raw weight change
					inline void backpropagate();

					//Adapts the weights after the method given by optimizer
					inline void update_weights();
					float* _weightsdelta;
				private:

				};




				//A class containing multiple perceptron layers already connected and ready to use
				class MultilayerPerzeptron
				{
				public:
					MultilayerPerzeptron(int,LayerArgument*);
					~MultilayerPerzeptron();

					//calculates an input through the layers of the net
					float* feedForward(float*);
					//backpropagates the expected result for the last calculation through the net
					float feedBackward(float*);
					bool save(std::string filename);

					PerzeptronLayer ** layers;
					int layercount;
					float error;
				private:

				};

			}

		}
	}
}

