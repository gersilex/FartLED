Protocol Reference
==================

LED Stream Data
---------------

- Underlying internet protocol: **TCP** (TCP/IP)
- Port number: **41** ("unix video")

### Data Structure

#### Header

##### Byte 1: Protocol and scale information

```text
0123 4567
0000 .... : Protocol version (0-15)
.... 0000 : Number of LEDs per cell (0-15)
```

- If the sender (client) presents an incompatible protocol version, the connection is closed by the receiver (server).

##### Byte 2: Message length

```text
0123 4567
0000 0000 : Number of cells in the upcoming message (24 bits each) (0-255)
```

#### Message

##### Bytes 3..n (n presented in header byte 1 bit 4-7)

```text
0123 4567   0123 4567   0123 4567
RRRR RRRR   GGGG GGGG   BBBB BBBB : Bitmap-style pixel data for one cell.
```

- These 3 bytes must be repeated for the number of cells announced in the *header's Byte 2*.
- Any remaining data is discarded by the receiver (server).
- The receiver waits up to 1000 milliseconds for data until the complete message is received.
- If the sender (client) sends a message shorter than described in the header (*byte 1 bit 4-7*) the receiver waits for the timeout and then displays the data anyways, leaving the missing pixels unchanged.

Control Data
------------

- Underlying internet protocol: **TCP** (TCP/IP)
- Port number: **23** ("unix telnet")

RFC
