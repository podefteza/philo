# Philosophers Project 🍴🕰️  
A solution to Dijkstra's Dining Philosophers Problem using threads and mutexes in C.

## 📖 Overview  
This project simulates philosophers sitting around a table, alternating between eating, thinking, and sleeping. Each philosopher must use two forks to eat, leading to a classic concurrency problem. The solution ensures:  

- No deadlocks  
- No starvation  
- Thread-safe resource sharing  

## 🎯 Problem Statement  
Philosophers represent threads, and forks represent shared resources (mutexes). The challenge is to:  

- Prevent philosophers from starving.  
- Avoid deadlocks when acquiring forks.  
- Synchronize actions without data races.  

## 🛠️ Solution Approach  

### 🔑 Key Techniques  
- **Mutexes**: Protect forks and shared data (e.g., meal counts, timestamps).  
- **Staggered Start**: Even-numbered philosophers delay starting to reduce contention.  
- **Monitor Thread**: Continuously checks for philosopher starvation or completion.  

### 🔄 Deadlock Prevention  
#### **Fork Ordering:**  
- **Odd philosophers**:  Left fork 🍴👈 → 👉🍴 Right fork 
- **Even philosophers**: Right fork 👉🍴 → 🍴👈 Left fork

🔓 **Immediate Fork Release**: If the simulation stops (e.g., a philosopher dies), all forks are released.  

## 🚀 Features  

### 📌 Input Arguments  
``` bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [optional_times_to_eat]  
```
Example:  
``` bash
./philo 5 800 200 200 7  
```
Simulates **5 philosophers** who must each eat **7 times**. They take **200ms to eat** and after that, **200ms to sleep**. Then, they start thinking until they have both forks available again to eat. If **800ms** pass since the **start of the simulation** or since the **start of their last meal**, they die.

### 📄 Output  
``` bash
timestamp X has taken a fork
timestamp X is eating
timestamp X is sleeping
timestamp X is thinking
timestamp X died
```
The message of a philosopher's death needs to be displayed within 10ms of actual death.

## 🔍 Detailed Program Flow  

### 📝 Argument Validation  
- The program begins by validating the command-line arguments using `init_args()`.  
- This ensures that the arguments comply with the rules (e.g., positive integers within valid ranges).  
- If the optional `times_to_eat` argument is `0`, the simulation exits immediately since no philosopher is required to eat.  

### 🛠 Initialization  
- The `init_values()` function initializes the main setup structure, allocating memory and creating mutexes for shared resources like forks and control flags.  
- The `init_philos()` function initializes each philosopher, associating them with their respective forks and preparing individual states, such as meal counts and last meal timestamps.  
- Each philosopher is represented by a thread.  

### 🎭 Simulation Management  
- The `run_simulation()` function orchestrates the simulation by calling `thread_creation()`, which creates threads for each philosopher and a monitor thread.  
- Each philosopher thread runs `philosopher_routine()`, simulating the life cycle of a philosopher:  
  - Taking forks  
  - Eating  
  - Sleeping  
  - Thinking  
- To prevent contention, even-numbered philosophers begin with a small delay (half the eating time) to stagger their actions.  

### 🍽 Fork Management  
- Philosophers use the `take_forks()` function to attempt to lock the left and right forks.  
- If a fork is unavailable, they wait for a short time before trying again, ensuring fairness and avoiding deadlocks.  
- After eating, forks are released using `release_forks()`.  

### 👀 Monitor Thread  
- The monitor thread runs `check_starvation()`, periodically checking the state of all philosophers to ensure no philosopher starves.  
- If a philosopher hasn’t eaten within the `time_to_die`, the simulation stops, and the program prints a death message.  
- If the optional `times_to_eat` argument is provided, the monitor also checks if all philosophers have eaten the required number of meals, stopping the simulation when the condition is met.  

### 🔄 Thread Synchronization  
- After thread creation, `run_simulation()` waits for all philosopher and monitor threads to complete using `pthread_join()`.  
- This ensures the main program only proceeds to cleanup after all threads finish execution.  

### 🧹 Resource Cleanup  
- The `cleanup_resources()` function destroys all mutexes (e.g., forks, control locks) and frees allocated memory for philosophers and forks.  
- This ensures the program exits cleanly without memory leaks.
