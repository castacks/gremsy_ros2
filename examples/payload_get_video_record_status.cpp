/**
 * This is an example on how to get the status of the video recording and download the video file after it finished.
 * We will use the STATUSTEXT message for the status
 * 
 * The steps will be:
 * 1. 
 **/

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
bool is_recording = false;

// Signal handler to exit safely
void quit_handler(int sig);

char getch();
void handle_record_video(bool power);

std::string replaceString(std::string str, const std::string& from, const std::string& to);

void onPayloadRecordStatusChanged(int event, char* param_char, double* param_double);

bool parseRecordStatus(const char* jsonStr, RecordStatus_t& info);
size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata);
bool downloadVideo(const std::string& url, const std::string& dir_download);

int main(int argc, char *argv[]) {
    SDK_LOG("Starting Get statustext Message example...");
    signal(SIGINT, quit_handler);

    // Create the Payload SDK object
    my_payload = new PayloadSdkInterface(s_conn);

    // Initialize the payload connection
    my_payload->sdkInitConnection();
    SDK_LOG("Waiting for payload signal...");

    // register callback function
	my_payload->regPayloadRecordInfoChanged(onPayloadRecordStatusChanged);

    // Check the payload connection
    my_payload->checkPayloadConnection();

    // set payload to video mode for testing
	my_payload->setPayloadCameraMode(CAMERA_MODE_VIDEO);

    my_payload->setPayloadCameraParam(PAYLOAD_CAMERA_RECORD_SRC, PAYLOAD_CAMERA_RECORD_BOTH, PARAM_TYPE_UINT32);

    SDK_LOG("\n[INPUT] Press r to start the video record");

    // Wait and exit
    while(!exit_flag){

        // char input = getch();
        char input;
        std::cin >> input;

        if (input == 'r'){
            // starting the video reocrd
            handle_record_video(1);
        }
        else if (input == 's'){
            // stop the video record
            handle_record_video(0);
        }
            

        usleep(500000); // 500 ms
    }
    // my_payload->sdkQuit();

    return 0;
}

