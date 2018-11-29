FartLED Documentation
=====================

Boot-Up
-------

When powering up, FartLED tests all its components. Components with a feedback channel (e.g. the network adapter) will report their state back to FartLED which in turn visualizes the status of the component as a colored row on the panel. Components without feedback channel (e.g. LEDs) will produce output themselves and expect to be tested by the operator in the *Boot-Up* phase.

### Component Row Mapping

Each component is represented as a full row. The first row (row 0) represents the first component and is initialized first. All components intialize after each other and in ascending row order (from top to bottom).

| Row | Components
| --- | ---
| 0 | LEDs (Displays test patterns) (no feedback)
| 1 | Network Card (Link, DHCP, Hardware)
| 2 | Remote Controller (Waits for connection at tcp/41)

### Color Mapping

Each component status row is represented in one of the following colors.

| Color | Meaning
| ----- | -------
| DarkGrey | Component test running, waiting for data
| Blue | Network card test: DHCP registration failed, using hard-coded fallback IP
| Red | Test failed
| Green | Test succeeded or no feedback channel

Usage
-----

- Use a FartLED client and connect to the TCP port 41 of FartLED to send data.

See the [protocol specification](PROTOCOL.md) for implementation details of the FartLED API.

Benchmark
---------

Using the [Python example script in the contrib folder](..\contrib\clients\python\benchmark.py) in a local Fast Ethernet network (100 Mbps) the following framerates were reached:

- Cell scale of 1, full panel => 384 bytes: 50 FPS
- Cell scale of 2, full panel => 192 bytes: 82 FPS

Benchmarking happens over Serial, which takes time, so frame rates are higher when the serial connection is disabled.
