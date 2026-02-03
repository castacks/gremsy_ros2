#include "stdio.h"
#include <pthread.h>
#include <cstdlib>
#include <string>
using namespace std;

#include"payloadSdkInterface.h"

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

PayloadSdkInterface* my_payload = nullptr;

pthread_t thrd_recv;
pthread_t thrd_gstreamer;

bool gstreamer_start();
void gstreamer_terminate();
void *start_loop_thread(void *threadid);


bool all_threads_init();
void quit_handler(int sig);

struct Config {
    float pitch = 0.0f;
    float yaw   = 0.0f;
    bool pitch_set = false;
    bool yaw_set   = false;
};

Config parseArgs(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "p:y:")) != -1) {
        switch (opt) {
            case 'p': {
                char* end;
                config.pitch = std::strtof(optarg, &end);
                if (*end != '\0') {
                    std::cerr << "Invalid pitch value\n";
                    std::exit(1);
                }
                config.pitch_set = true;
                break;
            }
            case 'y': {
                char* end;
                config.yaw = std::strtof(optarg, &end);
                if (*end != '\0') {
                    std::cerr << "Invalid yaw value\n";
                    std::exit(1);
                }
                config.yaw_set = true;
                break;
            }
            default:
                std::cerr << "Usage: " << argv[0] << " -p <pitch> -y <yaw>\n";
                std::exit(1);
        }
    }

    if (!config.pitch_set || !config.yaw_set) {
        std::cerr << "Error: both -p and -y options are required.\n";
        std::cerr << "Usage: " << argv[0] << " -p <pitch> -y <yaw>\n";
        std::exit(1);
    }

    return config;
}

void onPayloadStatusChanged(int event, double* param){
	// printf("%s %d \n", __func__, event);
	switch(event){
    case PAYLOAD_GB_ATTITUDE:{
		// param[0]: pitch
		// param[1]: roll
		// param[2]: yaw

		printf("Mount Orient : Pich: %.2f - Roll: %.2f - Yaw: %.2f\n", param[0], param[1], param[2]);
		break;
	}
	default: break;
	}
}

int main(int argc, char *argv[]){
	printf("Starting Set gimbal mode example...\n");
	signal(SIGINT,quit_handler);

	Config cfg = parseArgs(argc, argv);

	// create payloadsdk object
	my_payload = new PayloadSdkInterface(s_conn);

	// init payload
	my_payload->sdkInitConnection();
	printf("Waiting for payload signal! \n");

	my_payload->checkPayloadConnection();
	usleep(100000);

	printf("Set gimbal to FOLLOW mode \n");
	my_payload->setPayloadCameraParam(PAYLOAD_CAMERA_GIMBAL_MODE, PAYLOAD_CAMERA_GIMBAL_MODE_FOLLOW, PARAM_TYPE_UINT32);
	usleep(100000);

    // register function to recieve MAVlink msg ID : MAVLINK_MSG_ID_MOUNT_ORIENTATION
    my_payload->regPayloadStatusChanged(onPayloadStatusChanged);

	printf("Move gimbal pitch to %.2f deg, yaw to %.2f deg\n", cfg.pitch, cfg.yaw);
#if 0
	// for control the gimbal using MAVLink protocol v2
	// has limitation for yaw (-180:180), for pitch(-90:90)
	my_payload->setGimbalSpeed(cfg.pitch, 0 , cfg.yaw, INPUT_ANGLE);

#else
	// use this function to control the gimbal using command_long
	// no limitation range
	// only comaptible with the payload software v3.1.x and gimbal firmware v7.8.9 or higher
	my_payload->setGimbalMovement(cfg.pitch, 0 , cfg.yaw, INPUT_ANGLE);
#endif
	usleep(5000000);

	// close payload interface
	try {
		my_payload->sdkQuit();
	}
	catch (int error){}

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