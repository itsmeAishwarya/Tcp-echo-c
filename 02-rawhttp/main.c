#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {

    // Step 0: Check if user provided hostname
    // Example usage: ./rawhttp example.com
    if (argc != 2) {
        printf("Usage: ./rawhttp <hostname>\n");
        printf("Example: ./rawhttp example.com\n");
        return 1;
    }

    char *hostname = argv[1];
    char request[BUFFER_SIZE];
    char full_response[BUFFER_SIZE * 8];
    int sock_fd;

    // Step 1: Resolve hostname to IP address using DNS
    // example.com -> 93.184.216.34
    struct hostent *server = gethostbyname(hostname);

    if (server == NULL) {
        printf("Error: could not resolve host '%s'\n", hostname);
        return 1;
    }

    // Print resolved IP address
    printf("Resolved %s -> %s\n",
           hostname,
           inet_ntoa(*(struct in_addr*)server->h_addr));

    // Step 2: Create TCP socket
    // AF_INET     -> IPv4
    // SOCK_STREAM -> TCP
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    // Step 3: Prepare server address structure
    struct sockaddr_in server_addr;

    // Use IPv4
    server_addr.sin_family = AF_INET;

    // HTTP servers listen on port 80
    server_addr.sin_port = htons(80);

    // Copy server IP into address structure
    memcpy(
        &server_addr.sin_addr.s_addr,
        server->h_addr,
        server->h_length
    );

    // Step 4: Connect to the web server
    if (connect(
            sock_fd,
            (struct sockaddr*)&server_addr,
            sizeof(server_addr)) < 0) {

        perror("connect");
        return 1;
    }

    printf("Connected to %s:80\n", hostname);

    // Step 5: Build HTTP GET request manually
    snprintf(
        request,
        sizeof(request),

        "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",

        hostname
    );

    printf("\n--- SENDING REQUEST ---\n%s", request);

    // Step 6: Send HTTP request to server
    send(
        sock_fd,
        request,
        strlen(request),
        0
    );

    // Step 7: Receive complete HTTP response
    int total = 0;
    int bytes;

    while ((bytes = recv(
                sock_fd,
                full_response + total,
                sizeof(full_response) - total - 1,
                0)) > 0) {

        total += bytes;
    }

    // Convert received data into proper C string
    full_response[total] = '\0';

    // Step 8: Separate HTTP headers and body
    // Headers and body are separated by "\r\n\r\n"
    char *header_end = strstr(
        full_response,
        "\r\n\r\n"
    );

    if (header_end == NULL) {
        printf("Malformed response\n");
        close(sock_fd);
        return 1;
    }

    // Split into two strings
    *header_end = '\0';

    // Body starts after the separator
    char *body = header_end + 4;

    // Step 9: Print HTTP status line
    // Example: HTTP/1.1 200 OK
    printf("\n=== STATUS ===\n");

    char *line = strtok(
        full_response,
        "\r\n"
    );

    printf("%s\n", line);

    // Step 10: Print all HTTP headers
    printf("\n=== HEADERS ===\n");

    while ((line = strtok(NULL, "\r\n")) != NULL) {
        printf("  %s\n", line);
    }

    // Step 11: Print webpage body (usually HTML)
    printf("\n=== BODY ===\n%s\n", body);

    printf("=== TOTAL BYTES RECEIVED: %d ===\n", total);

    // Step 12: Close TCP connection
    close(sock_fd);

    return 0;
}
