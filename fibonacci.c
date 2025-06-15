#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to calculate Fibonacci number iteratively
int fibonacci_calculate(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    
    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Parallel method: Each child process calculates one Fibonacci number
void fibonacci_parallel_method(int n) {
    printf("\n=== Fibonacci Parallel Method using Fork() and Wait() ===\n");
    printf("Creating %d child processes, each calculating one Fibonacci number\n\n", n);
    
    // Create child processes
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            int fib_value = fibonacci_calculate(i);
            printf("Child Process %d (PID: %d): F(%d) = %d\n", 
                   i, getpid(), i, fib_value);
            fflush(stdout);
            exit(0);  // Child exits after calculation
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        // Parent continues to create more children
    }
    
    // Parent waits for all children to complete
    printf("\nParent Process (PID: %d) waiting for all children to complete...\n", getpid());
    
    int status;
    for (int i = 0; i < n; i++) {
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child with PID %d completed successfully\n", child_pid);
        } else {
            printf("Child with PID %d terminated abnormally\n", child_pid);
        }
    }
    
    printf("\nAll child processes completed!\n");
    printf("Fibonacci series calculation finished.\n");
}

int main() {
    int n;
    
    printf("====================================\n");
    printf("FIBONACCI SERIES USING FORK() & WAIT()\n");
    printf("====================================\n\n");
    
    // Get input from terminal
    printf("Enter the number of Fibonacci numbers to calculate: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Error: Please enter a valid positive integer\n");
        return 1;
    }
    
    printf("\nCalculating first %d Fibonacci numbers using parallel method...\n", n);
    printf("Main Process PID: %d\n", getpid());
    
    // Execute parallel method
    fibonacci_parallel_method(n);
    
    printf("\n====================================\n");
    printf("Program completed successfully!\n");
    printf("====================================\n");
    
    return 0;
}
