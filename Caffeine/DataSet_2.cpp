#include "DataSet_2.h"

DataPoint::DataPoint(uint32_t n)
{
	elements = (Value**)malloc(n * sizeof(Value*));
}

Value * DataPoint::operator[](uint32_t)
{
	return nullptr;
}
