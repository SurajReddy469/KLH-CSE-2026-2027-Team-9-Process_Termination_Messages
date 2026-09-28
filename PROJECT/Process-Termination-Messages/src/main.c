#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 32

const char *get_signal_name(int sig)
{
    switch (sig)
    {
        case SIGINT:
            return "SIGINT";

        case SIGTERM:
            return "SIGTERM";

        case SIGKILL:
            return "SIGKILL";

        case SIGSEGV:
            return "SIGSEGV";

        case SIGABRT:
            return "SIGABRT";

        case SIGFPE:
            return "SIGFPE";

        default:
            return "Unknown Signal";
    }
}

void explain_linux_status(pid_t pid, int status)
{
    printf("\n[Monitor] Process ended.\n");
    printf("[Monitor] Process ID (PID): %d\n", pid);

    if (WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);

        printf("[Monitor] Termination type: Normal\n");
        printf("[Monitor] Exit status: %d\n", exit_status);

        if (exit_status == 0)
        {
            printf("[Monitor] Message: Process terminated normally.\n");
        }
        else
        {
            printf("[Monitor] Message: Process terminated normally "
                   "with a non-zero exit status.\n");
        }
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);

        printf("[Monitor] Termination type: Abnormal (Signal)\n");
        printf("[Monitor] Signal: %d (%s)\n",
               sig,
               get_signal_name(sig));

        printf("[Monitor] Message: Process terminated abnormally "
               "because of a signal.\n");
    }
    else if (WIFSTOPPED(status))
    {
        printf("[Monitor] Termination type: Process stopped\n");
        printf("[Monitor] Signal: %d\n", WSTOPSIG(status));
        printf("[Monitor] Message: Process was stopped.\n");
    }
    else
    {
        printf("[Monitor] Message: Unknown process status.\n");
    }
}

int parse_command(char *command, char *argv[])
{
    int argc = 0;
    char *token;

    token = strtok(command, " \t\n");

    while (token != NULL && argc < MAX_ARGS - 1)
    {
        argv[argc] = token;
        argc++;

        token = strtok(NULL, " \t\n");
    }

    argv[argc] = NULL;

    return argc;
}

void monitor_linux_child(void)
{
    char command[MAX_COMMAND_LENGTH];
    char *argv[MAX_ARGS];

    printf("\nEnter a Linux command to launch.\n");
    printf("Examples: sleep 10 | bash | ./termination_test signal\n");
    printf("Command: ");

    if (fgets(command, sizeof(command), stdin) == NULL)
    {
        printf("[Monitor] Failed to read command.\n");
        return;
    }

    if (strlen(command) <= 1)
    {
        printf("[Monitor] Empty command. Please try again.\n");
        return;
    }

    int argc = parse_command(command, argv);

    if (argc == 0)
    {
        printf("[Monitor] Invalid command.\n");
        return;
    }

    printf("\n[Parent] PID: %d\n", getpid());
    printf("[Parent] Creating child using fork()...\n");

    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        perror("[Parent] fork() failed");
        return;
    }

    /* CHILD PROCESS */
    if (child_pid == 0)
    {
        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Launching with execvp()...\n");

        execvp(argv[0], argv);

        fprintf(stderr,
                "[Child] Could not launch '%s': %s\n",
                argv[0],
                strerror(errno));

        _exit(127);
    }

    /* PARENT PROCESS */
    printf("[Parent] Child PID: %d\n", child_pid);
    printf("[Monitor] Application is running...\n");
    printf("[Monitor] Waiting with waitpid()...\n");

    int status;

    pid_t result = waitpid(child_pid, &status, 0);

    if (result == -1)
    {
        perror("[Monitor] waitpid() failed");
        return;
    }

    explain_linux_status(child_pid, status);
}

