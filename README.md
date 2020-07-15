# Bank-Tellers-Simulation
Simulation of Multi-Core CPU Scheduling Algorithm thorough banking system application

	- allocation of customers dependes on number of waiting customers on queue.
	- if a queue has less number of customers then the customer will automatically leave its queue and move to the other queue.
	
the user enters the data for customers 
	- enter the arrival time.
	- enter the execution time for each employee.
	- choose to enter another customer or not.
	
after finishing by enter (n).
	the program starts to work for each unit time:
	- the program displays the waiting list at the current unit time as arrival_time:execution_time
	- the program displays the 3 queues in this structure Q  x:y:z || c1_data - c2_data - ...
			as.
			x: number of served customers by this queue
			y: number of current waiting customers on the queue
			z: total transaction time on the queue
			c1_data: (arrival_time:execution_time)
	- the program displays the served list of for each customer as
	     c1-data - c2_data- ....
  	     c1_data:arrival_Time:Execution_Time:Finish_Time:Start_Service_Time:Waiting_Time

after this the program displays 
	1-) the total bank reports
	2-) each teller reports
			
