[4:03 PM] Satyan Pradhan
#ifndef CLIENT_H 

#define CLIENT_H 

#include <stdio.h> 

#include <stdlib.h> 

#include <unistd.h> 

#include <string.h> 

#include <sys/types.h> 

#include <sys/socket.h> 

#include <arpa/inet.h> 

#include <netinet/in.h> 

#include <sys/stat.h> 

#include <mqueue.h> 

#include <time.h> 

#include <errno.h> 

#include <netdb.h> 

#define MAX_LOG_SIZE 80 

typedef struct { 

    char timestamp[25]; 

    int milliseconds; 

    char clientID[16]; 

    int appID; 

    char category[16]; 

    int errorCode; 

    char errorText[MAX_LOG_SIZE + 1]; 

} ErrorLog; 

void getCurrentTime(char* dateTimeString, int* milliseconds); 

void getIPAddress(char* ipAddress); 

void add_error_to_error_log(ErrorLog log); 


void getCurrentTime(char* dateTimeString, int* milliseconds) { 

    time_t currentTime = time(NULL); 

    struct tm* timeInfo = localtime(&currentTime); 

    strftime(dateTimeString, 25, "%d/%m/%Y; %H:%M:%S", timeInfo); 

    struct timespec currentTimeSpec; 

    clock_gettime(CLOCK_REALTIME, &currentTimeSpec); 

    *milliseconds = currentTimeSpec.tv_nsec / 1000000; 

} 

void getIPAddress(char* ipAddress) { 

    char hostName[256]; 

    gethostname(hostName); 

    struct hostent* hostEntry; 

    hostEntry = gethostbyname(hostName); 

    strcpy(ipAddress, inet_ntoa(*((struct in_addr*)hostEntry->h_addr_list[0]))); 

} 

void add_error_to_error_log(ErrorLog log) { 

    int client_socket; 

    struct sockaddr_in server_address; 

    // Create a UDP socket 

    client_socket = socket(AF_INET, SOCK_DGRAM, 0); 

    if (client_socket < 0) { 

        perror("Socket creation failed"); 

        return; 

    } 

    // Set up server address 

    server_address.sin_family = AF_INET; 

    server_address.sin_port = htons(4433); 

    server_address.sin_addr.s_addr = inet_addr("10.75.113.79"); 

    // Send log message to server 

    sendto(client_socket, &log, sizeof(ErrorLog), 0, (struct sockaddr*)&server_address, sizeof(server_address)); 

    close(client_socket); 

} 


#endif /* CLIENT_H */
