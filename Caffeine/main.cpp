#include <cstdio>
#include <iostream>
#include <limits>
#include <ctime>

#include "com\data\dataset\DataSet.h"

//Define
#define DATASET_EXAMPLE



void enter_to_return(double);


clock_t exec_time;

int main()
{
	exec_time = clock();


	//CODE GOES HERE
#ifdef DATASET_EXAMPLE
	Header * h = new Header(new std::string[3]{ "Name","Gewicht","Grösse" }, new int[3]{ STR,F64,F32 }, 3);
	Tuple * t = new Tuple(3);
	Data * d = new Data(h, 100000);





	Value * n = new Str("Leo");
	Value * w = new Float64(110.5);
	Value * s = new Float32(187.3f);
	
	t->set(0, n);
	t->set(1, w);
	t->set(2, s);
	


	d->pushback(*t);

	for (int i = 1; i < 10000; i++)
	{
		((Str*)n)->x = "Julian";
		((Float64*)w)->x = 82.5 + 1.0/i;
		((Float32*)s)->x = 172.1f + i/100.0;

		d->pushback(*t);
	}



	d->print();
#endif



#ifdef MLP_EXAMPLE
	printf("MLP");
#endif

	enter_to_return((clock() - exec_time) / (CLOCKS_PER_SEC / 1000.0));
	return 0;
}


void enter_to_return(double ms)
{
	printf("Execution time: %f ms\nPress Enter to Continue...\n", ms);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}