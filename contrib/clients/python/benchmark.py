import socket
import time

sock = socket.create_connection(("192.168.1.236", 41))
print("Connected to %s:%d" % sock.getpeername())

while True:
  for i in [8*8]:
    tosend = b'\x02' + i.to_bytes(1, 'little') + (b'\x01\x00\x00' * i)
    sock.send(tosend)
    tosend = b'\x02' + i.to_bytes(1, 'little') + (b'\x00\x00\x00' * i)
    sock.send(tosend)
