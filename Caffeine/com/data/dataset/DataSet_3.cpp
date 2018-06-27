#include "DataSet_3.h"


void Value::operator=(float v)
{
	memcpy(address, (uint8_t*)(&v), sizeof(float));
}
void Value::operator=(double v)
{
	memcpy(address, (uint8_t*)(&v), sizeof(double));
}
void Value::operator=(int v)
{
	memcpy(address, (uint8_t*)(&v), sizeof(int));
}
void Value::operator=(int64_t v)
{
	memcpy(address, (uint8_t*)(&v), sizeof(int64_t));
}
void Value::operator=(std::string v)
{
	uint64_t a = (uint64_t)new std::string(v);
	memcpy(address, (uint8_t*)(&a), sizeof(uint64_t));
}

Value::operator float()
{
	return *((float*)address);
}
Value::operator double()
{
	return *((double*)address);
}
Value::operator int()
{
	return *((int*)address);
}
Value::operator int64_t()
{
	return *((int64_t*)address);
}
Value::operator std::string()
{
	return *((std::string*)(*(uint64_t*)address));
}

std::ostream & operator<<(std::ostream & o, Value & v)
{
	switch (v.t)
	{
	case F32: o << 0; break;
	case F64: o << 0; break;
	case I32: o << 0; break;
	case I64: o << 0; break;
	case STR: o << 0; break;
	}
	return o;
}