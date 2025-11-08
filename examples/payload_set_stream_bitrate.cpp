/**
 * @file payload_set_stream_bitrate.cpp
 * @brief Example: Change the video stream bitrate via the Payload SDK.
 *
 * This example sends a command to change the bitrate configuration
 * for a specific camera stream (EO or IR). The change is applied immediately.
 *
 * Usage:
 *   ./payload_set_stream_bitrate -d <device> -b <bitrate>
 *
 * Device mapping:
 *   -d 1  → EO Camera (Electro-Optical)
 *   -d 2  → IR Camera (Infrared)
 *
 * Allowed bitrate range:
 *   512 kbps → 16 Mbps (512000 → 16000000 bps)
 *
 * Examples:
 *   ./payload_set_stream_bitrate -d 1 -b 4000000   # Set EO camera bitrate to 4 Mbps
 *   ./payload_set_stream_bitrate -d 2 -b 2000000   # Set IR camera bitrate to 2 Mbps
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
    int bitrate = -1;  // bitrate value (bps)
};

// ---------------- Argument parsing ----------------
Config parseArgs(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "d:b:h")) != -1) {
        switch (opt) {
            case 'd':
                config.device = std::atoi(optarg);
                break;
            case 'b':
                config.bitrate = std::atoi(optarg);
                break;
            case 'h':
                std::cout << "\nUsage:\n"
                          << "  " << argv[0] << " -d <device> -b <bitrate>\n\n"
                          << "Options:\n"
                          << "  -d <device>   Camera device to configure:\n"
                          << "                1 = EO (Electro-Optical)\n"
                          << "                2 = IR (Infrared)\n"
                          << "  -b <bitrate>  Bitrate in bps (512000 to 16000000)\n\n"
                          << "Examples:\n"
                          << "  " << argv[0] << " -d 1 -b 4000000   # Set EO camera bitrate to 4 Mbps\n"
                          << "  " << argv[0] << " -d 2 -b 2000000   # Set IR camera bitrate to 2 Mbps\n"
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
    if (config.device == -1 || config.bitrate == -1) {
        std::cout << "\n[ERROR] Missing required arguments.\n"
                  << "\nUsage:\n"
                  << "  " << argv[0] << " -d <device> -b <bitrate>\n\n"
                  << "Options:\n"
                  << "  -d <device>   Camera device to configure:\n"
                  << "                1 = EO (Electro-Optical)\n"
                  << "                2 = IR (Infrared)\n"
                  << "  -b <bitrate>  Bitrate in bps (512000 to 16000000)\n\n"
                  << "Examples:\n"
                  << "  " << argv[0] << " -d 1 -b 4000000   # Set EO camera bitrate to 4 Mbps\n"
                  << "  " << argv[0] << " -d 2 -b 2000000   # Set IR camera bitrate to 2 Mbps\n"
                  << std::endl;
        std::exit(1);
    }

    // Validate device
    if (config.device != 1 && config.device != 2) {
        std::cerr << "[ERROR] Invalid device (-d). Use 1 for EO, 2 for IR.\n";
        std::exit(1);
    }

    // Validate bitrate
    if (config.bitrate < 512000 || config.bitrate > 16000000) {
        std::cerr << "[ERROR] Bitrate must be between 512000 and 16000000 bps.\n";
        std::exit(1);
    }

    return config;
}

// ---------------- Main program ----------------
int main(int argc, char *argv[]) {
    printf("Starting SetStreamBitrate example...\n");
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
    printf("[INFO] Requested bitrate: %d bps\n", cfg.bitrate);

    // Send bitrate command
    my_payload->setPayloadStreamBitrate(cfg.device, cfg.bitrate);
    printf("[SUCCESS] Stream bitrate for %s camera set to %d bps.\n", camName, cfg.bitrate);

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
