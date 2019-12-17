import socket
import time

sock = socket.create_connection(("172.17.35.216", 41))
print("Connected to %s:%d" % sock.getpeername())

i = 64

while True:
    tosend = b'\x02' + i.to_bytes(1, 'little') + (b'\x01\x00\x00' * i)
    sock.send(tosend)
    tosend = b'\x02' + i.to_bytes(1, 'little') + (b'\x00\x00\x01' * i)
    sock.send(tosend)
