import numpy as np

# number of processes and resources
n_processes = 5
m_resources = 3

# initialize matrices
available = np.array([3, 3, 2])  # available resources
maximum = np.array([[7, 5, 3],   # max demand per process
                    [3, 2, 2],
                    [9, 0, 2],
                    [2, 2, 2],
                    [4, 3, 3]])

allocation = np.array([[0, 1, 0],  # allocated resources
                       [2, 0, 0],
                       [3, 0, 2],
                       [2, 1, 1],
                       [0, 0, 2]])

# calculate need matrix
need = maximum - allocation

# check if system is safe
def is_safe(available, allocation, need):
    work = available.copy()
    finish = [False] * n_processes
    safe_sequence = []

    while len(safe_sequence) < n_processes:
        found = False
        for p in range(n_processes):
            if not finish[p] and all(need[p] <= work):
                work += allocation[p]
                safe_sequence.append(p)
                finish[p] = True
                found = True
        if not found:
            return False, []
    return True, safe_sequence

# request resources
def request_resources(process, request):
    global available, allocation, need
    if all(request <= need[process]) and all(request <= available):
        # simulate allocation
        available -= request
        allocation[process] += request
        need[process] -= request

        # check safety
        safe, sequence = is_safe(available, allocation, need)
        if safe:
            print(f"Resources allocated to process {process}. Safe sequence: {sequence}")
        else:
            # rollback
            available += request
            allocation[process] -= request
            need[process] += request
            print(f"Error: Allocating resources to process {process} would result in an unsafe state.")
    else:
        print(f"Error: Process {process} requested more resources than allowed.")

# test cases
print("Initial safe test:")
safe, sequence = is_safe(available, allocation, need)
if safe:
    print(f"System is in a safe state. Safe sequence: {sequence}")
else:
    print("System is not in a safe state.")

# test case 2
request = np.array([1, 0, 2])
print("\nProcess 1 requesting resources [1, 0, 2]:")
request_resources(1, request)

# test case 3
request = np.array([3, 3, 1])
print("\nProcess 4 requesting resources [3, 3, 1]:")
request_resources(4, request)
