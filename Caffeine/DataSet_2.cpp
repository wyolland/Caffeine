#include "DataSet_2.h"

DataPoint::DataPoint(uint32_t n)
{
	elements = (Value**)malloc(n * sizeof(Value*));
}

Value& DataPoint::operator[](uint32_t i)
{	
	return *elements[i];
}


DataSet::DataSet(Header * head, size_t cap)
{
	point = new DataPoint(head->n);
	for (size_t i = 0; i < head->n; i++)
	{
		switch (head->t[i])
		{
		case F32: point->elements[i] = new Float32(); break;
		case F64: point->elements[i] = new Float64(); break;
		case I32: point->elements[i] = new Int32(); break;
		case I64: point->elements[i] = new Int64(); break;
		case STR: point->elements[i] = new Str(); break;
		}
	}
}

DataPoint& DataSet::operator[](size_t n)
{
	uint8_t offset = 0;
	for (size_t i = 0; i < head->n; i++)
	{
		switch (head->t[i])
		{
		case F32: ((Float32*)point->elements[i])->x		= *(float*)&data[n*datapointsize + offset];										offset += sizeof(float);	break;
		case F64: ((Float64*)point->elements[i])->x		= *(double*)&data[n*datapointsize + offset];									offset += sizeof(double);	break;
		case I32: ((Int32*)point->elements[i])->x		= *(int*)&data[n*datapointsize + offset];										offset += sizeof(int);		break;
		case I64: ((Int64*)point->elements[i])->x		= *(int64_t*)&data[n*datapointsize + offset];									offset += sizeof(int64_t);	break;
		case STR: ((Str*)point->elements[i])->x			= ((std::string*)(*(uint64_t*)&data[n*datapointsize + offset]))->c_str();		offset += sizeof(uint64_t); break;
		}
	}
	return *point;
}
