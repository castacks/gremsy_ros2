# PayloadSdk
This repo is officially SDK for all Gremsy's Payloads

## Hardware
- Ubuntu PC (x86_64)
- Jetson platform (aarch64)
- Raspberry Pi
- Qualcomm RB5165

## Software
This branch supported:
- Vio payload: software v2.0.0 or higher
- Zio payload: not supported yet
- GHardron payload: not supported yet
- OrusL payload: software v2.0.0 or higher

## Clone the project 
```
git clone -b payloadsdk_v3 https://github.com/Gremsy/PayloadSdk.git
```

## Hardware setup
PayloadSDK supports 2 control conections, that's configured at payloadsdk.h:

![Image](docs/images/PayloadSDK_HW_Setup.png)

**Figure 1:** Hardware setup use Ethernet or UART connection

## How to build
- Install required libraries
```bash
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt-get install libgtkmm-3.0-dev
sudo apt-get install libopencv-dev
```

**Note**: If you encounter OpenCV errors during cmake, install OpenCV:
```bash
sudo apt-get install libopencv-dev python3-opencv
```

- Build project
```bash
cd PayloadSdk
mkdir build && cd build

# Configure with payload type (choose one):
cmake -DVIO=1 ../      # For VIO payload
# OR
cmake -DMB1=1 ../      # For MB1 payload
# OR
cmake -DZIO=1 ../      # For ZIO payload
# OR
cmake -DORUSL=1 ../    # For ORUSL payload

make -j6
```

## UI Demo Application
![UI Demo Screenshot](docs/images/image.png)
A comprehensive graphical user interface application for testing and controlling Gremsy payload devices is available in the `tests/ui_demo` directory.

### Features
- **Full Camera Control**: View modes, zoom, focus, exposure, white balance, IR settings
- **Gimbal Control**: Mode selection, speed control, and angle positioning
- **Video Streaming**: Built-in RTSP stream player with interactive controls
- **Real-time Monitoring**: Storage info, capture status, GPS coordinates, telemetry data
- **Object Tracking**: Touch-based target selection and tracking control

### Building UI Demo

Build from the PayloadSDK root directory with VIO or ORUSL enabled:
```bash
cd PayloadSdk
mkdir build && cd build

# Configure with payload type (choose one):
cmake -DVIO=1 ../      # For VIO payload (required for UI Demo)
# OR
cmake -DORUSL=1 ../    # For ORUSL payload

make -j6
```

Run the application:
```bash
./tests/ui_demo/ui_demo
```

### Quick Start
1. Launch the application
2. Enter your payload IP address (default: 192.168.55.1)
   - **Important**: Change this to match your actual payload IP address
3. Click **Connect**
4. Control camera, gimbal, and view live video stream

For detailed documentation, see [tests/ui_demo/README.md](tests/ui_demo/README.md)
