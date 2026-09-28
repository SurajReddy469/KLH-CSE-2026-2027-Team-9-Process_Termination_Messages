/*
 * Optional demonstration program for project testing.
 * Build:
 *   gcc -Wall -Wextra -std=c11 -O2 -o termination_test termination_test.c
 *
 * Run:
 *   ./termination_test normal
 *   ./termination_test error
 *   ./termination_test signal
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s normal|error|signal\n", argv[0]);
        return 2;
    }

    if (strcmp(argv[1], "normal") == 0) {
        printf("Test application: normal termination\n");
        return 0;
    }

    if (strcmp(argv[1], "error") == 0) {
        printf("Test application: non-zero termination\n");
        return 5;
    }

    if (strcmp(argv[1], "signal") == 0) {
        printf("Test application: sending SIGTERM to itself\n");
        fflush(stdout);
        raise(SIGTERM);
        return 1;
    }

    printf("Unknown test mode.\n");
    return 2;
}
