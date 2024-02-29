#include "utils.hpp"
#include "test_system.hpp"

#include <cstdio>
#include <iostream>

// LINUX SPECIFIC HEADERS
#include <unistd.h>
#include <sys/wait.h>

// Test time limit
constexpr size_t TIMEOUT_MILLISSECONDS = 100U;

using namespace TestSystem;

TestResult TestSystem::run_test(const char* name, TestScenario test) {
	printf(SET_COLOR_CYAN "Running test %20s: " RESET_COLOR, name);
    fflush(stdout); // print everything unprinted

	// LINUX SPECIFIC CODE
	// Create a new parallel process to run the test in it 
	// and handle the timeout
	pid_t process_id = fork(); 
		// ==0 if child process
		// >0 if main process
		// <0 if something wrong

	// Handle the excception while calling fork()
	if (process_id < 0) {
		fprintf(stderr, "Unable to call fork()\n");
		exit(EXIT_FAILURE);
	}

	// Child process
	if (process_id == 0) {
		try {
			bool result = test(); // run the test
			exit(result ? OK : FAIL);
		}
		catch (const std::exception& exc) {
			printf(SET_COLOR_PURPLE "[EXC] (exception: %s)\n" RESET_COLOR, exc.what());
			exit(EXCEPTION);
		}
	}

	// Parent process
	int status = 0; // the code that the child process ended with
	pid_t ret = 0; // the code that wait function returned, trying to get status

	// Run the loop for the fixed amount of times, one millisecond each
	for (size_t ms = 0; ms < TIMEOUT_MILLISSECONDS; ms++) {
		// wait for the child process to return
		ret = waitpid(process_id, &status, WNOHANG);

		// Handle the exception in waitpid() function
		if (ret == -1) {
			fprintf(stderr, "Unable to call wait()\n");
            exit(EXIT_FAILURE);
		}

		// Break the loop if something meaningful returned
		if (ret != 0) break;

		// sleep for 1ms
		usleep(1000U);
	}

	// Handle timeout exit
    if (ret == 0)
    {
        printf(SET_COLOR_PURPLE "[TIMEOUT]\n" RESET_COLOR);
        return TIMEOUT;
    }

    // Handle correct exit
    if (WIFEXITED(status))
    {
        switch (WEXITSTATUS(status))
        {
            case OK:
   			{
                printf(SET_COLOR_GREEN "[OK]\n" RESET_COLOR);
                return OK;
            }
            case FAIL:
            {
                printf(SET_COLOR_RED "[FAIL]\n" RESET_COLOR);
                return FAIL;
            }
            case EXCEPTION:
            {
                printf(SET_COLOR_PURPLE "[EXC]\n" RESET_COLOR);
                return EXCEPTION;
            }
            default:
            {
                fprintf(stderr, "Unexpected switch case\n");
                exit(EXIT_FAILURE);
            }
        };
    }

    // Handle error exit:
    else if (WIFSIGNALED(status))
    {
        printf(SET_COLOR_PURPLE "[ERROR] (killed by signal %d)\n" RESET_COLOR, WTERMSIG(status));
        return ERROR;
    }

    // This point must be unreachable:
    fprintf(stderr, "Unexpected waitpid() opeartion\n");
    exit(EXIT_FAILURE);
}