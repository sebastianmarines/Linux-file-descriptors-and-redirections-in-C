#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    // Message to write to stdout (file descriptor 1)
    const char *stdoutMessage = "This message is to stdout\n";
    ssize_t written = write(STDOUT_FILENO, stdoutMessage, strlen(stdoutMessage));
    if (written == -1) {
        perror("write to stdout");
        return 1;
    }

    // Message to write to stderr (file descriptor 2)
    const char *stderrMessage = "This message is to stderr\n";
    written = write(STDERR_FILENO, stderrMessage, strlen(stderrMessage));
    if (written == -1) {
        perror("write to stderr");
        return 1;
    }

    return 0;
}

