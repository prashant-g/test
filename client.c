#include <stdio.h> 
#include <unistd.h> 
#include <signal.h> 
#include <sys/types.h> 
#include "client2.h" 
void logm(int signum) { 
    ErrorLog log; 
    getCurrentTime(log.timestamp, &log.milliseconds); 
    getIPAddress(log.clientID); 
    log.appID = getpid(); 
    strcpy(log.category, "WARNING"); 
    log.errorCode = 1; 
    strcpy(log.errorText, "ECU AT HIGH"); 
    add_error_to_error_log(log); 
} 
int main() { 
    signal(SIGINT, logm); 
    while (1) { 
        printf("HII\n"); 
        sleep(1); 
    } 
    return 0; 
}
