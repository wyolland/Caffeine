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

struct Value { uint8_t t; Value() {} };
struct Float32 : Value	{ float x;			Float32(float f) :x(f)	{ t = F32; }	Float32() :x(std::numeric_limits<float>::max())		{ t = F32; }};
struct Float64 : Value	{ double x;			Float64(double d) :x(d) { t = F64; }	Float64() :x(std::numeric_limits<double>::max())	{ t = F64; }};
struct Int32 : Value	{ int x;			Int32(int i) :x(i)		{ t = I32; }	Int32() :x(std::numeric_limits<int>::max())			{ t = I32; }};
struct Int64 : Value	{ long x;			Int64(long l):x(l)		{ t = I64; }	Int64() :x(std::numeric_limits<long>::max())		{ t = I64; }};
struct Str : Value		{ std::string x;	Str(std::string s) :x(s){ t = STR; }	Str() :x("-???-")									{ t = STR; }};

struct Header
{

};

struct DataPoint
{
	size_t types;
	uint32_t length;
	Value** elements;
	DataPoint(uint32_t n);

	Value* operator[](uint32_t);
	
};

struct DataSet
{
	Header * head;
	uint8_t * data;
	size_t datapointsize, datapointcount, insertindex, capacity;

	DataSet(Header * head, size_t cap = 1000);
	~DataSet();

	DataSet& operator+(const DataPoint&);
	bool operator+=(const DataPoint&);

	DataSet& operator+(const DataSet&);
	bool operator+=(const DataSet&);

	DataPoint& operator[](size_t);
};