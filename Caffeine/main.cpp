#include <cstdio>
#include <iostream>
#include <limits>
#include <ctime>


void enter_to_return(double);


clock_t exec_time;

int main()
{
	exec_time = clock();

	//CODE GOES HERE


	

	
	enter_to_return((clock() - exec_time) / (CLOCKS_PER_SEC / 1000.0));
	return 0;
}


void enter_to_return(double ms)
{
	printf("Execution time: %f ms\nPress Enter to Continue...\n", ms);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}