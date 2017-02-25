# cmpsc473_p1

what i added:
  data structure
    defined at the top of myscheduler.h
    used a vector structure because vectors have a lot of good functionality
    it is declared at the top of myscheduler.cpp as "buffer"

  workbench
    the workbench should be sufficient, we can always create more complex thread combinations
    found out that we call Go(), not Dispatch()

  FCFS
    i'm still working on this policy - 50% finished
    my issue is how to handle multiple CPUs
  
overview on how the program works
  so after all the threads are created, main calls Go() to start the scheduling
  Go() is looped and calls Dispatch() on every iteration
  have Dispatch return 1 in order to have a  CPU work on a thread
  in Dispatch(), we determine which element in our buffer is assigned a CPUs (which is defined in scheduler.h line 43)
  Go() loops until Dispatch() returns 0
