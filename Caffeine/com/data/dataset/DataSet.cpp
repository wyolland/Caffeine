#include "DataSet.h"

namespace DATA
{
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
		case F32: o << (float)v; break;
		case F64: o << (double)v; break;
		case I32: o << (int)v; break;
		case I64: o << (int64_t)v; break;
		case STR: o << (std::string)v; break;
		}
		return o;
	}




	Header::Header(std::string * Names, uint8_t * Types, uint32_t Length)
	{
		n = Length;
		names = Names;
		types = Types;
	}

	Header::~Header()
	{

	}




	DataPoint::DataPoint(uint32_t n, Header * h)
	{
		length = n;
		parent = h;
		elements = (Value**)malloc(n * sizeof(Value*));
	}

	Value& DataPoint::operator[](uint32_t i)
	{
		if (elements[0]->address != address)
		{
			size_t offset = 0;
			for (uint32_t j = 0; j < length; j++)
			{
				elements[j]->address = address + offset;
				switch (parent->types[j])
				{
				case F32: offset += sizeof(float); break;
				case F64: offset += sizeof(double); break;
				case I32: offset += sizeof(int); break;
				case I64: offset += sizeof(int64_t); break;
				case STR: offset += sizeof(uint64_t); break;
				}
			}
		}
		return *elements[i];
	}

	std::ostream & operator<<(std::ostream & o, DataPoint & p)
	{
		for (uint32_t i = 0; i < p.length; i++)
			o << p[i] << "\t";
		return o;
	}




	DataSet::DataSet(Header * head, uint32_t cap)
	{
		datapointsize = 0;
		datapointcount = 0;
		insertindex = 0;
		capacity = cap;
		point = new DataPoint(head->n, head);
		for (size_t i = 0; i < head->n; i++)
		{
			switch (head->types[i])
			{
			case F32: point->elements[i] = new Float32(); datapointsize += sizeof(float); break;
			case F64: point->elements[i] = new Float64(); datapointsize += sizeof(double); break;
			case I32: point->elements[i] = new Int32(); datapointsize += sizeof(int); break;
			case I64: point->elements[i] = new Int64(); datapointsize += sizeof(int64_t); break;
			case STR: point->elements[i] = new Str(); datapointsize += sizeof(uint64_t); break;
			}
		}
		data = (uint8_t*)malloc(capacity*datapointsize * sizeof(uint8_t));
		this->head = head;
	}

	DataSet::~DataSet()
	{
	}

	DataPoint& DataSet::operator[](size_t n)
	{
		point->address = &data[n*datapointsize];
		return *point;
	}

	std::ostream & operator<<(std::ostream & o, DataSet & p)
	{
		o << "Printing " << p.name << std::endl
			//<< "Number of Datapoints: " << p.datapointcount << std::endl
			//Maybe eyport the following to header and overload stream operator
			<< "Number of attributes: " << p.point->length << std::endl;

		size_t width = 0;
		for (size_t i = 0; i < p.head->n; i++)
			width += p.head->names[i].length() + 8 - (p.head->names[i].length() % 8);

		for (size_t i = 0; i < width; i++)
			o << "-";
		o << std::endl;

		for (size_t i = 0; i < p.head->n; i++)
			o << p.head->names[i] << "\t";
		o << std::endl;

		for (size_t i = 0; i < width; i++)
			o << "_";
		o << std::endl;

		for (size_t i = 0; i < 4; i++)
			o << p[i] << std::endl;

		//o << p[999999] << std::endl;
		//o << p[1] << std::endl; 
		//o << "..." << std::endl;
		//o << p[p.datapointcount] << std::endl;

		for (size_t i = 0; i < width; i++)
			o << "-";

		return o;
	}
}