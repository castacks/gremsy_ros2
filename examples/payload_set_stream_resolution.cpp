/**
 * @file payload_set_stream_resolution.cpp
 * @brief Example: Change the video stream resolution via the Payload SDK.
 *
 * It only support OrusL running software package v305.3 or higher
 * This example sends a command to change the resolution configuration
 * This chaging only apply in runtime, and does not saved to the database.
 * for a specific camera stream (EO or IR). The change is applied immediately.
 *
 * Does not support IR camera for now. It will be fixed at 640x512
 * 
 * Usage:
 *   ./payload_set_stream_resolution -d <device> -r <resolution level>
 *
 * Device mapping:
 *   -d 1  → EO Camera (Electro-Optical)
 *   -d 2  → IR Camera (Infrared)
 *
 * Allowed resolution level range:
 *   1: 1920x1080
 *   2: 1280x720
 *   3: 960x540
 *
 * Examples:
 *   ./payload_set_stream_resolution -d 1 -r 1   # Set EO stream resolution to 1920x1080
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
    int resolution = -1;  // resolution level
};

// ---------------- Argument parsing ----------------
Config parseArgs(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "d:r:h")) != -1) {
        switch (opt) {
            case 'd':
                config.device = std::atoi(optarg);
                break;
            case 'r':
                config.resolution = std::atoi(optarg);
                break;
            case 'h':
                std::cout << "\nUsage:\n"
                          << "  " << argv[0] << " -d <device> -r <resolution level>\n\n"
                          << "Options:\n"
                          << "  -d <device>   Camera device to configure:\n"
                          << "                1 = EO (Electro-Optical)\n"
                          << "                2 = IR (Infrared)\n"
                          << "  -r <resolution level>  The resolution in level\n\n"
                          << "Examples:\n"
                          << "  " << argv[0] << " -d 1 -r 1   # Set EO stream resolution to 1920x1080\n"
                          << "  " << argv[0] << " -d 1 -r 2   # Set EO stream resolution to 1280x720\n"
                          << "  " << argv[0] << " -d 1 -r 3   # Set EO stream resolution to 960x540\n"
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
    if (config.device == -1 || config.resolution == -1) {
        std::cout << "\n[ERROR] Missing required arguments.\n"
                  << "\nUsage:\n"
                  << "  " << argv[0] << " -d <device> -r <resolution level>\n\n"
                  << "Options:\n"
                  << "  -d <device>   Camera device to configure:\n"
                  << "                1 = EO (Electro-Optical)\n"
                  << "                2 = IR (Infrared)\n"
                  << "  -r <resolution level>  The resolution in level\n\n"
                  << "Notes: It is not support for IR camera now\n\n"
                  << "Examples:\n"
                  << "  " << argv[0] << " -d 1 -r 1   # Set EO stream resolution to 1920x1080\n"
                  << "  " << argv[0] << " -d 1 -r 2   # Set EO stream resolution to 1280x720\n"
                  << "  " << argv[0] << " -d 1 -r 3   # Set EO stream resolution to 960x540\n"
                  << std::endl;
        std::exit(1);
    }

    // Validate device
    if (config.device != 1 && config.device != 2) {
        std::cerr << "[ERROR] Invalid device (-d). Use 1 for EO, 2 for IR.\n";
        std::exit(1);
    }

    // Validate bitrate
    if (config.resolution < 1 || config.resolution > 3) {
        std::cerr << "[ERROR] Resolution level must be between 1 (1920x1080) and 3 (960x540).\n";
        std::exit(1);
    }

    return config;
}

// ---------------- Main program ----------------
int main(int argc, char *argv[]) {
    printf("Starting SetStreamResolution example...\n");
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
    printf("[INFO] Requested resolution level: %d\n", cfg.resolution);

    // Send resolution command
    my_payload->setPayloadStreamResolution(cfg.device, cfg.resolution);
    printf("[SUCCESS] Stream resolution for %s camera set to level %d.\n", camName, cfg.resolution);

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
