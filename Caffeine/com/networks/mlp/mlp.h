#pragma once
#include <cstdlib>
#include <cmath>
#include <random>
#include <fstream>



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

