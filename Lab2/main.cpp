#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

const int NUM_PHILOSOPHERS = 5;

// Mutexes for forks
pthread_mutex_t forks[NUM_PHILOSOPHERS];
// Condition variables to manage fork availability
pthread_cond_t cond_var[NUM_PHILOSOPHERS];
// Array to track fork availability (true means the fork is available)
bool forks_available[NUM_PHILOSOPHERS];

// Function for philosopher to pick up forks
void pickup_forks(int philosopher_number) {
    int id = philosopher_number;
    int left_fork = philosopher_number;
    int right_fork = (philosopher_number + 1) % NUM_PHILOSOPHERS;

    // Lock the mutex associated with the left fork
    pthread_mutex_lock(&forks[left_fork]);

    // Wait until both the left and right forks are available
    while (!forks_available[left_fork] || !forks_available[right_fork]) {
        pthread_cond_wait(&cond_var[left_fork], &forks[left_fork]);
    }

    // Both forks are available, so the philosopher can pick them up
    forks_available[left_fork] = false;
    std::cout << "Philosopher " << id << " has fork #" << left_fork << std::endl;
    forks_available[right_fork] = false;
    std::cout << "Philosopher " << id << " has fork #" << right_fork << std::endl;

    std::cout << "Philosopher " << id << " has both forks and is ready to eat." << std::endl;
    pthread_mutex_unlock(&forks[left_fork]);
}

// Function for philosopher to return forks
void return_forks(int philosopher_number) {
    int id = philosopher_number;
    int left_fork = philosopher_number;
    int right_fork = (philosopher_number + 1) % NUM_PHILOSOPHERS;

    // Lock the mutex associated with the left fork
    pthread_mutex_lock(&forks[left_fork]);

    // Mark both forks as available
    forks_available[left_fork] = true;
    std::cout << "Philosopher " << id << " has dropped fork #" << left_fork << std::endl;
    forks_available[right_fork] = true;
    std::cout << "Philosopher " << id << " has dropped fork #" << right_fork<< std::endl;

    // Signal to the philosophers waiting on these forks that they are now available
    pthread_cond_signal(&cond_var[left_fork]);
    pthread_cond_signal(&cond_var[right_fork]);

    pthread_mutex_unlock(&forks[left_fork]);
}

// Philosopher thread function
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    while (true) {
        std::cout << "Philosopher " << id << " is thinking." << std::endl;
        sleep(rand() % 3 + 1);  // Simulate thinking

        pickup_forks(id);

        std::cout << "Philosopher " << id << " is eating." << std::endl;
        sleep(rand() % 3 + 1);  
        
        return_forks(id);
    }

    return nullptr;
}

int main() {
    std::vector<pthread_t> threads(NUM_PHILOSOPHERS);
    std::vector<int> ids(NUM_PHILOSOPHERS);

    // Initialize mutexes and condition variables
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], nullptr);
        pthread_cond_init(&cond_var[i], nullptr);
        forks_available[i] = true;  // Initially, all forks are available
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, &ids[i]);
    }

    // Join philosopher threads (this will never actually happen because of the infinite loop)
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Destroy mutexes and condition variables (never reached in this code)
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
        pthread_cond_destroy(&cond_var[i]);
    }

    return 0;
}
