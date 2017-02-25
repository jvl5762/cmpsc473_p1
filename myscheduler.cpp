//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"
#include <algorithm> 

// global thread buffer that contains all created threads
ThreadBuffer buffer;

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
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch(policy)
	{
		case FCFS:		//First Come First Serve
			// sort buffer by arrival time
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByArrivalTime);
			//ThreadBuffer::iterator it = buffer.begin()

			// iterate through buffer and dispatch
			for (ThreadBuffer::iterator it = buffer.begin(); it != buffer.end(); ++it) {

				while(it->remaining_time != 0) {
					buffer.erase(std::remove(buffer.begin(), buffer.end(), CPUs[0]->tid), buffer.end());
					CPUs[0] = &(*it);
					return 1;
				}

 			}
 			//while (!buffer.empty()) {
 			//	for (unsigned int i = 0; i < num_cpu && it != buffer.end(); i++) {
 			//		if (CPUs[i] != NULL) {
 			//			if (CPUs[i].remaining_time == 0) {
 			//				buffer.erase(std::remove(buffer.begin(), buffer.end(), CPUs[i].tid), buffer.end());
 			//				CPUs[i] = &(*it++);
 			//			}
 			//		}
 			//		else {
 			//			CPUs[i] = &(*it++);
 			//		}
 			//	}
 			//}
 			return 0;
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