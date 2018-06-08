#pragma once

struct LSTM_Argument
{

};

class LongShortTermMemory
{
public:
	//maybe remove constructor as no object should be createt of this class
	LongShortTermMemory();
	~LongShortTermMemory();

	float * input;
	float * output;
	float * state;
private:

};

class DefaultLSTM : LongShortTermMemory
{
public:
	DefaultLSTM();
	~DefaultLSTM();

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
