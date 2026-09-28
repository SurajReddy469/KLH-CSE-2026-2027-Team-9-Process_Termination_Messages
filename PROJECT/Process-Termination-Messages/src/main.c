/*
 * PROCESS TERMINATION MESSAGES
 * Operating Systems and Systems Programming (25CS2104E)
 *
 * CORE LINUX MODE:
 *   fork() -> execvp() -> waitpid()
 *   WIFEXITED()/WEXITSTATUS()
 *   WIFSIGNALED()/WTERMSIG()
 *
 * WINDOWS CHROME MODE (WSL):
 *   Normal: launch Chrome, monitor chrome.exe, report PID + normal closure.
 *   Abnormal: launch Chrome, deliberately terminate its Windows processes,
 *             report PID + forced/abnormal termination.
 *
 * Note:
 * Windows Chrome is not a Linux child process, so its mode does not use
 * waitpid(). The Linux mode is the POSIX implementation required by the
 * submitted project abstract.
 */

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT 512
#define MAX_ARGS 64

static void trim_newline(char *s)
{
    if (s) s[strcspn(s, "\n")] = '\0';
}

static int parse_command(char *input, char *argv[], int max_args)
{
    int argc = 0;
    char *token = strtok(input, " \t");

    while (token && argc < max_args - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

static void separator(void)
{
    printf("\n========================================\n");
}

static void banner(void)
{
    separator();
    printf("     PROCESS TERMINATION MESSAGES\n");
    printf("========================================\n");
    printf("Linux process management + Chrome demo\n");
    separator();
}

/* ---------- Linux/POSIX core implementation ---------- */

static void explain_linux_status(pid_t pid, int status)
{
    printf("\n[Monitor] Process ended.\n");
    printf("[Monitor] Process ID (PID): %ld\n", (long)pid);

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);

        printf("[Monitor] Termination type: Normal\n");
        printf("[Monitor] Exit status: %d\n", code);

        if (code == 0)
            printf("[Monitor] Message: Process terminated normally.\n");
        else
            printf("[Monitor] Message: Process terminated normally "
                   "with a non-zero exit status.\n");
    } else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);

        printf("[Monitor] Termination type: Abnormal (Signal)\n");
        printf("[Monitor] Signal number: %d\n", sig);

        if (sig == SIGTERM)
            printf("[Monitor] Signal: SIGTERM\n");
        else if (sig == SIGKILL)
            printf("[Monitor] Signal: SIGKILL\n");
        else if (sig == SIGINT)
            printf("[Monitor] Signal: SIGINT\n");
        else if (sig == SIGHUP)
            printf("[Monitor] Signal: SIGHUP\n");
        else
            printf("[Monitor] Signal: other (%d)\n", sig);

        printf("[Monitor] Exit status: Not available because the "
               "process was signal-terminated.\n");
        printf("[Monitor] Message: Process terminated abnormally.\n");
    } else {
        printf("[Monitor] Termination type: Other status\n");
        printf("[Monitor] Exit status: Not available\n");
    }
}

static void linux_monitor(void)
{
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];

    printf("\nEnter a Linux command to launch.\n");
    printf("Examples: sleep 10 | bash | ./termination_test signal\n");
    printf("Command: ");

    if (!fgets(input, sizeof(input), stdin)) {
        printf("[Monitor] Input error.\n");
        return;
    }

    trim_newline(input);

    if (!input[0]) {
        printf("[Monitor] No command entered.\n");
        return;
    }

    int argc = parse_command(input, argv, MAX_ARGS);
    if (argc == 0) {
        printf("[Monitor] Invalid command.\n");
        return;
    }

    printf("\n[Parent] PID: %ld\n", (long)getpid());
    printf("[Parent] Creating child using fork()...\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("[Parent] fork");
        return;
    }

    if (pid == 0) {
        printf("[Child] PID: %ld\n", (long)getpid());
        printf("[Child] Launching with execvp()...\n");
        fflush(stdout);

        execvp(argv[0], argv);

        fprintf(stderr, "[Child] Could not launch '%s': %s\n",
                argv[0], strerror(errno));
        _exit(127);
    }

    printf("[Parent] Child PID: %ld\n", (long)pid);
    printf("[Monitor] Application is running...\n");
    printf("[Monitor] Waiting with waitpid()...\n");

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result == -1 && errno == EINTR);

    if (result == -1) {
        perror("[Parent] waitpid");
        return;
    }

    explain_linux_status(pid, status);
}

