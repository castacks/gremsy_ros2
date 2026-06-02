#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <chrono>

#include "payloadSdkInterface.h"

// Pointer to SDK interface
PayloadSdkInterface* my_payload = nullptr;

// Connection info (UART or UDP)
#if (CONTROL_METHOD == CONTROL_UART)
T_ConnInfo s_conn = {
    CONTROL_UART,
    payload_uart_port,
    payload_uart_baud
};
#else
T_ConnInfo s_conn = {
    CONTROL_UDP,
    udp_ip_target,
    udp_port_target
};
#endif

bool exit_flag = false;

// Signal handler to exit safely
void quit_handler(int sig);
char getch();
void printCommandMenu();

int main(int argc, char *argv[]) {
    SDK_LOG("Starting Set Standby Mode example...");
    signal(SIGINT, quit_handler);

    // Create the Payload SDK object
    my_payload = new PayloadSdkInterface(s_conn);

    // Initialize the payload connection
    my_payload->sdkInitConnection();
    SDK_LOG("Waiting for payload signal...");

    // Check the payload connection
    my_payload->checkPayloadConnection();

    printCommandMenu();

    // Wait and exit
    while(!exit_flag){
        char input;
        std::cin >> input;

        if (input == '0'){
            // Off Standby
            SDK_LOG("Set Standby Mode: OFF");
            my_payload->setPayloadStandbyMode(false);
        }
        else if (input == '1'){
            // On Standby
            SDK_LOG("Set Standby Mode: ON");
            my_payload->setPayloadStandbyMode(true);
        }
        else if (input == 'q'){
            // Quit
            SDK_LOG("Quit example");
            break;
        }
        else{
            SDK_LOG("Input invalid");
        }
        printCommandMenu();

        usleep(500000); // 500 ms
    }
    my_payload->sdkQuit();

    return 0;
}

// ---------------- Clean exit handler ----------------
void quit_handler(int sig) {
    SDK_LOG("\n[INFO] Terminating at user request...");
    exit_flag = true;

    if (my_payload != nullptr) {
        try {
            my_payload->sdkQuit();
        } catch (...) {}
    }

    exit(0);
}

char getch(){
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

void printCommandMenu(){
    printf("\n");
    SDK_LOG("=== Command Menu ===");
    SDK_LOG("Press '0': Disable Standby Mode");
    SDK_LOG("Press '1': Enable Standby Mode");
    SDK_LOG("Press 'q': Quit example \n");
}