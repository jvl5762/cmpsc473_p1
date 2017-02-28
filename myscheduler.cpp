//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"
#include <algorithm> 

// global thread buffer that contains all created threads
ThreadBuffer buffer;
ThreadBuffer::iterator it;

//Function to Create Thread(s) and insert them in the student
//defined data structure
void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	ThreadDescriptorBlock new_thread;

	new_thread.arriving_time = arriving_time;
	new_thread.remaining_time = remaining_time;
	new_thread.priority = priority;
	new_thread.tid = tid;

	buffer.push_back(new_thread);
	it = buffer.begin();
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch(policy)
	{
		case FCFS:		//First Come First Serve
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByArrivalTime);
			for (ThreadBuffer::iterator it_eraser = buffer.begin(); it_eraser != buffer.end(); it_eraser++) {
				if (it_eraser->remaining_time == 0)
					buffer.erase(it_eraser);
				if (buffer.empty())
					return 0;
			}
			for (unsigned int i = 0; i < num_cpu; i++) {
				if (CPUs[i] == NULL && i < buffer.size()) {
					CPUs[i] = &(buffer[i]);
					//for (unsigned int j = 0; j < num_cpu && CPUs[i] != NULL && CPUs[j] != NULL; j++) {
					//	if (i != j)
					//		printf("if %d == %d ", CPUs[i]->tid, CPUs[j]->tid);
					//	if (i != j && CPUs[i]->tid == CPUs[j]->tid) {
					//		printf("then here\n");
					//		if (num_cpu < buffer.size())
					//			CPUs[i] = &(buffer[num_cpu]);
					//		else
					//			CPUs[i] = NULL;
					//	}
					//}
				}
			}
			return 1;
			break;
		case STRFwoP:	//Shortest Time Remaining First, without preemption

			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption

			break;
		case PBS:		//Priority Based Scheduling, with preemption

			break;
		default :
			cout<<"Invalid policy!";
			throw 0;
	}
	return true;
}

// helper function that sorts thread buffer by arrival_time 
bool MyScheduler::CompareByArrivalTime(const ThreadDescriptorBlock &a, const ThreadDescriptorBlock &b)
{
	return (a.arriving_time < b.arriving_time);
}