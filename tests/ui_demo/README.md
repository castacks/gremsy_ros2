# 🎨 UI Demo Application

## Your Complete Control Center for Gremsy Payloads

**A professional-grade graphical interface** that brings the full power of Gremsy's advanced payload systems to your fingertips. Built for evaluation, development, and demonstration, this application showcases every capability of the PayloadSDK in an intuitive, production-ready interface.

---

## 🌟 What Makes This Special?

The UI Demo isn't just a testing tool—it's a **complete reference implementation** that demonstrates:

✨ **Best Practices** - See how to properly integrate PayloadSDK features
🎯 **Real-world Usage** - Production-ready code you can learn from
🚀 **Rapid Evaluation** - Test every payload feature without writing code
💼 **Client Demonstrations** - Professional interface for showcasing capabilities
📚 **Interactive Learning** - Hands-on way to understand the SDK

### Technology Stack

Built with industry-standard technologies for reliability and performance:
- **GTK+ 3.0 (gtkmm)** - Native Linux GUI framework
- **GStreamer 1.0** - Professional video streaming pipeline
- **C++** - High-performance, low-latency control
- **PayloadSDK** - Direct integration with Gremsy hardware

---

## 🎯 Complete Feature Set

### 📹 Camera Control Suite

<details>
<summary><b>View Modes & Recording</b> - Multi-sensor visualization</summary>

- **6 View Modes**: EO/IR, EO only, IR only, IR/EO, SYNC, Side-by-Side
- **Smart Recording**: Choose recording source (EO, IR, Both, or OSD overlay)
- **One-Click Capture**: Instant image capture with status feedback
- **Recording Timer**: Real-time recording duration display
</details>

<details>
<summary><b>Zoom Control System</b> - Professional precision zoom</summary>

- **Continuous Zoom**: Smooth zoom in/out with adjustable speed (0-7 levels)
- **Step Zoom**: Precise incremental zoom control
- **Range Zoom**: Direct zoom level slider (0-100%)
- **Speed Control**: Dedicated zoom speed adjustment
</details>

<details>
<summary><b>Focus Control</b> (VIO/ORUSL only) - Sharp image capture</summary>

- **Auto Focus**: One-click automatic focus
- **Continuous Focus**: Manual focus in/out control
- **Speed Adjustment**: Configurable focus speed (0-7 levels)
- **Real-time Feedback**: Instant focus position updates

> **Note**: Focus controls are not available for MB1 payload
</details>

<details>
<summary><b>Exposure Control</b> - Professional image quality</summary>

- **5 Exposure Modes**: Auto, Manual, Shutter Priority, Iris Priority, Gain Priority
- **Shutter Speed**: 28 settings from 1/1 to 1/10000 second
- **Aperture/Iris**: 15 settings from F2.0 to F11
- **Gain Control**: 13 levels from 0 dB to 36 dB
- **Real-time Preview**: See changes instantly in video stream
</details>

<details>
<summary><b>White Balance</b> - Natural color reproduction</summary>

- **Auto Mode**: Intelligent automatic white balance
- **Scene Presets**: Indoor, Outdoor, ATW (Auto Tracking White)
- **One Push WB**: Instant white balance calibration
- **Manual Mode**: Fine-tune color temperature
</details>

<details>
<summary><b>IR Thermal Imaging</b> - Advanced thermal control</summary>

- **10 Color Palettes**: Choose the perfect visualization
- **FFC Modes**: Manual or automatic Flat Field Correction
- **Manual FFC Trigger**: On-demand calibration
- **Temperature Display**: Real-time Min/Max/Mean temperature
- **IR Gain Modes**: Low (-50~150°C) or High (-50~550°C) range
- **AGC Modes**: Normal, Hold, Threshold, Bright, Linear, Manual
</details>

### 🎮 Gimbal Control System

<details>
<summary><b>Stabilization Modes</b> - Professional camera stabilization</summary>

- **OFF**: Free movement, no stabilization
- **LOCK**: Hold absolute position regardless of platform movement
- **FOLLOW**: Smooth follow with platform movements
- **MAPPING**: Specialized mode for surveying and mapping
- **HOME**: Quick return to center position
</details>

<details>
<summary><b>Speed Control</b> - Responsive movement control</summary>

- **Adjustable Speed**: 1-180 degrees per second
- **Interactive Control Pad**:
  - Up/Down for tilt
  - Left/Right for pan
  - Visual speed indicator
- **Continuous Operation**: Hold buttons for smooth movement
</details>

<details>
<summary><b>Angle Positioning</b> - Precision targeting</summary>

- **Pitch Control**: -90° to +90° (full vertical range)
- **Roll Control**: -45° to +45° (stabilization range)
- **Yaw Control**: -180° to +180° (full rotation)
- **Live Feedback**: Real-time attitude display
- **Smooth Sliders**: Drag to desired angle
</details>

### 🎬 Video Streaming & Display

