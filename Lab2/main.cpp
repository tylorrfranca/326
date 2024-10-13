#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>

const int NUM_PHILOSOPHERS = 5;
const int MAX_COURSES = 3;

// mutexes for forks
pthread_mutex_t forks[NUM_PHILOSOPHERS];
// condition variables for fork availability
pthread_cond_t cond_var[NUM_PHILOSOPHERS];
// track fork availability
bool forks_available[NUM_PHILOSOPHERS];

// mutex for printing
pthread_mutex_t print_mutex;

// safely print to console
void print(const std::string& message) {
    pthread_mutex_lock(&print_mutex);
    std::cout << message << std::endl;
    pthread_mutex_unlock(&print_mutex);
}

// pick up forks
void pickup_forks(int philosopher_number) {
    int id = philosopher_number;
    int left_fork = philosopher_number;
    int right_fork = (philosopher_number + 1) % NUM_PHILOSOPHERS;

    // lock left fork
    pthread_mutex_lock(&forks[left_fork]);

    // wait for both forks
    while (!forks_available[left_fork] || !forks_available[right_fork]) {
        pthread_cond_wait(&cond_var[left_fork], &forks[left_fork]);
    }

    // pick up forks
    forks_available[left_fork] = false;
    print("Philosopher " + std::to_string(id) + " has picked up fork #" + std::to_string(left_fork));
    forks_available[right_fork] = false;
    print("Philosopher " + std::to_string(id) + " has picked up fork #" + std::to_string(right_fork));

    print("Philosopher " + std::to_string(id) + " has both forks and is ready to eat.");
    pthread_mutex_unlock(&forks[left_fork]);
}

// return forks
void return_forks(int philosopher_number) {
    int id = philosopher_number;
    int left_fork = philosopher_number;
    int right_fork = (philosopher_number + 1) % NUM_PHILOSOPHERS;

    // lock left fork
    pthread_mutex_lock(&forks[left_fork]);

    // mark forks available
    forks_available[left_fork] = true;
    print("Philosopher " + std::to_string(id) + " has dropped fork #" + std::to_string(left_fork));
    forks_available[right_fork] = true;
    print("Philosopher " + std::to_string(id) + " has dropped fork #" + std::to_string(right_fork));

    // signal availability
    pthread_cond_signal(&cond_var[left_fork]);
    pthread_cond_signal(&cond_var[right_fork]);

    pthread_mutex_unlock(&forks[left_fork]);
}

// philosopher thread
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int eat_count = 0;

    while (eat_count < MAX_COURSES) {
        print("Philosopher " + std::to_string(id) + " is thinking.");
        sleep(rand() % 3 + 1);  // think

        pickup_forks(id);

        print("Philosopher " + std::to_string(id) + " is eating.");
        sleep(rand() % 3 + 1);  // eat

        return_forks(id);
        eat_count++;
    }

    print("Philosopher " + std::to_string(id) + " has finished a full course and is leaving the table.");
    return nullptr;
}

int main() {
    std::vector<pthread_t> threads(NUM_PHILOSOPHERS);
    std::vector<int> ids(NUM_PHILOSOPHERS);

    // init mutexes and conditions
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], nullptr);
        pthread_cond_init(&cond_var[i], nullptr);
        forks_available[i] = true;  // all forks available
    }

    // create threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, &ids[i]);
    }

    // join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // cleanup
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
        pthread_cond_destroy(&cond_var[i]);
    }

    std::cout << "All philosophers have finished eating and left the table." << std::endl;
    return 0;
}
