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
				};

				class PerzeptronLayer
				{
				public:
					virtual void calculate() = 0;
					virtual float backpropagate(float*) = 0;
					virtual void backpropagate() = 0;

					virtual void update_weights() = 0;
					LayerArgument arg;
					float* _inputs;
					float* _activation;
					float* _outputs;
					float* _weights;
					float* _delta;
					float* _error;

					float* deltaforward;
					float* weightsforward;
					int outputsforwardlength;
				private:

				};

				class PerzeptronLayerHeavy : public PerzeptronLayer
				{
				public:
					PerzeptronLayerHeavy(LayerArgument);
					~PerzeptronLayerHeavy();

					inline void calculate();
					inline float backpropagate(float*);
					inline void backpropagate();

					inline void update_weights();
					float* _weightsdelta;
				private:

				};





				class MultilayerPerzeptron
				{
				public:
					MultilayerPerzeptron(int,LayerArgument*);
					~MultilayerPerzeptron();

					virtual float* feedForward(float*);
					virtual float feedBackward(float*);
					virtual bool save(std::string filename);

					PerzeptronLayer ** layers;
					int layercount;
					float error;
				private:

				};

			}

		}
	}
}

