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

struct Value { 
	uint8_t t; 
	Value() {} 
	void operator=(float v); 
	void operator=(double v);
	void operator=(int v); 
	void operator=(int64_t v); 
	void operator=(std::string v);
	friend std::ostream& operator<<(std::ostream& o, Value& v);
};
struct Float32 : Value	{ float x;			Float32(float f) :x(f)	{ t = F32; }	Float32() :x(std::numeric_limits<float>::max())		{ t = F32; }};
struct Float64 : Value	{ double x;			Float64(double d) :x(d) { t = F64; }	Float64() :x(std::numeric_limits<double>::max())	{ t = F64; }};
struct Int32 : Value	{ int x;			Int32(int i) :x(i)		{ t = I32; }	Int32() :x(std::numeric_limits<int>::max())			{ t = I32; }};
struct Int64 : Value	{ long long x;		Int64(long long l):x(l)	{ t = I64; }	Int64() :x(std::numeric_limits<long>::max())		{ t = I64; }};
struct Str : Value		{ std::string x;	Str(std::string s) :x(s){ t = STR; }	Str() :x("-???-")									{ t = STR; }};

struct Header
{
	size_t n;
	uint8_t* types;
	std::string* names;
	Header(std::string*, uint8_t*, size_t);
	~Header();
};

struct DataPoint
{
	size_t types;
	uint32_t length;
	Value** elements;
	void* parent;

	DataPoint(uint32_t, void*);


	Value& operator[](uint32_t);
	friend std::ostream& operator<<(std::ostream& o, DataPoint& p);
};

struct DataSet
{
	Header * head;
	DataPoint * point;
	uint8_t * data;
	std::string name = "DataSet";
	size_t datapointsize, datapointcount, insertindex, capacity;

	DataSet(Header * head, uint32_t cap = 1000);
	~DataSet();

	void commit();

	DataSet& operator+(const DataPoint&);
	bool operator+=(const DataPoint&);

	DataSet& operator+(const DataSet&);
	bool operator+=(const DataSet&);

	DataPoint& operator[](size_t);
	friend std::ostream& operator<<(std::ostream& o, DataSet& p);
};