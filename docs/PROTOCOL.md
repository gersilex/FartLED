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

- If the sender (client) presents an incompatible protocol version, the connection should be closed by the receiver (server).

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

- These 3 bytes are to be repeated *n* times (see *Byte 2*).
- Any remaining data is to be discarded by the receiver (server).
- If the sender (client) sends a message shorter than described in the header (*byte 1 bit 4-7*) the receiver should display the data anyways, leaving the missing pixels unchanged.
- If the sender stops sending data within the message part, the receiver should time out and display the incomplete data, while leaving the missing pixels unchanged.

Control Data
------------

- Underlying internet protocol: **TCP** (TCP/IP)
- Port number: **23** ("unix telnet")

TBD
