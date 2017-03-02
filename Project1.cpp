// Project1.cpp : Defines the entry point for the console application.
//

#include "myscheduler.h"
#include <algorithm> 

#define scheduler_num 	4
#define cpu_num 		2

int main(int argc, char* argv[])
{
	MyScheduler s[scheduler_num] = { 	
		{FCFS, cpu_num},
		{STRFwoP, cpu_num},
		{STRFwP, cpu_num},
		{PBS, cpu_num}
	};

	s[0].CreateThread(2, 6, 2, 2);
	s[0].CreateThread(1, 5, 1, 1);
	s[0].CreateThread(0, 4, 0, 0);
	s[0].CreateThread(3, 7, 3, 3);

	//for(int i = 0; i < 4; i++) {
	//	s[i].Go();
	//}

	//s[0].Go();
	s[3].Go();

	return 0;
}