/* ---------- Windows/WSL Chrome helpers ---------- */

/*
 * Returns the first chrome.exe PID, or -1 if none exists / command failed.
 * The output is written to a temporary file so WSL can parse the Windows
 * PowerShell result reliably.
 */
static long get_chrome_pid(void)
{
    const char *tmp = "/tmp/ptm_chrome_pid.txt";
    const char *cmd =
        "powershell.exe -NoProfile -Command "
        "\"$p=Get-Process -Name chrome -ErrorAction SilentlyContinue | "
        "Select-Object -First 1 -ExpandProperty Id; "
        "if($p){$p}\" "
        "> /tmp/ptm_chrome_pid.txt 2>/dev/null";

    if (system(cmd) == -1)
        return -1;

    FILE *f = fopen(tmp, "r");
    if (!f)
        return -1;

    long pid = -1;
    if (fscanf(f, "%ld", &pid) != 1)
        pid = -1;

    fclose(f);
    return pid;
}

static int chrome_process_count(void)
{
    const char *tmp = "/tmp/ptm_chrome_count.txt";
    const char *cmd =
        "powershell.exe -NoProfile -Command "
        "\"@(Get-Process -Name chrome -ErrorAction SilentlyContinue).Count\" "
        "> /tmp/ptm_chrome_count.txt 2>/dev/null";

    if (system(cmd) == -1)
        return -1;

    FILE *f = fopen(tmp, "r");
    if (!f)
        return -1;

    int count = 0;
    if (fscanf(f, "%d", &count) != 1)
        count = 0;

    fclose(f);
    return count;
}

static int start_chrome(void)
{
    const char *cmd =
        "cmd.exe /C start \"\" "
        "\"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe\" "
        "--new-window about:blank";

    return system(cmd);
}

static int kill_all_chrome(void)
{
    /*
     * taskkill /F deliberately forces termination. This is used only for
     * the abnormal-termination demonstration selected by the user.
     */
    return system("taskkill.exe /F /IM chrome.exe > /dev/null 2>&1");
}

static int wait_for_chrome_to_start(long *pid)
{
    for (int i = 0; i < 20; ++i) {
        sleep(1);
        long p = get_chrome_pid();

        if (p > 0) {
            *pid = p;
            return 1;
        }
    }

    return 0;
}

static int wait_for_chrome_to_end(void)
{
    for (int i = 0; i < 120; ++i) {
        sleep(1);
        int count = chrome_process_count();

        if (count == 0)
            return 1;

        if (count < 0)
            return 0;
    }

    return 0;
}

static int ensure_chrome_not_running(void)
{
    int count = chrome_process_count();

    if (count < 0) {
        printf("[Chrome Monitor] Could not query Windows Chrome.\n");
        return 0;
    }

    if (count > 0) {
        printf("\n[Chrome Monitor] Chrome is already running.\n");
        printf("[Chrome Monitor] Current chrome.exe process count: %d\n", count);
        printf("[Chrome Monitor] Close ALL existing Chrome windows and "
               "background Chrome processes, then try again.\n");
        return 0;
    }

    return 1;
}

/* ---------- Chrome normal termination ---------- */

static void chrome_normal(void)
{
    if (!ensure_chrome_not_running())
        return;

    printf("\n[Chrome Monitor] Starting Windows Google Chrome...\n");

    if (start_chrome() == -1) {
        printf("[Chrome Monitor] Failed to start Chrome.\n");
        return;
    }

    long pid = -1;

    if (!wait_for_chrome_to_start(&pid)) {
        printf("[Chrome Monitor] Chrome did not start or its PID "
               "could not be detected.\n");
        return;
    }

    printf("[Chrome Monitor] Chrome started.\n");
    printf("[Chrome Monitor] Process ID (PID): %ld\n", pid);
    printf("[Chrome Monitor] Termination type: Waiting for normal closure\n");
    printf("[Chrome Monitor] Close ALL Chrome windows normally.\n");

    if (!wait_for_chrome_to_end()) {
        printf("[Chrome Monitor] Could not confirm Chrome termination.\n");
        return;
    }

    /*
     * Windows' process enumeration does not provide a POSIX waitpid-style
     * exit status here. We therefore do not invent an exit code.
     */
    printf("\n[Chrome Monitor] Application process ended.\n");
    printf("[Chrome Monitor] Process ID (PID): %ld\n", pid);
    printf("[Chrome Monitor] Termination type: Normal\n");
    printf("[Chrome Monitor] Exit status: Unavailable (Windows process "
           "was monitored externally)\n");
    printf("[Chrome Monitor] Message: Google Chrome closed normally.\n");
}

