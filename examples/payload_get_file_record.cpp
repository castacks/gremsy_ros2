#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <chrono>
#include <termios.h>
#include <sstream>
#include <regex>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

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
std::string dir_save_record = "./";

// Signal handler to exit safely
void quit_handler(int sig);

char getch();
void handle_record_video(bool record);

void onPayloadRecordStatusChanged(int event, char* param_char, double* param_double);

bool parseRecordStatus(const char* jsonStr, RecordStatus_t& info);
size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata);
bool downloadVideo(const std::string& url, const std::string& dir_download);

int main(int argc, char *argv[]) {
    printf("Starting Get statustext Message example...\n");
    signal(SIGINT, quit_handler);

    // Create the Payload SDK object
    my_payload = new PayloadSdkInterface(s_conn);

    // Initialize the payload connection
    my_payload->sdkInitConnection();
    printf("Waiting for payload signal...\n");

    // register callback function
	my_payload->regPayloadRecordInfoChanged(onPayloadRecordStatusChanged);

    // Check the payload connection
    my_payload->checkPayloadConnection();

    // set payload to video mode for testing
	my_payload->setPayloadCameraMode(CAMERA_MODE_VIDEO);

    my_payload->setPayloadCameraParam(PAYLOAD_CAMERA_RECORD_SRC, PAYLOAD_CAMERA_RECORD_BOTH, PARAM_TYPE_UINT32);

    bool do_record = false;

    // Wait and exit
    while(!exit_flag){

        // char input = getch();
        char input;
        std::cin >> input;

        if (input == 'r'){
            do_record = !do_record;
            handle_record_video(do_record);
        }
            

        usleep(500000); // 500 ms
    }
    // my_payload->sdkQuit();

    return 0;
}

// ---------------- Clean exit handler ----------------
void quit_handler(int sig) {
    printf("\n[INFO] Terminating at user request...\n");
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

void handle_record_video(bool record){
    if(record){
        printf("Start Recording \n");
        my_payload->setPayloadCameraRecordVideoStart();
    }
    else{
        printf("Stop Recording \n");
        my_payload->setPayloadCameraRecordVideoStop();
    }
}

void onPayloadRecordStatusChanged(int event, char* param_char, double* param_double){

	switch(event){
	case PAYLOAD_RECORD_STATUS:{
        printf("\n%s\n", param_char);

        RecordStatus_t info;

        if (parseRecordStatus(param_char, info))
        {
            printf("cam_id     = %d\n", info.cam_id);
            printf("rec_status = %d\n", info.rec_status);
            printf("timestamp  = %llu\n", info.timestamp);
            printf("size       = %llu\n", info.size);
            printf("duration   = %d\n", info.duration);
            printf("url        = %s\n", info.url.c_str());

            if (info.rec_status == 1)
            {
                printf("Payload is Recording\n");
            }
            else
            {
                printf("Payload has finished recording.\n");
            }

            if(info.url != ""){
                downloadVideo(info.url, dir_save_record);
            }
        }

		break;
	}
	default: break;
	}
}

bool parseRecordStatus(const char* jsonStr, RecordStatus_t& info){
    if (jsonStr == nullptr)
        return false;

    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errs;
    std::istringstream iss(jsonStr);

    if (!Json::parseFromStream(builder, iss, &root, &errs)){
        printf("JSON parse error: %s\n", errs.c_str());
        return false;
    }

    if (!root.isObject()){
        printf("JSON root is not object\n");
        return false;
    }

    if (!root.isMember("cam_id") || !root["cam_id"].isInt()){
        printf("Missing or invalid field: cam_id\n");
        return false;
    }

    if (!root.isMember("rec_status") || !root["rec_status"].isInt()){
        printf("Missing or invalid field: rec_status\n");
        return false;
    }

    if (!root.isMember("timestamp") || !root["timestamp"].isUInt64()){
        printf("Missing or invalid field: timestamp\n");
        return false;
    }

    info.cam_id     = root["cam_id"].asInt();
    info.rec_status = root["rec_status"].asInt();
    info.timestamp  = root["timestamp"].asUInt64();

    info.size = 0;
    info.duration = 0;
    info.url.clear();

    if (info.rec_status == 1)
        return true;

    if (info.rec_status == 0){
        if (!root.isMember("size") || !root["size"].isUInt64())
        {
            printf("Missing or invalid field: size\n");
            return false;
        }

        if (!root.isMember("duration") || !root["duration"].isInt())
        {
            printf("Missing or invalid field: duration\n");
            return false;
        }

        if (!root.isMember("url") || !root["url"].isString())
        {
            printf("Missing or invalid field: url\n");
            return false;
        }

        info.size     = root["size"].asUInt64();
        info.duration = root["duration"].asInt();
        info.url      = root["url"].asString();

        return true;
    }

    printf("Invalid rec_status: %d\n", info.rec_status);
    return false;
}

size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata){
    FILE* fp = static_cast<FILE*>(userdata);
    return fwrite(ptr, size, nmemb, fp);
}

bool downloadVideo(const std::string& url, const std::string& dir_download){
    size_t pos = url.find_last_of('/');
    std::string file_name =
        (pos != std::string::npos) ?
        url.substr(pos + 1) :
        "download.bin";

    std::string file_path = dir_download;

    if (!file_path.empty() && file_path.back() != '/')
        file_path += '/';

    file_path += file_name;

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        printf("curl_easy_init failed\n");
        return false;
    }

    FILE* fp = fopen(file_path.c_str(), "wb");
    if (!fp)
    {
        printf("Cannot create file: %s\n", file_path.c_str());
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);

    CURLcode res = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        printf("Download failed: %s\n",
               curl_easy_strerror(res));
        return false;
    }

    printf("Download success: %s\n", file_path.c_str());
    return true;
}