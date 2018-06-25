#pragma once
#include <string>

struct LSTM_Argument
{
	uint8_t batch_learn;
	uint32_t timesteps;
};

class LongShortTermMemory
{
public:
	//maybe remove constructor as no object should be createt of this class
	LongShortTermMemory();
	~LongShortTermMemory();

	virtual void unroll() = 0;
	virtual void backpropagate() = 0;

	float * input;
	float * output;
	float * state;
private:

};

class DefaultLSTM : LongShortTermMemory
{
public:
	DefaultLSTM(LSTM_Argument *);
	~DefaultLSTM();

	void unroll();
	void backpropagate();
};

class GersSchmidhuberLSTM : LongShortTermMemory
{
public:
	GersSchmidhuberLSTM();
	~GersSchmidhuberLSTM();

private:

};


class ChoLSTM : LongShortTermMemory
{
public:
	ChoLSTM();
	~ChoLSTM();

private:

};
