// function to implement correct readline in the server
ssize_t readLine(int fd, char* buffer, size_t maxLength) {
    ssize_t bytesRead = 0;
    ssize_t totalBytesRead = 0;
    char ch;

    while (totalBytesRead < maxLength - 1) {
        ssize_t bytesRead = read(fd, &ch, 1);

        if (bytesRead < 0) {
            return bytesRead; // Error
        } else if (bytesRead == 0 || ch == '\n') {
            break; // End of file or end of line
        } else {
            buffer[totalBytesRead] = ch;
            totalBytesRead++;
        }
    }

    buffer[totalBytesRead] = '\0'; // Null-terminate the string
    return totalBytesRead;
}