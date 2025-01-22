# Philosophers Project: Overview
The Philosophers project is an engaging simulation designed to test concurrency management using threads and mutexes in C.

## Dining Philosophers Problem
This project demonstrates a classic problem in computer science often referred to as the "Dining Philosophers Problem." Philosophers alternate between eating, thinking, and sleeping. To eat, a philosopher needs two forks: one on their left and one on their right. They must avoid starving and minimize contention over shared resources (forks).
 
### Key Concepts
- **Threads**: parts of the program responsible for performing specific tasks. Each philosopher in this simulation is represented as a thread, working independently to eat, sleep and think.
- **Concurrency**: multiple tasks running at the same time while sharing resources. In this project, the philosophers represent tasks that need to share forks. Concurrency ensures that philosophers coordinate their actions so that no two philosophers attempt to grab the same fork simultaneously.
- **Mutexes (Mutual Exclusion)**: tools used in multithreaded programs to manage access to shared resources and prevent race conditions. A race condition occurs when multiple threads access shared data simultaneously, leading to unexpected results. By locking a mutex, a thread gains exclusive access to a resource, ensuring no other thread can use it until the mutex is unlocked.
- **Deadlocks**: occur when two or more threads are stuck waiting for each other, preventing any of them from proceeding. In this simulation, a deadlock could happen if every philosopher picks up one fork and waits indefinitely for the other fork to become available. Proper logic and synchronization prevent this situation.

## Program Features
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
- **number_of_philosophers**: Total number of philosophers and forks.
- **time_to_die**: Time (ms) before a philosopher dies if they don’t eat.
- **time_to_eat**: Time (ms) a philosopher spends eating.
- **time_to_sleep**: Time (ms) a philosopher spends sleeping.
- **[number_of_times_each_philosopher_must_eat] (optional)**: Stops simulation if all philosophers eat this many times.

## Implementation Notes
### Threads and Mutexes:
- Each philosopher is represented as a thread.
- Mutexes protect forks to prevent race conditions.
### Global Variables:
- Forbidden by project rules. All shared data is managed through structures passed to threads.
### Accuracy:
- Death messages must be logged no later than 10ms after the actual death.
- State messages are synchronized to avoid overlapping.

## Rules and Constraints
### No Data Races:
- Ensure proper synchronization using mutexes.
### Forks Management:
- Each fork is shared between two philosophers, requiring careful mutex usage.
### Philosopher Behavior:
- Philosophers think, eat, and sleep in loops, avoiding starvation.
### Error Handling:
- Handles invalid arguments (non-numeric, out of range, or missing).
- Ends the simulation immediately if number_of_times_each_philosopher_must_eat == 0.
