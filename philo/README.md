# 42's Philosophers Notes

-  Even NumOfPhilos => time_to_die = time_to_eat + time_to_sleep + 10
-  Odd NumOfPhilos => time_to_die = 2*time_to_eat + time_to_sleep + 10

How to mitigate Deadlocks and Dying:
1) Make half of the Philosophers sleep by the start of the program. F.ex. take
   all philos with odd num to sleep tte/2
2) 