int get_chrome_process_info(int *pid, int *count)
{
    FILE *fp;
    char line[512];
    char image_name[128];
    int process_id;

    *pid = 0;
    *count = 0;

        system(
        "tasklist.exe /FI \"IMAGENAME eq chrome.exe\" "
        "/FO CSV /NH > /tmp/chrome_processes.txt 2>/dev/null"
    );

    fp = fopen("/tmp/chrome_processes.txt", "r");

    if (fp == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        image_name[0] = '\0';
        process_id = 0;

            if (sscanf(line,
                   "\"%127[^\"]\",\"%d\"",
                   image_name,
                   &process_id) == 2)
        {
            if (strcmp(image_name, "chrome.exe") == 0)
            {
                (*count)++;

                if (*pid == 0)
                {
                    *pid = process_id;
                }
            }
        }
    }

    fclose(fp);

    return (*count > 0);
}

int launch_chrome(void)
{
    printf("[Chrome Monitor] Starting Windows Google Chrome...\n");

    int result = system(
        "powershell.exe -NoProfile -Command "
        "\"Start-Process "
        "'C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe' "
        "-ArgumentList '--new-window','about:blank'\""
    );

    if (result != 0)
    {
        printf("[Chrome Monitor] Failed to send Chrome launch command.\n");
        return 0;
    }

    printf("[Chrome Monitor] Chrome launch command sent.\n");

    sleep(3);

    return 1;
}

int wait_for_chrome_start(int *pid, int *count)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if (get_chrome_process_info(pid, count))
        {
            return 1;
        }

        sleep(1);
    }

    return 0;
}

int is_chrome_running(void)
{
    int pid;
    int count;

    return get_chrome_process_info(&pid, &count);
}


void wait_for_chrome_close(void)
{
    while (is_chrome_running())
    {
        sleep(1);
    }
}


void chrome_normal_termination(void)
{
    int existing_pid;
    int existing_count;

    printf("\n[Chrome Monitor] Checking whether Chrome is already running...\n");

    if (get_chrome_process_info(&existing_pid, &existing_count))
    {
        printf("[Chrome Monitor] Chrome is already running.\n");
        printf("[Chrome Monitor] Current chrome.exe process count: %d\n",
               existing_count);

        printf("[Chrome Monitor] Close all existing Chrome windows "
               "and background Chrome processes, then try again.\n");

        return;
    }

    if (!launch_chrome())
    {
        return;
    }

    int chrome_pid;
    int chrome_count;

    if (!wait_for_chrome_start(&chrome_pid, &chrome_count))
    {
        printf("[Chrome Monitor] Chrome did not start or its PID "
               "could not be detected.\n");

        return;
    }

    printf("[Chrome Monitor] Chrome is now being monitored.\n");
    printf("[Chrome Monitor] chrome.exe detected: %d process(es).\n",
           chrome_count);

    printf("[Chrome Monitor] Monitoring PID: %d\n",
           chrome_pid);

    printf("[Chrome Monitor] Waiting for Chrome to close normally...\n");

    wait_for_chrome_close();

    printf("\n[Chrome Monitor] Chrome process terminated.\n");

    printf("[Chrome Monitor] Process ID (PID): %d\n",
           chrome_pid);

    printf("[Chrome Monitor] Termination type: Normal\n");

    printf("[Chrome Monitor] Exit status: Unavailable\n");

    printf("[Chrome Monitor] Message: Google Chrome has been "
           "closed normally.\n");

    printf("[Chrome Monitor] Detection method: Windows "
           "tasklist.exe process monitoring.\n");
}

