// Project1.cpp : Defines the entry point for the console application.
//

#include "myscheduler.h"
#include <ctime> 

#define CPU_NUM 		4
#define THREAD_AMOUNT 	20
#define ARRIVAL_LIMIT 	30
#define BURST_LIMIT 	30
#define PRIORITY_LIMIT 	10

// functional prototypes
void randomScenario(MyScheduler &s, int thread_amount, int arrival_limit, int burst_limit, int priority_limit);
void scenario0 (MyScheduler &s);

int main(int argc, char* argv[])
{
	std::srand(std::time(0));

	// declare schedulers
	MyScheduler s_FCFS(FCFS, CPU_NUM);
	MyScheduler s_STRFwoP(STRFwoP, CPU_NUM);
	MyScheduler s_STRFwP(STRFwP, CPU_NUM);
	MyScheduler s_PBS(PBS, CPU_NUM);

	randomScenario(s_FCFS, THREAD_AMOUNT, ARRIVAL_LIMIT, BURST_LIMIT, PRIORITY_LIMIT);
	s_FCFS.Go();

	//randomScenario(s_STRFwoP, THREAD_AMOUNT, ARRIVAL_LIMIT, BURST_LIMIT, PRIORITY_LIMIT);
	//s_STRFwoP.Go();

	//randomScenario(s_STRFwP, THREAD_AMOUNT, ARRIVAL_LIMIT, BURST_LIMIT, PRIORITY_LIMIT);
	//s_STRFwP.Go();

	randomScenario(s_PBS, THREAD_AMOUNT, ARRIVAL_LIMIT, BURST_LIMIT, PRIORITY_LIMIT);
	s_PBS.Go();

	return 0;
}

// generates a random thread combination
void randomScenario( MyScheduler &s, int thread_amount, int arrival_limit, int burst_limit, int priority_limit) {
	int arrival_time, remaining_time, priority, tid;

	for (int i = 0; i < thread_amount; i++) {
		arrival_time = rand()%arrival_limit;
		remaining_time = rand()%burst_limit;
		priority = rand()%priority_limit;
		tid = i;

		s.CreateThread(arrival_time, remaining_time, priority, tid);

		printf("created (%d,%d,%d,%d)\n", arrival_time, remaining_time, priority, tid);
	}
}

void scenario0 (MyScheduler &s) {
	s.CreateThread(2, 6, 2, 2);
	s.CreateThread(1, 5, 1, 1);
	s.CreateThread(0, 4, 0, 0);
	s.CreateThread(3, 7, 3, 3);
}
