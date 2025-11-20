# UI Demo Application

A comprehensive graphical user interface (GUI) application for testing and controlling Gremsy payload devices. This demo provides an intuitive interface for managing camera settings, gimbal controls, video streaming, and monitoring payload status in real-time.

## Overview

The UI Demo is built using GTK+ 3.0 (gtkmm) and GStreamer, providing a native desktop application for Linux platforms. It demonstrates the full capabilities of the PayloadSDK through an interactive control panel.

## Features

### Camera Control
- **View Mode Selection**: Switch between EO/IR, EO, IR, IR/EO, SYNC, and SBS (Side-by-Side) modes
- **Recording Controls**: 
  - Capture images
  - Start/stop video recording
  - Select recording source (EO, IR, BOTH, OSD)
- **Zoom Controls**:
  - Continuous zoom control
  - Step zoom control
  - Range zoom control
  - Adjustable zoom speed
- **Focus Controls**:
  - Auto focus
  - Continuous focus control
  - Adjustable focus speed
- **Exposure Settings**:
  - Auto/Manual exposure modes
  - Shutter speed adjustment (1/1 to 1/10000)
  - Iris/Aperture control (F2.0 to F11)
  - Gain adjustment (0 dB to 36 dB)
- **White Balance**: Auto, Indoor, Outdoor, One Push WB, ATW, Manual modes
- **IR Camera Settings**:
  - Palette selection (10 different palettes)
  - FFC (Flat Field Correction) mode: Manual/Auto
  - FFC trigger
  - Temperature monitoring (Min/Max/Mean)
- **Additional Features**:
  - LRF (Laser Range Finder) mode control
  - OSD (On-Screen Display) mode selection
  - Image flip control
  - Object tracking and detection

### Gimbal Control
- **Gimbal Modes**: OFF, LOCK, FOLLOW, MAPPING
- **Speed Control**: Real-time pan and tilt speed adjustment
- **Angle Control**: Precise pitch, roll, and yaw positioning
- **Attitude Display**: Live gimbal attitude feedback

### Video Streaming
- **RTSP Stream Playback**: Built-in video player using GStreamer
- **Real-time Display**: Low-latency video preview
- **Interactive Touch Control**: Click on video to set tracking target position
- **Auto RTSP URL Generation**: Automatically generates RTSP URL from payload IP address

### Status Monitoring
- **Storage Information**: Total, used, and available capacity
- **Capture Status**: Image capture and video recording status
- **GPS Coordinates**: Payload and target GPS information
- **Telemetry Data**: Real-time sensor data display

## Architecture

### Main Components

```
ui_demo/
├── main.cpp                 # Application entry point and SDK interface
├── main.h                   # Main header and configuration
├── CMakeLists.txt          # Build configuration
└── ui_gtk/                 # GTK UI components
    ├── MainWindow.cpp      # Main application window
    ├── MainWindow.h
    └── PayloadControlTab/  # Payload control interface
        ├── PayloadSettingsTab.cpp  # Settings and controls
        └── PayloadSettingsTab.h
```

### Key Classes

- **MainWindow**: Main application window managing the overall UI layout and connection status
- **PayloadSettingsTab**: Comprehensive control panel for all payload settings and video display
- **PayloadSdkInterface**: SDK wrapper handling communication with payload devices

## Building

### Prerequisites

Install the required dependencies:

```bash
sudo apt-get install libgtkmm-3.0-dev
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev
sudo apt-get install libopencv-dev
```

**Note**: OpenCV is required by the PayloadSDK examples. If you encounter cmake errors about missing OpenCV, install it:
```bash
sudo apt-get install libopencv-dev python3-opencv
```

### Build Instructions

**Important**: The UI Demo must be built as part of the main PayloadSDK project, not standalone.

1. Navigate to the PayloadSDK root directory:
```bash
cd PayloadSdk
```

2. Create build directory and configure with VIO or ORUSL enabled:
```bash
mkdir build && cd build
cmake -DVIO=1 ..
# or for ORUSL: cmake -DORUSL=1 ..
```

3. Build the project:
```bash
make -j6
```

4. Run the UI Demo application:
```bash
./tests/ui_demo/ui_demo
```

## Usage

### Connecting to Payload

1. Launch the application
2. Enter your payload's IP address in the IP field (default: 192.168.55.1)
   - **Important**: Change this to match your actual payload IP address
   - **Auto RTSP URL**: When you enter an IP address, the RTSP URL field will automatically update to `rtsp://[IP]:8554/payload`
   - You can manually edit the RTSP URL field if you need a different port or path
