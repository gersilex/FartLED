FartLED Documentation
=====================

Usage
-----

- Use a FartLED client and connect to the TCP port 41 of the FartLED device to send data.

See the [protocol specification](PROTOCOL.md) for implementation details of the FartLED API.

Boot-Up Debugging
-----------------

Connect Pin 2 to Ground (GND/0V) to enable Boot-Up Debugging. The Pin is checked once immediately after powering on or resetting the device. The connection from the debugging pin to Ground may be removed at any time but the device has to be reset to completely disable debugging.

When powering up with Boot-Up Debugging enabled, FartLED starts verbose serial logging at 115200 baud (115200,8,N,1) and runs tests of all its components. Components with a feedback channel (e.g. the network adapter) report their state back to FartLED which in turn visualizes the status of the component as a colored row on the panel. Components without feedback channel (e.g. LEDs) produce output themselves and expect to be tested by the operator in the *Boot-Up* phase.

FartLED continues with normal operation after testing the components. Remember to reboot FartLED without debugging for maximum performance, because logging to serial takes a lot of time and will reduce the frame rate if left enabled.

### Component Row Mapping

Each component is represented as a full row. The first row (row 0) represents the first component and is initialized first. All components intialize after each other and in ascending row order (from top to bottom).

| Row | Components                                         |
| --- | -------------------------------------------------- |
| 0   | LEDs (Displays test patterns) (no feedback)        |
| 1   | Network Card (Link, DHCP, Hardware)                |
| 2   | Remote Controller (Waits for connection at tcp/41) |

### Color Mapping

Each component status row is represented in one of the following colors.

| Color   | Meaning                                                                   |
| ------- | ------------------------------------------------------------------------- |
| DimGray | Component test running, waiting for data                                  |
| Blue    | Network card test: DHCP registration failed, using hard-coded fallback IP |
| Red     | Test failed                                                               |
| Green   | Test succeeded or no feedback channel                                     |

Benchmark
---------

Using the [Python example script in the contrib folder](..\contrib\clients\python\benchmark.py) in a local Fast Ethernet network (100 Mbps) the following framerates were reached:

- Cell scale of 1, full panel => 384 bytes: 50 FPS
- Cell scale of 2, full panel => 192 bytes: 82 FPS

Benchmarking results were read over Serial, which takes time, so frame rates are higher when the serial connection / Boot-Up Debugging is disabled.