/* ---------- Chrome abnormal termination ---------- */

static void chrome_abnormal(void)
{
    if (!ensure_chrome_not_running())
        return;

    printf("\n[Chrome Monitor] Starting Windows Google Chrome...\n");

    if (start_chrome() == -1) {
        printf("[Chrome Monitor] Failed to start Chrome.\n");
        return;
    }

    long pid = -1;

    if (!wait_for_chrome_to_start(&pid)) {
        printf("[Chrome Monitor] Chrome did not start or its PID "
               "could not be detected.\n");
        return;
    }

    printf("[Chrome Monitor] Chrome started.\n");
    printf("[Chrome Monitor] Process ID (PID): %ld\n", pid);
    printf("[Chrome Monitor] Termination type: Waiting for abnormal "
           "termination\n");
    printf("[Chrome Monitor] Press ENTER to forcibly terminate Chrome...\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (kill_all_chrome() == -1) {
        printf("[Chrome Monitor] Failed to issue taskkill.\n");
        return;
    }

    if (!wait_for_chrome_to_end()) {
        printf("[Chrome Monitor] Could not confirm Chrome termination.\n");
        return;
    }

    printf("\n[Chrome Monitor] Application process ended.\n");
    printf("[Chrome Monitor] Process ID (PID): %ld\n", pid);
    printf("[Chrome Monitor] Termination type: Abnormal (Forced)\n");
    printf("[Chrome Monitor] Exit status: Unavailable\n");
    printf("[Chrome Monitor] Reason: Windows taskkill /F forcibly "
           "terminated chrome.exe.\n");
    printf("[Chrome Monitor] Message: Google Chrome was terminated "
           "abnormally.\n");
}

/* ---------- Information ---------- */

static void show_concepts(void)
{
    separator();
    printf("PROJECT CONCEPTS\n");
    separator();

    printf("fork()        : creates a Linux child process.\n");
    printf("execvp()      : launches a Linux application in the child.\n");
    printf("waitpid()     : parent waits for its Linux child.\n");
    printf("getpid()      : obtains a process ID.\n");
    printf("WIFEXITED()   : checks normal Linux termination.\n");
    printf("WEXITSTATUS() : obtains a normal Linux exit status.\n");
    printf("WIFSIGNALED() : checks signal-based Linux termination.\n");
    printf("WTERMSIG()    : obtains the Linux terminating signal.\n");

    printf("\nChrome/WSL mode:\n");
    printf("PowerShell/Get-Process : obtains Chrome PID/process count.\n");
    printf("taskkill /F            : demonstrates forced Windows termination.\n");

    printf("\nSyllabus mapping:\n");
    printf("CO-1 -> Linux/POSIX system programming.\n");
    printf("CO-2 -> Process creation, execution, synchronization, termination.\n");
    printf("CO-3 -> Signals and signal-based termination status.\n");
}

int main(void)
{
    int choice;

    banner();

    while (1) {
        printf("\n1. Monitor Linux child process\n");
        printf("2. Chrome - Normal Termination\n");
        printf("3. Chrome - Abnormal Termination\n");
        printf("4. Display project concepts\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("[Monitor] Enter a number from 1 to 5.\n");
            continue;
        }

        switch (choice) {
            case 1:
                {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                }
                linux_monitor();
                break;

            case 2:
                chrome_normal();
                break;

            case 3:
                chrome_abnormal();
                break;

            case 4:
                show_concepts();
                break;

            case 5:
                printf("\n[Monitor] Exiting project.\n");
                return 0;

            default:
                {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                }
                printf("[Monitor] Invalid choice.\n");
        }
    }
}
