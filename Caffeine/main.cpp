#include <cstdio>
#include <iostream>
#include <limits>
#include <ctime>

#include "com\data\dataset\DataSet.h"


void enter_to_return(double);


clock_t exec_time;

int main()
{
	exec_time = clock();

	//CODE GOES HERE

	Header * h = new Header(new std::string[3]{ "Name","Gewicht","Grösse" }, new int[3]{ STR,F64,F32 }, 3);
	Tuple * t = new Tuple(3);
	Data * d = new Data(h, 3);





	Value * n = new Str("Leo");
	Value * w = new Float64(110.5);
	Value * s = new Float32(187.3f);
	
	t->set(0, n);
	t->set(1, w);
	t->set(2, s);
	


	d->pushback(*t);

	((Str*)n)->x = "Julian";
	((Float64*)w)->x = 82.5;
	((Float32*)s)->x = 172.1f;

	d->pushback(*t);



	d->print();
	enter_to_return((clock() - exec_time) / (CLOCKS_PER_SEC / 1000.0));
	return 0;
}


void enter_to_return(double ms)
{
	printf("Execution time: %f ms\nPress Enter to Continue...\n", ms);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}