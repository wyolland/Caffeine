#pragma once
#include <cstdlib>
#include <string>
#include <limits>

namespace DATA
{
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
		uint8_t * address;
		Value() {}

		//Every value struct, that inherits from value, needs an assign and cast operator for the desired type

		void operator=(float);
		void operator=(double);
		void operator=(int);
		void operator=(int64_t);
		void operator=(std::string);

		operator float();
		operator double();
		operator int();
		operator int64_t();
		operator std::string();

		friend std::ostream& operator<<(std::ostream& o, Value& v);
	};
	struct Float32 : Value { float x;			Float32(float f) :x(f) { t = F32; }	Float32() :x(std::numeric_limits<float>::max()) { t = F32; } };
	struct Float64 : Value { double x;			Float64(double d) :x(d) { t = F64; }	Float64() :x(std::numeric_limits<double>::max()) { t = F64; } };
	struct Int32 : Value { int x;			Int32(int i) :x(i) { t = I32; }	Int32() :x(std::numeric_limits<int>::max()) { t = I32; } };
	struct Int64 : Value { long long x;		Int64(long long l) :x(l) { t = I64; }	Int64() :x(std::numeric_limits<long>::max()) { t = I64; } };
	struct Str : Value { std::string x;	Str(std::string s) :x(s) { t = STR; }	Str() :x("-???-") { t = STR; } };



	struct Header
	{
		uint32_t n;
		uint8_t* types;
		std::string* names;
		Header(std::string*, uint8_t*, uint32_t);
		~Header();
	};

	//This is a helper struct for the DataSet. Its only purpose is to allow access to the elements of the row.
	struct DataPoint
	{
		size_t types;
		uint32_t length;
		Value** elements;
		Header* parent;

		//The address of the data array from the dataset, where the current row begins
		uint8_t *address;

		//Constructs a DataPoint with uint32_t elements (N-Tuple) and the types specified in Header*
		DataPoint(uint32_t, Header*);

		//Accesses the value at the index given in the current row
		Value& operator[](uint32_t);
		//Prints the DataPoint to the ostream
		friend std::ostream& operator<<(std::ostream& o, DataPoint& p);
	};

	//This struct holds an 2D-Table as an continuous array. 
	//It allows access via bracket operator and can store different types of values (See struct Value for more detail).
	struct DataSet
	{
		//The header of the data containing names and data types for each column
		Header * head;
		//A Point struct to allow two bracket operator accessing. This selects the row
		DataPoint * point;
		//The raw data format as unsigned chars
		uint8_t * data;
		//The title of the DataSet
		std::string name = "DataSet";
		//helper variables (insertindex is deprected and datapointcount not working)
		size_t datapointsize, datapointcount, insertindex, capacity;

		//TODO datapointcount adapting

		//Constructor with default rowcapcity of 1000 elements
		DataSet(Header * head, uint32_t cap = 1000);
		~DataSet();

		//DataSet& operator+(const DataPoint&);
		//bool operator+=(const DataPoint&);

		//DataSet& operator+(const DataSet&);
		//bool operator+=(const DataSet&);

		//Access a specific row (DataPoint)
		//Sets the address pointer of DataPoint *point the the specified index (transfered from rownumber to array index)
		DataPoint& operator[](size_t);
		//Output the DataSet to an outstream (e.g. Console)
		friend std::ostream& operator<<(std::ostream& o, DataSet& p);
	};
}