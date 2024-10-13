#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>

const int NUM_PHILOSOPHERS = 5;

// philosopher states
enum { THINKING, HUNGRY, EATING } state[NUM_PHILOSOPHERS];

// fork states
bool fork_avail[NUM_PHILOSOPHERS];

// mutex for synchronization
pthread_mutex_t mutex;

// condition variables for philosophers
pthread_cond_t self[NUM_PHILOSOPHERS];

// mutex for printing
pthread_mutex_t print_mutex;

// safe console print
void print(const std::string& message) {
    pthread_mutex_lock(&print_mutex);
    std::cout << message << std::endl;
    pthread_mutex_unlock(&print_mutex);
}

// check if philosopher can eat
void test(int i) {
    int left = (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
    int right = (i + 1) % NUM_PHILOSOPHERS;
    if (state[left] != EATING && state[i] == HUNGRY && state[right] != EATING &&
        fork_avail[left] && fork_avail[i]) {
        // start eating if neighbors aren't and forks are available
        state[i] = EATING;
        fork_avail[left] = false;
        fork_avail[i] = false;
        pthread_cond_signal(&self[i]); // signal to start eating
    }
     // show fork state
    print("Fork " + std::to_string(left) + " is with Philosopher " + std::to_string(i));
    print("Fork " + std::to_string(i) + " is with Philosopher " + std::to_string(i));
}

// pick up forks
void pickup_forks(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    print("Philosopher " + std::to_string(i) + " is hungry.");
    
    test(i);
    if (state[i] != EATING) {
        pthread_cond_wait(&self[i], &mutex); // wait if can't eat
    }    
 
    pthread_mutex_unlock(&mutex);
}

// return forks
void return_forks(int i) {
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    int left = (i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
    int right = (i + 1) % NUM_PHILOSOPHERS;
    fork_avail[left] = true;
    fork_avail[i] = true;
    print("Philosopher " + std::to_string(i) + " has returned the forks.");
    
    // test neighbors
    test(left); 
    test(right); 
    pthread_mutex_unlock(&mutex);
}

// philosopher thread
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int eat_count = 0;

    // end loop arbitrarily after 3 meals
    while (eat_count < 3) {
        int think_time = rand() % 3 + 1;
        int eat_time = rand() % 3 + 1;
        print("Philosopher " + std::to_string(id) + " spent " + std::to_string(think_time) + "s thinking.");
        sleep(think_time);  // simulate thinking

        pickup_forks(id);

        print("Philosopher " + std::to_string(id) + " spent " + std::to_string(eat_time) + "s eating.");
        sleep(eat_time);  // simulate eating
        eat_count++;

        return_forks(id);
    }

    print("Philosopher " + std::to_string(id) + " has finished their meal and is leaving the table.");
    return nullptr;
}

int main() {
    std::vector<pthread_t> threads(NUM_PHILOSOPHERS);
    std::vector<int> ids(NUM_PHILOSOPHERS);

    // initialize mutex and condition variables
    pthread_mutex_init(&mutex, nullptr);
    pthread_mutex_init(&print_mutex, nullptr);
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_cond_init(&self[i], nullptr);
        state[i] = THINKING; // start thinking
        fork_avail[i] = true; // all forks are initially available
    }

    // Print initial fork state
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        print("Fork " + std::to_string(i) + " is available");
    }

    // create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, &ids[i]);
    }

    // join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&print_mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_cond_destroy(&self[i]);
    }

    std::cout << "All philosophers have finished eating and left the table." << std::endl;
    return 0;
}
