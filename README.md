# Tcp-echo-c
A TCP echo server and client built from scratch in C using raw POSIX sockets.

## What it does
Client sends a message → server receives it → server echoes it back.
Demonstrates the full TCP connection lifecycle: socket creation, binding,
listening, accepting, sending, and receiving.

## Concepts covered
- BSD socket API (socket, bind, listen, accept, connect, send, recv)
- TCP 3-way handshake
- Network byte order (htons, inet_pton)
- File descriptors in Linux

## Next steps
This is the foundation toward building a full userspace TCP/IP stack
(NetStacc) — implementing ARP, IPv4, ICMP, UDP, TCP, and HTTP 1.0
from scratch using TUN/TAP virtual interfaces.
