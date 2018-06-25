#include "DataSet_2.h"


void Value::operator=(float v)
{
	if (t == F32) { ((Float32*)this)->x = v; } 
	else { throw - 1; }
}

void Value::operator=(double v)
{
	if (t == F64) { ((Float64*)this)->x = v; }
	else { throw - 1; }
}

void Value::operator=(int v)
{
	if (t == I32) { ((Int32*)this)->x = v; } 
	else { throw - 1; }
}

void Value::operator=(int64_t v)
{
	if (t == I64) { ((Int64*)this)->x = v; } 
	else { throw - 1; }
}

void Value::operator=(std::string v)
{
	if (t == STR) { ((Str*)this)->x = v; } 
	else { throw - 1; }
}

std::ostream & operator<<(std::ostream & o, Value & v)
{
	switch (v.t)
	{
	case F32: o << ((Float32*)&v)->x; break;
	case F64: o << ((Float64*)&v)->x; break;
	case I32: o << ((Int32*)&v)->x; break;
	case I64: o << ((Int64*)&v)->x; break;
	case STR: o << ((Str*)&v)->x; break;
	}
	return o;
}



Header::Header(std::string * Names, uint8_t * Types, size_t Length)
{
	n = Length;
	names = Names;
	types = Types;
}



DataPoint::DataPoint(uint32_t n, void* d)
{
	length = n;
	parent = d;
	elements = (Value**)malloc(n * sizeof(Value*));
}

Value& DataPoint::operator[](uint32_t i)
{	
	((DataSet*)parent)->commit(); //TODO this is messed up shit... Too much memory accessing. Make it light
	return *elements[i];
}

std::ostream & operator<<(std::ostream & o, DataPoint & p)
{
	for (size_t i = 0; i < p.length; i++)
	{
		o << p[i] << "\t";
	}
	return o;
}





DataSet::DataSet(Header * head, uint32_t cap)
{
	//Is pointer so operator[] wich accesses point will not work
	//calculate datapoint size
	datapointsize = 0;
	datapointcount = 0;
	insertindex = 0;
	capacity = cap;
	point = new DataPoint(head->n, this);
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

void DataSet::commit()
{
	if (datapointcount < capacity)
	{
		uint32_t offset = 0;
		for (size_t i = 0; i < head->n; i++)
		{
			uint64_t a;
			switch (head->types[i])
			{
			case F32:
				memcpy(&data[insertindex + offset], (uint8_t*)(&(((Float32*)point->elements[i])->x)), sizeof(float));
				offset += sizeof(float);
				break;
			case F64:
				memcpy(&data[insertindex + offset], (uint8_t*)(&(((Float64*)point->elements[i])->x)), sizeof(double));
				offset += sizeof(double);
				break;
			case I32:
				memcpy(&data[insertindex + offset], (uint8_t*)(&(((Int32*)point->elements[i])->x)), sizeof(int));
				offset += sizeof(int);
				break;
			case I64:
				memcpy(&data[insertindex + offset], (uint8_t*)(&(((Int64*)point->elements[i])->x)), sizeof(int64_t));
				offset += sizeof(int64_t);
				break;
			case STR:
				a = (uint64_t)new std::string(((Str*)point->elements[i])->x.c_str());
				memcpy(&data[insertindex + offset], (uint8_t*)&a, sizeof(uint64_t));
				offset += sizeof(uint64_t);
				break;
			}
		}
	}
	else
	{
		throw - 1;
	}

}

DataSet & DataSet::operator+(const DataPoint &)
{
	// TODO: hier Rückgabeanweisung eingeben
	return *this;
}

bool DataSet::operator+=(const DataPoint &)
{
	return false;
}

DataSet & DataSet::operator+(const DataSet &)
{
	// TODO: hier Rückgabeanweisung eingeben
	return *this;
}

bool DataSet::operator+=(const DataSet &)
{
	return false;
}

DataPoint& DataSet::operator[](size_t n)
{
	size_t offset = 0;
	
	//only if the index changes reassign the values
	if (insertindex != datapointsize*n)
	{
		//commit();
		insertindex = datapointsize*n;
		for (size_t i = 0; i < head->n; i++)
		{
			switch (head->types[i])
			{
			case F32: (*point)[i] = *(float*)&data[n*datapointsize + offset];						offset += sizeof(float);	break;
			case F64: (*point)[i] = *(double*)&data[n*datapointsize + offset];						offset += sizeof(double);	break;
			case I32: (*point)[i] = *(int*)&data[n*datapointsize + offset];							offset += sizeof(int);		break;
			case I64: (*point)[i] = *(int64_t*)&data[n*datapointsize + offset];						offset += sizeof(int64_t);	break;
			case STR: (*point)[i] = *((std::string*)(*(uint64_t*)&data[n*datapointsize + offset])); offset += sizeof(uint64_t); break;
			}
		}
	}
	return *point;
}

std::ostream & operator<<(std::ostream & o, DataSet & p)
{
	o << "Printing " << p.name << std::endl
		<< "Number of Datapoints: " << p.datapointcount << std::endl
		//Maybe eyport the following to header and overload stream operator
		<< "Number of attributes: " << p.point->length << std::endl
		<< "----------------------------------------" << std::endl;
		for (size_t i = 0; i < p.head->n; i++)
			o << p.head->names[i] << "\t";
		o << "\n________________________________________\n" << std::endl
			<< p[0] << "\n" << p[1] << "\n" << "...\n" << p[p.datapointcount] << std::endl
			<< "----------------------------------------" << std::endl;

	return o;
}