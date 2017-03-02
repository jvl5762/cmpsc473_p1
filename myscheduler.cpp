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

	int arriving_time[10], remaining_time[10], endtime, i, priority, process_id, n;
	int remain = 0, time, wt = 0, tat = 0;	//wt is waiting time, and tat is turnaround time
	//n = 5; //number of process

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
			remaining_time[9] = 9999;
			for (time = 0; remain != n; time++)
			{
				remaining_time[i]--;
				if (remaining_time[i] == 0)
				{
					remain++;
					endtime = time + 1;
					process_id = i + 1;	//number of process id
					wt += endtime - remaining_time[i] - arriving_time[i];	//total waiting time
					tat += endtime - arriving_time[i];	//total turnaround time
				}
			}
			break;
		case STRFwP:	//Shortest Time Remaining First, with preemption
			remaining_time[9] = 9999;	//set the last element of remaining time array as 9999
			for (time = 0; remain != n; time++)
			{
				priority = 9;	//set priority to 9
				for (i = 0; i < n; i++)
				{
					if (arriving_time[i] <= time&&remaining_time[i] < remaining_time[priority] && remaining_time[i]>0)
					{
						priority = i;	//set priority as i;
					}
				}
				remaining_time[priority]--;
				if (remaining_time[priority] == 0)
				{
					remain++;
					endtime = time + 1;
					process_id = priority + 1;	//number of process id
					wt += endtime - remaining_time[priority] - arriving_time[priority];	//total wait time
					tat += endtime - arriving_time[priority];	//total turnaround time
				}
			}
			break;
		case PBS:		//Priority Based Scheduling, with preemption
		{
			int arrived = 0;			// number of threads that have arrived	
			int lowest_priority = -1;	// lowest priority thread that is currently running
			int cpu_low = -1;			// cpu that is running the lowest priority thread
			bool cpu_available = 0 ;			// available cpu

			// sort the buffer by arrival time
			std::sort(buffer.begin(), buffer.end(), MyScheduler::CompareByArrivalTime);

			for (int i = 0; i <= timer && (unsigned int) i < buffer.size(); i++) {
				if (buffer[i].thread.arriving_time <= timer)
					arrived++;
			}

			// sort threads that have arrived by priority
			std::sort(buffer.begin(), buffer.begin()+arrived, MyScheduler::CompareByPriority);

			for (std::vector<ThreadBuffer>::iterator it_eraser = buffer.begin(); it_eraser != buffer.end(); it_eraser++) {
				if (it_eraser->thread.remaining_time == 0)
					buffer.erase(it_eraser);
				if (buffer.empty())	// quit when all threads have finished
					return 0;
			}

			// assign each CPU a thread
			// iterate through the list of threads
			for (unsigned int thread_num = 0; buffer[thread_num].thread.arriving_time <= timer && thread_num < num_cpu && thread_num < buffer.size(); thread_num++) {
				// if cpu has has not finished processing a thread, assign the cpu the same thread
				if (buffer[thread_num].processing_cpu != -1)
					CPUs[buffer[thread_num].processing_cpu] = &(buffer[thread_num].thread);
				else {
					// if thread is unassigned, find an available cpu to process
					for (unsigned int cpu_num = 0; cpu_num < num_cpu  && !cpu_available; cpu_num++) {
						cpu_available = 1;
						for (unsigned int i = 0; i < buffer.size() && cpu_available; i++) {
							if (buffer[i].processing_cpu == (int) cpu_num)
								cpu_available = 0;
						}
						if (cpu_available) {
							CPUs[cpu_num] = &(buffer[thread_num].thread);
							buffer[thread_num].processing_cpu = cpu_num;
						}
					}
					// if cpu was not found, switch with cpu that is processing the lowest priority thread
					if (buffer[thread_num].processing_cpu == -1) {
						for (unsigned int i = 0; i < buffer.size(); i++) {
							if (buffer[i].processing_cpu != -1 && buffer[i].thread.priority > lowest_priority) {
								lowest_priority = buffer[i].thread.priority;
								cpu_low = buffer[i].processing_cpu;
							}
						}
						for (unsigned int i = 0; i < buffer.size(); i++) {
							if (buffer[i].processing_cpu == cpu_low) {
								buffer[i].processing_cpu = -1;
								break;
							}
						}
						CPUs[cpu_low] = &(buffer[thread_num].thread);
						buffer[thread_num].processing_cpu = cpu_low;
					}
				}
			}
			if (timer == 17)
				return 0;
			return 1;
			break;
		}
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

// helper function that sorts thread buffer by arrival_time 
bool MyScheduler::CompareByPriority(const ThreadBuffer &a, const ThreadBuffer &b)
{
	return (a.thread.priority < b.thread.priority);
}