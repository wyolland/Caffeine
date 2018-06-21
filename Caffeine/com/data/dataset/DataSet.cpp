#include "DataSet.h"

#pragma region Header

Header::Header(std::string* n, int* t, uint32_t size)
{
	names = n;
	types = t;
	length = size;
}

Header::~Header()
{
}
#pragma endregion




#pragma region Tuple

Tuple::Tuple(uint32_t n)
{
	elements = (Value**)malloc(sizeof(Value*)*n);
}

void Tuple::set(uint32_t i, Value * v)
{
	elements[i] = v;
}

Tuple::~Tuple()
{
}
#pragma endregion




#pragma region Data
Data::Data(Header * h, uint64_t size)
{
	datapointsize = 0;
	insertIndex = 0;
	datapointcount = 0;
	header = h;
	for (uint32_t i = 0; i < header->length; i++)
	{
		switch (header->types[i])
		{
		case F32:
			datapointsize += sizeof(float);
			break;
		case F64:
			datapointsize += sizeof(double);
			break;
		case I32:
			datapointsize += sizeof(int);
			break;
		case I64:
			datapointsize += sizeof(int64_t);
			break;
		case STR:
			datapointsize += sizeof(uint64_t);
			break;
		/*case UNK:
			datapointsize += sizeof(char);
			break;*/
		}
	}
	data = (uint8_t*)malloc(datapointsize*size);
}

Data::~Data()
{
}

void Data::pushback(Tuple t)
{
	//TODO add dynamic size

	uint32_t offset = 0;
	for (uint32_t i = 0; i < header->length; i++)
	{
		uint32_t a;
		switch (header->types[i])
		{
		case F32:
			memcpy(&data[insertIndex + offset], (uint8_t*)(&(((Float32*)t.elements[i])->x)), sizeof(float));
			offset += sizeof(float);
			break;
		case F64:
			memcpy(&data[insertIndex + offset], (uint8_t*)(&(((Float64*)t.elements[i])->x)), sizeof(double));
			offset += sizeof(double);
			break;
		case I32:
			memcpy(&data[insertIndex + offset], (uint8_t*)(&(((Int32*)t.elements[i])->x)), sizeof(int));
			offset += sizeof(int);
			break;
		case I64:
			memcpy(&data[insertIndex + offset], (uint8_t*)(&(((Int64*)t.elements[i])->x)), sizeof(int64_t));
			offset += sizeof(int64_t);
			break;
		case STR:
			std::string * c = new std::string(((Str*)t.elements[i])->x.c_str());
			a = (uint64_t)c;

			memcpy(&data[insertIndex + offset], (uint8_t*)&a, sizeof(uint64_t));
			offset += sizeof(uint64_t);
			break;
		/*case UNK:
			data[insertIndex + offset + sizeof(char)] = 1;
			offset += sizeof(char);
			break;*/
		}
	}
	insertIndex += datapointsize;
	datapointcount++;
}

void Data::print()
{
	for (uint32_t i = 0; i < datapointcount; i++)
	{
		uint32_t offset = 0;
		for (uint32_t j = 0; j < header->length; j++)
		{
			switch (header->types[j])
			{
			case F64:
				std::numeric_limits<double>::max()== *(double*)&data[i*datapointsize + offset] ? printf("-????- \t") : printf("%f \t", *(double*)&data[i*datapointsize + offset]);
				offset += sizeof(double);
				break;
			case F32:
				std::numeric_limits<float>::max() == *(float*)&data[i*datapointsize + offset] ? printf("-????- \t") : printf("%f \t", *(float*)&data[i*datapointsize + offset]);
				offset += sizeof(float);
				break;
			case I64:
				0xFFFFFFFFFFFFFFFF == *(int64_t*)&data[i*datapointsize + offset] ? printf("-????- \t") : printf("%lld \t", *(int64_t*)&data[i*datapointsize + offset]);
				offset += sizeof(int64_t);
				break;
			case I32:
				std::numeric_limits<int>::max() == *(int*)&data[i*datapointsize + offset] ? printf("-????- \t") : printf("%d \t", *(int*)&data[i*datapointsize + offset]);		
				offset += sizeof(int);
				break;
			case STR:
				printf("%s \t", ((std::string*)(*(uint64_t*)&data[i*datapointsize + offset]))->c_str());
				offset += sizeof(uint64_t);
				break;
			/*case UNK:
				printf("??? \t");
				offset += sizeof(char);
				break;*/
			}
		}
		printf(" \n");
	}
}
#pragma endregion