void chrome_abnormal_termination(void)
{
    int existing_pid;
    int existing_count;

    printf("\n[Chrome Monitor] Checking whether Chrome is already running...\n");

    if (get_chrome_process_info(&existing_pid, &existing_count))
    {
        printf("[Chrome Monitor] Chrome is already running.\n");

        printf("[Chrome Monitor] Current chrome.exe process count: %d\n",
               existing_count);

        printf("[Chrome Monitor] Close all existing Chrome windows "
               "and background Chrome processes, then try again.\n");

        return;
    }

    if (!launch_chrome())
    {
        return;
    }

    int chrome_pid;
    int chrome_count;

    if (!wait_for_chrome_start(&chrome_pid, &chrome_count))
    {
        printf("[Chrome Monitor] Chrome did not start or its PID "
               "could not be detected.\n");

        return;
    }

    printf("[Chrome Monitor] Chrome is now being monitored.\n");

    printf("[Chrome Monitor] chrome.exe detected: %d process(es).\n",
           chrome_count);

    printf("[Chrome Monitor] Monitoring PID: %d\n",
           chrome_pid);

    printf("\n[Chrome Monitor] Chrome is running.\n");

    printf("[Chrome Monitor] Press ENTER to force terminate Chrome...\n");

    getchar();

    printf("[Chrome Monitor] Sending forced termination command...\n");

    int result = system(
        "taskkill.exe /F /IM chrome.exe > /dev/null 2>&1"
    );

    if (result != 0)
    {
        printf("[Chrome Monitor] taskkill command returned an error.\n");
    }

    sleep(2);

    printf("\n[Chrome Monitor] Chrome process terminated.\n");

    printf("[Chrome Monitor] Process ID (PID): %d\n",
           chrome_pid);

    printf("[Chrome Monitor] Termination type: Abnormal (Forced)\n");

    printf("[Chrome Monitor] Exit status: Unavailable\n");

    printf("[Chrome Monitor] Reason: Chrome was forcibly terminated "
           "using taskkill.exe.\n");

    printf("[Chrome Monitor] Message: Google Chrome was terminated "
           "abnormally by an external process-control command.\n");

    printf("[Chrome Monitor] Detection method: Windows "
           "tasklist.exe/taskkill.exe process monitoring.\n");
}

void display_project_concepts(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("                 PROJECT CONCEPTS\n");
    printf("============================================================\n");

    printf("\nLinux/POSIX Concepts:\n");

    printf("  fork()        - Creates a child process\n");
    printf("  execvp()      - Executes a program in the child\n");
    printf("  waitpid()     - Parent waits for child termination\n");
    printf("  exit()        - Process termination with status\n");
    printf("  getpid()      - Gets process ID\n");
    printf("  WIFEXITED()   - Checks normal termination\n");
    printf("  WEXITSTATUS() - Gets normal exit status\n");
    printf("  WIFSIGNALED() - Checks signal termination\n");
    printf("  WTERMSIG()    - Gets terminating signal number\n");

    printf("\nChrome/Windows Monitoring:\n");

    printf("  tasklist.exe  - Detects chrome.exe\n");
    printf("  PowerShell    - Launches Chrome\n");
    printf("  taskkill.exe  - Forces Chrome termination\n");

    printf("\nImportant distinction:\n");

    printf("  Linux mode uses POSIX fork(), execvp(), waitpid(),\n");
    printf("  and termination-status macros directly.\n");

    printf("\n");

    printf("  Chrome mode monitors a Windows process from WSL.\n");
    printf("  Therefore POSIX waitpid() exit status is not available\n");
    printf("  for Chrome.\n");

    printf("\n");
    printf("============================================================\n");
}

/* ---------------------------------------------------------
   Display menu
   --------------------------------------------------------- */
void display_menu(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("              PROCESS TERMINATION MESSAGES\n");
    printf("============================================================\n");
    printf("\n");

    printf("1. Monitor Linux child process\n");
    printf("2. Chrome - Normal Termination\n");
    printf("3. Chrome - Abnormal Termination\n");
    printf("4. Display project concepts\n");
    printf("5. Exit\n");

    printf("\nEnter your choice: ");
}


int main(void)
{
    int choice;

    while (1)
    {
        display_menu();

        if (scanf("%d", &choice) != 1)
        {
            printf("[Monitor] Enter a number from 1 to 5.\n");

            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
               }

            continue;
        }

       
        getchar();

        switch (choice)
        {
            case 1:
                monitor_linux_child();
                break;

            case 2:
                chrome_normal_termination();
                break;

            case 3:
                chrome_abnormal_termination();
                break;

            case 4:
                display_project_concepts();
                break;

            case 5:
                printf("\nExiting program...\n");
                return 0;

            default:
                printf("[Monitor] Enter a number from 1 to 5.\n");
        }
    }

    return 0;
}
