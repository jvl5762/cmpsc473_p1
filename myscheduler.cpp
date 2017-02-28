//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"
#include <algorithm>
#include <vector>

// global thread buffer that contains all created threads
std::vector<ThreadBuffer> buffer;

//Function to Create Thread(s) and insert them in the student
//defined data structure
void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	ThreadBuffer new_thread;

	new_thread.thread.arriving_time = arriving_time;
	new_thread.thread.remaining_time = remaining_time;
	new_thread.thread.priority = priority;
	new_thread.thread.tid = tid;
	new_thread.processing_cpu = -1;	// flag that thread has not begun processing yet

	buffer.push_back(new_thread);
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch(policy)
	{
		case FCFS:		//First Come First Serve
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByArrivalTime);	// sort the buffer by arrival time
			// destroy any finished threads with vector::erase
			// vector::erase shifts the remaining elements to the right which affects memory locations
			// so below: we use processing_cpu to allow CPUs to continue processing the correct thread
			for (std::vector<ThreadBuffer>::iterator it_eraser = buffer.begin(); it_eraser != buffer.end(); it_eraser++) {
				if (it_eraser->thread.remaining_time == 0)
					buffer.erase(it_eraser);
				// quit when all threads have finished
				if (buffer.empty())
					return 0;
			}
			// assign each CPU a thread
			for (unsigned int cpu_num = 0; cpu_num < num_cpu; cpu_num++) {
				// iterate through the list of threads
				for (unsigned int thread_num = 0; thread_num < buffer.size(); thread_num++) {
					// if cpu has has not finished processing a thread, assign the cpu the same thread
					if (buffer[thread_num].processing_cpu == (int) cpu_num)
						CPUs[cpu_num] = &(buffer[thread_num].thread);
					// if cpu is unassigned, find an available thread for the cpu to process
					else if (CPUs[cpu_num] == NULL && buffer[thread_num].processing_cpu == -1 && buffer[thread_num].thread.arriving_time <= timer) {
						CPUs[cpu_num] = &(buffer[thread_num].thread);
						buffer[thread_num].processing_cpu = cpu_num;
					}
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
bool MyScheduler::CompareByArrivalTime(const ThreadBuffer &a, const ThreadBuffer &b)
{
	return (a.thread.arriving_time < b.thread.arriving_time);
}