3. Click the **Connect** button
4. Wait for connection status to show "Connected" in green
5. All control panels will become active upon successful connection

### Disconnecting

Click the **Disconnect** button to safely close the connection to the payload device.

### Controlling the Camera

- Use dropdown menus to select camera modes and settings
- Adjust sliders for zoom speed, focus speed, and gimbal control
- Click buttons for immediate actions (capture, record, auto focus, etc.)
- Settings are applied in real-time to the connected payload

### Video Streaming

1. After connection, the streaming URL will be automatically populated
2. The RTSP URL is automatically generated from the IP address (format: `rtsp://[IP]:8554/payload`)
3. You can manually modify the RTSP URL if needed (different port, path, etc.)
4. Click **Play** to start the video stream
5. Click **Stop** to stop the stream
6. Click on the video area to set tracking target position (when tracking is enabled)

### Monitoring Status

All status information updates automatically in the information panel:
- Storage status and capacity
- Capture and recording status
- Gimbal attitude (pitch, roll, yaw)
- Zoom levels (EO/IR)
- Temperature readings
- GPS coordinates
- LRF measurements

## Configuration

### Connection Settings

Default connection configuration in `main.h`:
```cpp
T_ConnInfo s_conn = {
    CONTROL_UDP,
    udp_ip_target,
    udp_port_target
};
```

### RTSP URL Format

The RTSP URL is automatically generated from the IP address using the format:
```
rtsp://[IP_ADDRESS]:8554/payload
```

You can manually modify this in the RTSP URL field if your payload uses a different configuration.

### Parameter Update Rates

The application configures various parameter update rates in `initPayloadSDKInterface()`:
- Zoom levels: 1000 ms
- LRF data: 100 ms
- GPS coordinates: 1000 ms
- Camera settings: 1000 ms
- Tracking position: 100 ms

## Callback System

The application uses a callback-based architecture for event handling:

- **onUICommandChanged**: Handles UI control events and sends commands to payload
- **onUIConnectCommandChanged**: Manages connection/disconnection events
- **onPayloadStatusChanged**: Receives payload status updates
- **onPayloadParamChanged**: Receives parameter value changes
- **onPayloadStreamChanged**: Receives streaming information updates

## Troubleshooting

### Build Errors

**Error: payloadSdkInterface.h not found**
- Ensure you're building from the PayloadSDK root directory, not from `tests/ui_demo`
- Make sure you've specified `-DVIO=1` or `-DORUSL=1` in the cmake command

**Error: OpenCV not found**
- Install OpenCV development libraries:
  ```bash
  sudo apt-get install libopencv-dev python3-opencv
  ```
- If still failing, verify OpenCV installation:
  ```bash
  pkg-config --modversion opencv4
  # or
  pkg-config --modversion opencv
  ```

### Video Stream Not Working

If video streaming doesn't work:
1. Ensure "Auto Connection" is enabled in the payload web interface
2. Check network connectivity between PC and payload
3. Verify firewall settings allow RTSP traffic
4. Ensure GStreamer plugins are properly installed
5. Verify the RTSP URL format is correct (default: `rtsp://[IP]:8554/payload`)
6. Try manually editing the RTSP URL if your payload uses a different port or path

### Connection Failed

If connection fails:
1. Verify the IP address is correct and matches your payload's IP
2. Check network cable connection
3. Ensure payload is powered on
4. Verify payload is running compatible firmware (v2.0.0 or higher)

### UI Not Responsive

If controls are disabled:
1. Ensure you are connected to the payload
2. Check connection status indicator
3. Try disconnecting and reconnecting

## Dependencies

- **GTK+ 3.0 (gtkmm)**: GUI framework
- **GStreamer 1.0**: Video streaming and playback
- **OpenCV**: Computer vision library (required by PayloadSDK examples)
- **PayloadSDK**: Gremsy Payload SDK library
- **pthread**: Threading support

## Supported Payloads

- VIO Payload (software v2.0.0 or higher)
- ORUSL Payload (software v2.0.0 or higher)
- Other Gremsy payloads with compatible firmware

## Notes

- The application requires an active network connection to the payload
- Some features may not be available depending on the connected payload type
- Real-time updates depend on the configured parameter rates
- Video streaming quality depends on network bandwidth and payload settings
- The RTSP URL is automatically generated from the IP address but can be manually edited if needed
- **Important**: Always update the IP address field to match your actual payload IP address
