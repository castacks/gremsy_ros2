/**
 * This sample will send the command to change the config for the bitrate of the video stream
 * The change will be applied immediately
 **/

#include "stdio.h"
#include"payloadSdkInterface.h"

#include <iostream>
#include <chrono>

PayloadSdkInterface* my_payload = nullptr;

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

void quit_handler(int sig);

struct Config {
    int device = -1;
    int bitrate = -1;
};

// Function to parse command-line arguments
Config parseArgs(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "d:b:")) != -1) {
        switch (opt) {
            case 'd':
                config.device = std::atoi(optarg);
                break;
            case 'b':
                config.bitrate = std::atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -d <device> -b <bitrate>\n";
                std::exit(1);
        }
    }

    if (config.device == -1 || config.bitrate == -1) {
        std::cerr << "Error: both -d and -b options are required.\n";
        std::cerr << "Usage: " << argv[0] << " -d <device> -b <bitrate>\n";
        std::exit(1);
    }

    return config;
}

int main(int argc, char *argv[]){
	printf("Starting SendSystemTime example...\n");
	signal(SIGINT,quit_handler);

	Config cfg = parseArgs(argc, argv);

	// creat payloadsdk object
	my_payload = new PayloadSdkInterface(s_conn);

	// init payload
	my_payload->sdkInitConnection();
	printf("Waiting for payload signal! \n");

	// check connection
	my_payload->checkPayloadConnection();

	int msg_cnt = 0;
	int boot_time_ms = 0;

	while(1){
		
		my_payload->setPayloadStreamBitrate(cfg.device, cfg.bitrate);

		printf("The stream bitrate was set to %d. Exit \n", cfg.bitrate);

		usleep(1000000); // 1000ms, 1Hz
		exit(0);
	}

	return 0;
}

void quit_handler( int sig ){
    printf("\n");
    printf("TERMINATING AT USER REQUEST \n");
    printf("\n");

    // close payload interface
    try {
        my_payload->sdkQuit();
    }
    catch (int error){}

    // end program here
    exit(0);
}