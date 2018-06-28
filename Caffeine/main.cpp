#include <cstdio>
#include <iostream>
#include <limits>
#include <ctime>
#include <cmath>

#include "com\data\dataset\DataSet_3.h"

//Define
//#define DATASET_EXAMPLE
#define MLP_EXAMPLE


void enter_to_return(double);


clock_t exec_time;

int main()
{
	exec_time = clock();

	//CODE GOES HERE


#ifdef DATASET_EXAMPLE
	Header* h = new Header(new std::string[4]{ "Index", "Name", "Value",  "Label" }, new uint8_t[4]{ I32, STR, F64,STR }, 4);
	DataSet D(h,10);
	
	
	//Takes about 5 sec to assign 3000000 (1M*3)values -> Benchmark
	for (int i = 0; i < 10; i++)
	{
		D[i][0] = i;
		D[i][1] = i % 2 ? "Leander" : "Julian";
		D[i][2] = 0.1*i;
		D[i][3] = i%2?"A":"B";
		//std::cout << D[i] << std::endl;
	}

	//int i = D[2][0];
	std::cout << D << std::endl;

	
	
#endif



#ifdef MLP_EXAMPLE
	//printf("MLP training XOR...\n");
	Header * h = new Header(new std::string[3]{ "A","B","Label" }, new uint8_t[3]{ I32,I32,STR }, 3);
	DataSet D(h, 4);
	D.name = "XOR Dataset";
	for (size_t i = 0; i < 4; i++)
	{
		D[i][0] = (i/2) >= 1 ? 1:0;
		D[i][1] = i % 2 ? 1 : 0;
	}

	D[0][2] = "False";
	D[1][2] = "True";
	D[2][2] = "True";
	D[3][2] = "False";

	std::cout << D << std::endl;
#endif

	enter_to_return((clock() - exec_time) / (CLOCKS_PER_SEC / 1000.0));
	return 0;
}


void enter_to_return(double ms)
{
	printf("Execution time: %f ms\nPress Enter to Continue...\n", ms);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}