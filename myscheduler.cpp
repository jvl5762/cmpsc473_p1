////////////////////////////////////////////////////////////////////////////////
//
//  File           : myschedule.cpp
//  Description    : Defines all the functions in 'myschedule.h'
//					 A thread based scheduler
//
//  Author         : Jeff Li, Frank Guo, Chen Dikan
//  Last Modified  : 03/04/2017
//

#include "myscheduler.h"
#include <algorithm>
#include <vector>

// global student-defined thread buffer that contains all created threads
std::vector<ThreadBuffer> buffer;

////////////////////////////////////////////////////////////////////////////////
//
// Function     : CreateThread
// Description  : Function to Create Thread(s) and insert them in the student
//				  defined data structure
//
// Inputs       : arriving_time - time the thread arrives
//                remaining_time - amount of time left to process a thread
//				  priority - priority level of a thread - lower is greater priority
//				  tid - thread id
// Outputs      : none
void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	ThreadBuffer new_thread;

	new_thread.thread.arriving_time = arriving_time;
	new_thread.thread.remaining_time = remaining_time;
	new_thread.thread.priority = priority;
	new_thread.thread.tid = tid;
	new_thread.processing_cpu = -1;	// thread has not begun processing yet

	buffer.push_back(new_thread);	// new threads are pushed to the back of data struct
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : Dispatch
// Description  : Check and remove finished threads
//				  Decide which threads are in CPUs to be processed for
//				  next clock cycle according to different policies
//				  idea: sort buffer by policy standards, then assign the first
//					    CPU-number of threads that have arrived
//
// Inputs       : none
// Outputs      : none
bool MyScheduler::Dispatch()
{
	// Check and remove finished threads;
	// Check if all the threads are finished; if so, return false
	if (!MyScheduler::RemoveFinishedThreads()) return false;

	Sorting();	// sort the threads according to policy

	switch(policy)
	{
		case FCFS:	case STRFwoP:	// First Come First Serve and Shortest Time Remaining First, without preemption
		{
			for (unsigned int i = 0; i < num_cpu; i++) {			// assign each CPU a thread
				for (unsigned int j = 0; j < buffer.size(); j++) {	// iterate through the list of threads
					// if cpu has not finished processing a thread, assign the same thread
					// else if cpu is unassigned, find a thread for the cpu to process
					if (buffer[j].processing_cpu == (int) i) {
						CPUs[i] = &(buffer[j].thread);
						break;
					}
					else if (CPUs[i] == NULL && buffer[j].processing_cpu == -1 && buffer[j].thread.arriving_time <= timer) {
						CPUs[i] = &(buffer[j].thread);
						buffer[j].processing_cpu = i;
						break;
					}
				}
			}
			break;
		}
		case STRFwP: case PBS:	//Shortest Time Remaining First, with preemption and Priority Based Scheduling, with preemption
		{
			// reset CPUs
			for (unsigned i = 0; i < buffer.size(); i++)
				buffer[i].processing_cpu = -1;

			for (unsigned int i = 0; i < num_cpu; i++) {			// assign each CPU a thread
				for (unsigned int j = 0; j < buffer.size(); j++) {	// iterate through the list of threads
					// iterate through buffer, which is already in ascending order by remaining_time/priority
					// if the thread has arrived, assign cpu to that thread
					if (buffer[j].processing_cpu == -1 && buffer[j].thread.arriving_time <= timer) {
						CPUs[i] = &(buffer[j].thread);
						buffer[j].processing_cpu = i;
						break;
					}
					else
						CPUs[i] = NULL;
				}
			}
			break;
		}
		default :
			cout<<"Invalid policy!";
			throw 0;
	}

	MyScheduler::Display();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : RemoveFinishedThreads
// Description  : Remove finished threads and check if buffer is empty
//
// Inputs       : none
// Outputs      : 1 if there are still threads to process, 0 if buffer is empty
int MyScheduler::RemoveFinishedThreads() 
{
	for (std::vector<ThreadBuffer>::reverse_iterator it_eraser = buffer.rbegin(); it_eraser != buffer.rend(); it_eraser++) {
		if (it_eraser->thread.remaining_time == 0) 
			buffer.erase(std::next(it_eraser).base());
		if (buffer.empty())	// quit when all threads have finished
			return 0;
	}
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : Sorting
// Description  : Sort through all threads so that the buffer is in an order
//				  according to policy
//
// Inputs       : none
// Outputs      : none
void MyScheduler::Sorting() 
{
	switch(policy)
	{
		case FCFS:
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByArrivalTime);
			break;
		case STRFwoP: case STRFwP:
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByRemainingTime);
			break;
		case PBS:
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByPriority);
			break;

		//if (sorted == 0) {
		//	printf("InitialSorting: ");
		//	for (unsigned int i = 0; i < buffer.size(); ++i)
		//		printf("%d[%d] ", buffer[i].thread.tid, buffer[i].thread.priority);
		//	printf("\n");
		//}
	}
}

// helper function that displays the current thread pool
void MyScheduler::Display() 
{
	printf("pool: ");
	for (unsigned int i = 0; i < buffer.size(); ++i) {
		if (buffer[i].thread.arriving_time <= timer)
			printf("%d[%d] ", buffer[i].thread.tid, buffer[i].thread.priority);
	}
	printf("\n");
}

// helper function that sorts thread buffer by arrival_time 
bool MyScheduler::CompareByArrivalTime(const ThreadBuffer &a, const ThreadBuffer &b)
{
	return (a.thread.arriving_time < b.thread.arriving_time);
}

// helper function that sorts thread buffer by arrival_time 
bool MyScheduler::CompareByRemainingTime(const ThreadBuffer &a, const ThreadBuffer &b)
{
	if (a.thread.remaining_time == b.thread.remaining_time)
		return (a.thread.arriving_time < b.thread.arriving_time);
	else
		return (a.thread.remaining_time < b.thread.remaining_time);
}

// helper function that sorts thread buffer by arrival_time 
bool MyScheduler::CompareByPriority(const ThreadBuffer &a, const ThreadBuffer &b)
{
	if (a.thread.priority == b.thread.priority)
		return (a.thread.arriving_time < b.thread.arriving_time);
	else
		return (a.thread.priority < b.thread.priority);
}