<details>
<summary><b>RTSP Stream Player</b> - Professional video playback</summary>

- **Built-in Player**: Native GStreamer integration
- **Low Latency**: Optimized for real-time applications
- **Auto URL Generation**: Just enter IP, RTSP URL auto-configures
- **Manual Override**: Edit RTSP URL for custom configurations
- **Fullscreen Mode**: Immersive video viewing
- **16:9 Aspect Ratio**: Automatic aspect ratio maintenance
</details>

<details>
<summary><b>Interactive Tracking</b> - Point-and-track interface</summary>

- **Touch Mode**: Click anywhere on video to set tracking point
- **Visual Feedback**: See tracking status in real-time
- **Tracking/Detection**: Switch between tracking and detection modes
- **Target Coordinates**: GPS coordinates of tracked target
</details>

### 📊 Real-time Status Monitoring

<details>
<summary><b>Telemetry Dashboard</b> - Complete situational awareness</summary>

**System Status:**
- Storage capacity (total/used/available)
- Image capture count
- Recording status and duration
- Current view mode and recording source

**Gimbal Status:**
- Current mode (OFF/LOCK/FOLLOW/MAPPING)
- Real-time attitude (Pitch/Roll/Yaw)
- Position feedback

**Camera Status:**
- EO zoom level (current magnification)
- IR zoom level (thermal magnification)
- IR camera type (G1/F1 or none)
- Active IR palette
- FFC mode status

**Environmental Data:**
- IR temperature readings (Min/Max/Mean °C)
- Laser rangefinder distance (meters)
- LRF offset position (X/Y)

**GPS & Navigation:**
- Payload GPS (Longitude/Latitude/Altitude)
- Target GPS coordinates (when tracking)
- Real-time coordinate updates
</details>

### 🔧 MB1 Advanced Features

<details>
<summary><b>MB1-Specific Controls</b> - Enhanced MB1 payload features</summary>

**General Settings:**
- Setting Target: EO Camera / IR Camera / Gimbal Device
- RC Mode: Gremsy / Standard
- Storage Type: Internal / SD Card
- Object Detection: Enable/Disable AI detection
- Isotherms Gain: High/Low gain modes
- Gimbal FW Flag: Overwrite/Forward

**EO Advanced:**
- Scene Modes: 16 presets (Action, Portrait, Landscape, Night, etc.)
- AE Compensation: -12 to +12 range
- White Balance: 11 modes including Manual RGB
- ISO: Auto, Deblur, 100-3200
- Sharpness: 0-6 levels

**IR Advanced:**
- Gain Mode: Low (-50~150°C) / High (-50~550°C)
- Contrast Mode: Default / Custom
- AGC Mode: 6 modes (Normal, Hold, Threshold, etc.)
- AGC Linear Percent: 0-100% adjustment

**IR SpotMeter:**
- Mode: Enable/Disable
- Units: Celsius / Fahrenheit / Kelvin
- Size: 16-128 pixels

**IR Isotherm:**
- Mode: Enable/Disable color highlighting
- Units: Kelvin / Celsius / Fahrenheit / Percent / Raw Counts
- Threshold: 0-150 adjustable range
</details>

---

## 🚀 Quick Start Guide

---

## 🏗️ Architecture & Design

### Clean, Maintainable Code Structure

```
ui_demo/
├── main.cpp                          # 🚀 Application entry & SDK integration
│   ├── PayloadSDK initialization
│   ├── Callback handlers (status, params, streaming)
│   └── Event dispatching
│
├── main.h                            # ⚙️ Configuration & definitions
│   ├── Connection parameters
│   ├── Parameter indices
│   └── System constants
│
├── CMakeLists.txt                    # 🔨 Build configuration
│   └── Dependency management
│
└── ui_gtk/                           # 🎨 GTK+ User Interface
    ├── MainWindow.cpp/.h             # 📱 Main application window
    │   ├── Connection management
    │   ├── Status bar
    │   └── Tab management
    │
    └── PayloadControlTab/            # 🎛️ Control interface
        ├── PayloadSettingsTab.cpp/.h # Complete control panel
        │   ├── Camera controls
        │   ├── Gimbal controls
        │   ├── Video streaming
        │   ├── Status monitoring
        │   └── MB1 advanced features
```

### Design Patterns & Best Practices

#### 🎯 **Callback-Driven Architecture**
- **Event-based communication** between UI and SDK
- **Non-blocking operations** for responsive interface
- **Thread-safe updates** using GLib idle callbacks

#### 🔌 **Modular Component Design**
- **Separation of concerns**: UI, logic, and SDK layers clearly separated
- **Reusable widgets**: Generic combo boxes, buttons, and controls
- **Easy maintenance**: Each feature in its own function

#### ⚡ **Performance Optimizations**
- **Efficient update throttling**: Prevent UI overload
- **Smart signal management**: Disconnect inactive callbacks
- **Resource cleanup**: Proper memory and thread management

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
