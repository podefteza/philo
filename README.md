# Philosophers Project: Overview
The Philosophers project is an engaging simulation designed to test concurrency management using threads and mutexes in C. It demonstrates a classic problem in computer science often referred to as the "Dining Philosophers Problem."

## Key Concepts
### Dining Philosophers Problem:
- Philosophers alternate between eating, thinking, and sleeping.
- To eat, a philosopher needs two forks: one on their left and one on their right.
- Philosophers must avoid starving and minimize contention over shared resources (forks).

### Simulation Details:
- Each philosopher:
  - Starts as a thread.
  - Uses mutexes to acquire and release forks.
- The simulation ends when:
  - A philosopher dies due to starvation (hasn't eaten within time_to_die milliseconds).
  - All philosophers have eaten at least number_of_times_each_philosopher_must_eat times (if specified).
 
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
