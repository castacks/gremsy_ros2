#include "stdio.h"
#include"payloadSdkInterface.h"

PayloadSdkInterface* my_payload = nullptr;
bool time_to_exit = false;

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
void onPayloadInfoChanged(int event, std::vector<std::string> info);

int main(int argc, char *argv[]){
	printf("Starting Zoom camera to specific level example...\n");
	signal(SIGINT,quit_handler);

	// creat payloadsdk object
	my_payload = new PayloadSdkInterface(s_conn);

	// init payload
	my_payload->sdkInitConnection();
	printf("Waiting for payload signal! \n");

	my_payload->regPayloadInfoChanged(onPayloadInfoChanged);

	// check connection
	my_payload->checkPayloadConnection();

	// check payload messages
	while(1){

        printf("[EO] Switching to Combine zoom mode for extended range...\n");
        my_payload->setPayloadCameraParam(PAYLOAD_CAMERA_VIDEO_ZOOM_MODE, PAYLOAD_CAMERA_VIDEO_ZOOM_MODE_COMBINE, PARAM_TYPE_UINT32);
        usleep(1000000);

        // zoom EO camera to 1x
        printf("[EO] zoom EO camera to 1x \n");
        my_payload->setCameraZoomTarget(1.0);
        usleep(5000000);
        
        // zoom EO camera to 13.5x
        printf("[EO] zoom EO camera to 13.5x \n");
        my_payload->setCameraZoomTarget(13.5);
        usleep(5000000);

        // zoom EO camera to 25.0x
        printf("[EO] zoom EO camera to 25.0x \n");
        my_payload->setCameraZoomTarget(25.0);
        usleep(5000000);

        // zoom EO camera to 33.5x
        printf("[EO] zoom EO camera to 33.5x \n");
        my_payload->setCameraZoomTarget(33.5);
        usleep(5000000);

        // zoom EO camera to 203.5x
        printf("[EO] zoom EO camera to 203.5x \n");
        my_payload->setCameraZoomTarget(233.5);
        usleep(5000000);

        // zoom EO camera to 300x
        printf("[EO] zoom EO camera to 300x \n");
        my_payload->setCameraZoomTarget(300.0);
        usleep(5000000);
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

void onPayloadInfoChanged(int event, std::vector<std::string> info){
    
    switch(event){
    case PAYLOAD_COMP_INFO:{
        printf(" --> Got Component Info,\n");
        printf(" -->  Model name: %s,\n", info[0].c_str());
        printf(" -->  Software version: %s,\n", info[1].c_str());
        printf(" -->  Serial number: %s,\n", info[2].c_str());
        break;
    }
    default: break;
    }
}