// ---------------- Clean exit handler ----------------
void quit_handler(int sig) {
    SDK_LOG("[INFO] Terminating at user request...");
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

void handle_record_video(bool power){
    if(power){
        if(is_recording){
            SDK_LOG("[INFO] The recording is running. Skip");
            return;
        }

        SDK_LOG("[INFO] Start Recording");
        my_payload->setPayloadCameraRecordVideoStart();
        is_recording = true;
    }
    else{
        if(!is_recording){
            SDK_LOG("[INFO] The recording is not running. Skip");
            return;
        }

        SDK_LOG("[INFO] Stop Recording");
        my_payload->setPayloadCameraRecordVideoStop();
        is_recording = false;
    }
}

static int _record_status_prev = -1;
static int _cam_rec_status_prev = -1;
static int _cam_rec_time_prev = -1;
void onPayloadRecordStatusChanged(int event, char* param_char, double* param_double){

	switch(event){
	case PAYLOAD_RECORD_STATUS:{
        RecordStatus_t info;

        if (parseRecordStatus(param_char, info))
        {
            // check if the status changed
            if((info.rec_status != _cam_rec_status_prev)
                || (info.rec_time != _cam_rec_time_prev))
            {
                _cam_rec_status_prev = info.rec_status;
                _cam_rec_time_prev = info.rec_time;     // for print the debug when the camera is recording

                // print the raw input
                SDK_LOG("%s", param_char);

                if(info.rec_status ==  1)
                {
                    // the camera is recording the video

                    // store the flag for checking the stop
                    _record_status_prev = info.rec_status;

                    SDK_LOG("rec_src    = %d", info.rec_src);
                    SDK_LOG("rec_status = %d", info.rec_status);
                    SDK_LOG("rec_time   = %d", info.rec_time);
                    SDK_LOG("timestamp  = %llu", info.timestamp);
                    SDK_LOG("size       = %llu", info.size);
                    SDK_LOG("duration   = %d", info.duration);
                    SDK_LOG("url        = %s", info.url.c_str());

                    if (info.rec_status == 1)
                    {
                        SDK_LOG("Payload is Recording");
                        is_recording = true;

                        SDK_LOG("\n[INPUT] Press s to stop the video record");
                    }
                    else
                    {
                        SDK_LOG("Payload has finished recording.");

                        SDK_LOG("Start downloading the video file at %s", info.url.c_str());
                    }
                }
                else if(info.rec_status == 0)
                {
                    // if the camera stoped record

                    if(_record_status_prev == 1){
                        // if the camera is recording then stop
                        // check to download the files

                        if(info.url != ""){
                            SDK_LOG("Start to downlading the video files");

                            downloadVideo(info.url, dir_save_record);

                            if(info.rec_src == PAYLOAD_CAMERA_RECORD_BOTH){
                                std::string ir_url = replaceString(info.url, "VID_EO_", "VID_IR_");
                                if (ir_url != "")
                                    downloadVideo(ir_url, dir_save_record);
                            }

                            SDK_LOG("Exit");
                            exit(0);
                        }
                        else{
                            SDK_LOG("[ERROR] Can not download. The URL is empty");
                            SDK_LOG("Exit");
                            exit(-1);
                        }
                    }
                }
            }
        }
        else{
            SDK_LOG("[ERROR] Can not parse the STATUSTEXT.");
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
        SDK_LOG("JSON parse error: %s", errs.c_str());
        return false;
    }

    if (!root.isObject()){
        SDK_LOG("JSON root is not object");
        return false;
    }

    if (!root.isMember("rec_src") || !root["rec_src"].isInt()){
        SDK_LOG("Missing or invalid field: rec_src");
        return false;
    }

    if (!root.isMember("rec_status") || !root["rec_status"].isInt()){
        SDK_LOG("Missing or invalid field: rec_status");
        return false;
    }
    
    if (!root.isMember("rec_time") || !root["rec_time"].isInt()){
        SDK_LOG("Missing or invalid field: rec_time");
        return false;
    }

    if (!root.isMember("timestamp") || !root["timestamp"].isUInt64()){
        SDK_LOG("Missing or invalid field: timestamp");
        return false;
    }

    info.rec_src    = root["rec_src"].asInt();
    info.rec_status = root["rec_status"].asInt();
    info.rec_time   = root["rec_time"].asInt();
    info.timestamp  = root["timestamp"].asUInt64();

    info.size = 0;
    info.duration = 0;
    info.url.clear();

    if (info.rec_status == 1)
        return true;

    if (info.rec_status == 0){
        if (!root.isMember("size") || !root["size"].isUInt64())
        {
            SDK_LOG("Missing or invalid field: size");
            return false;
        }

        if (!root.isMember("duration") || !root["duration"].isInt())
        {
            SDK_LOG("Missing or invalid field: duration");
            return false;
        }

        if (!root.isMember("url") || !root["url"].isString())
        {
            SDK_LOG("Missing or invalid field: url");
            return false;
        }

        info.size     = root["size"].asUInt64();
        info.duration = root["duration"].asInt();
        info.url      = root["url"].asString();

        return true;
    }

    SDK_LOG("Invalid rec_status: %d", info.rec_status);
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
        SDK_LOG("curl_easy_init failed");
        return false;
    }

    FILE* fp = fopen(file_path.c_str(), "wb");
    if (!fp)
    {
        SDK_LOG("Cannot create file: %s", file_path.c_str());
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
        SDK_LOG("Download failed: %s",
               curl_easy_strerror(res));
        
        SDK_LOG("Exit with error");
        exit(-1);
        
        return false;
    }

    SDK_LOG("Download success: %s", file_path.c_str());
    
    return true;
}

std::string replaceString(std::string str, const std::string& from, const std::string& to){
    auto pos = str.find(from);
    if (pos == std::string::npos){
        SDK_LOG("Cannot find '%s' in '%s'", from.c_str(), str.c_str());
        return "";
    }

    str.replace(pos, from.length(), to);
    return str;
}