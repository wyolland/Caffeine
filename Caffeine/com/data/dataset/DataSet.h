#pragma once
#include <cstdlib>
#include <string>
#include <limits>

enum TypeName
{
	F32,
	F64,
	I32,
	I64,
	STR
};

struct Value { Value() {} };
struct Float32 : Value	{ float x;			Float32(float f)	:x(f) {}	Float32()	:x(std::numeric_limits<float>::max())	{} };
struct Float64 : Value	{ double x;			Float64(double d)	:x(d) {}	Float64()	:x(std::numeric_limits<double>::max())	{} };
struct Int32 : Value	{ int x;			Int32(int i)		:x(i) {}	Int32()		:x(std::numeric_limits<int>::max())		{} };
struct Int64 : Value	{ long x;			Int64(long l)		:x(l) {}	Int64()		:x(std::numeric_limits<long>::max())	{} };
struct Str : Value		{ std::string x;	Str(std::string s)	:x(s) {}	Str()		:x("-???-")								{} };

struct Header
{
	Header(std::string* n, int* t, uint32_t size);
	~Header();

	std::string * names;
	int * types;
	uint32_t length;
};

struct Tuple
{
	Tuple(uint32_t n);
	~Tuple();
	Value ** elements;

	void set(uint32_t i, Value * v);
};

struct Data
{
	Header * header;
	uint8_t * data;
	uint32_t datapointsize;
	uint32_t insertIndex;
	uint32_t datapointcount;
	Data(Header * h, uint64_t size);
	~Data();

	//The values of the Tuple are copied so the tuple can be reassigned and reused
	void pushback(Tuple t);
	void print();
};