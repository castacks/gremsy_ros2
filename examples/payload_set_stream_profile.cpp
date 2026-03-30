/**
 * @file payload_set_stream_resolution.cpp
 * @brief Example: Change the video stream encoder (h264) profile via the Payload SDK.
 *
 * It only support OrusL running software package v305.14 or higher
 * This example sends a command to change the h264 encoder configuration
 * This chaging only apply in runtime, and saved to the database.
 * for a specific camera stream (EO or IR). The change is applied immediately.
 *
 * 
 * Usage:
 *   ./payload_set_stream_resolution -d <device> -p <profile>
 *
 * Device mapping:
 *   -d 1  → EO Camera (Electro-Optical)
 *   -d 2  → IR Camera (Infrared)
 *
 * Allowed EO resolution level range:
 *   0: Low latency
 *   1: Balance
 *   2: High performance
 * 
 *
 * Examples:
 *   ./payload_set_stream_profile -d 1 -p 1   # Set EO stream resolution to Balance mode
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include "payloadSdkInterface.h"
#include <chrono>

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

// Signal handler to exit safely
void quit_handler(int sig);

// ---------------- Configuration structure ----------------
struct Config {
    int device = -1;   // 1 = EO, 2 = IR
    int enc_profile = -1;  // encoder profile
};

// ---------------- Argument parsing ----------------
Config parseArgs(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "d:p:h")) != -1) {
        switch (opt) {
            case 'd':
                config.device = std::atoi(optarg);
                break;
            case 'p':
                config.enc_profile = std::atoi(optarg);
                break;
            case 'h':
                std::cout << "\nUsage:\n"
                          << "  " << argv[0] << " -d <device> -p <profile>\n\n"
                          << "Options:\n"
                          << "  -d <device>   Camera device to configure:\n"
                          << "                1 = EO (Electro-Optical)\n"
                          << "                2 = IR (Infrared)\n"
                          << "  -p <profile>  The encoder profile\n\n"
                          << "Examples:\n"
                          << "  " << argv[0] << " -d 1 -p 1   # Set EO stream profile to Balance\n"
                          << "  " << argv[0] << " -d 2 -p 1   # Set IR stream profile to Balance\n"
                          << std::endl;
                std::exit(0);
                break;
            default:
                std::cerr << "\n[ERROR] Invalid arguments.\n"
                          << "Run with -h for usage help.\n";
                std::exit(1);
        }
    }

    // If missing arguments → show full help automatically
    if (config.device == -1 || config.enc_profile == -1) {
        std::cout << "\n[ERROR] Missing required arguments.\n"
                  << "\nUsage:\n"
                  << "  " << argv[0] << " -d <device> -p <profile>\n\n"
                  << "Options:\n"
                  << "  -d <device>   Camera device to configure:\n"
                  << "                1 = EO (Electro-Optical)\n"
                  << "                2 = IR (Infrared)\n"
                  << "  -p <profile>  The encoder profile\n\n"
                  << "Examples:\n"
                  << "  " << argv[0] << " -d 1 -p 1   # Set EO stream enc_profile to Balance\n"
                  << "  " << argv[0] << " -d 2 -p 1   # Set IR stream profile to Balance\n"
                  << std::endl;
        std::exit(1);
    }

    // Validate device
    if (config.device != 1 && config.device != 2) {
        std::cerr << "[ERROR] Invalid device (-d). Use 1 for EO, 2 for IR.\n";
        std::exit(1);
    }


    // Validate enc_profile
    if(config.device == 1){
        if (config.enc_profile < 0 || config.enc_profile > 2) {
            std::cerr << "[ERROR] Resolution level for EO must be between 0 (Low latency) and 2 (High performance).\n";
            std::exit(1);
        }
    }
    else if(config.device == 2){
        if (config.enc_profile < 0 || config.enc_profile > 2) {
            std::cerr << "[ERROR] Resolution level for IR must be between 0 (Low latency) and 2 (High performance).\n";
            std::exit(1);
        }
    }
    

    return config;
}

// ---------------- Main program ----------------
int main(int argc, char *argv[]) {
    printf("Starting SetStreamEncProfile example...\n");
    signal(SIGINT, quit_handler);

    Config cfg = parseArgs(argc, argv);

    // Create the Payload SDK object
    my_payload = new PayloadSdkInterface(s_conn);

    // Initialize the payload connection
    my_payload->sdkInitConnection();
    printf("Waiting for payload signal...\n");

    // Check the payload connection
    my_payload->checkPayloadConnection();

    // Print debug info
    const char* camName = (cfg.device == 1) ? "EO (Electro-Optical)" : "IR (Infrared)";
    printf("[INFO] Target camera: %s\n", camName);
    printf("[INFO] Requested encoder profile: %d\n", cfg.enc_profile);

    // Send enc_profile command
    my_payload->setPayloadStreamProfile(cfg.device, cfg.enc_profile);
    printf("[SUCCESS] Stream encoder profile for %s camera set to level %d.\n", camName, cfg.enc_profile);

    // Wait and exit
    usleep(500000); // 500 ms
    my_payload->sdkQuit();
    return 0;
}

// ---------------- Clean exit handler ----------------
void quit_handler(int sig) {
    printf("\n[INFO] Terminating at user request...\n");

    if (my_payload) {
        try {
            my_payload->sdkQuit();
        } catch (...) {}
    }

    exit(0);
}
