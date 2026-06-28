# sockets-in-c 🔌

Progressive exploration of raw socket programming in C — building from first principles toward a full userspace TCP/IP stack.

Each project in this repo goes one level deeper into how the internet actually works under the hood, with zero abstractions and zero libraries doing the heavy lifting.

---

## Projects

### 01 — TCP Echo Server & Client
A bare-bones TCP server and client built using raw POSIX sockets.

**What it does:** Client connects to server over TCP, sends a message, server echoes it back.

**Why it matters:** Implements the full TCP connection lifecycle manually —
socket creation, binding, listening, accepting, sending, and receiving —
without any networking library. Every byte is handled explicitly.

**Concepts:** BSD socket API, TCP 3-way handshake, network byte order, file descriptors in Linux.

**How to run:**
gcc server.c -o server
gcc client.c -o client
./server # Terminal 1
./client # Terminal 2

---

### 02 — rawhttp: HTTP/1.1 Client over Raw TCP
An HTTP/1.1 client built entirely from scratch — no curl, no libcurl, no abstractions.

**What it does:** Takes a hostname, resolves it via DNS, opens a raw TCP connection to port 80, hand-crafts an HTTP GET request byte by byte, reads the response, and parses it into status line, headers, and body.

**Why it matters:** This is exactly what your browser does under the hood on every page load — except here every single step is explicit and visible. No magic.

**Concepts:** DNS resolution with `gethostbyname`, HTTP/1.1 request structure, `\r\n\r\n` header/body splitting, chunked transfer encoding, raw TCP I/O.

**How to run:**
gcc main.c -o rawhttp
./rawhttp example.com
./rawhttp neverssl.com

**Sample output:**
Resolved example.com → 172.66.147.243
Connected to example.com:80
--- SENDING REQUEST ---
GET / HTTP/1.1
Host: example.com
Connection: close
=== STATUS ===

HTTP/1.1 200 Ok
=== HEADERS ===
Content-Type: text/html
Transfer-Encoding: chunked
Server: cloudflare
...
=== BODY ===
<!doctype html>...
=== TOTAL BYTES RECEIVED: 865 ===

---

## Where this is going

These projects are the foundation toward building **NetStacc** — a full userspace TCP/IP stack from scratch in C, implementing:

- ARP
- IPv4
- ICMP
- UDP
- TCP
- HTTP 1.0

Using TUN/TAP virtual network interfaces to bypass the OS network stack entirely and handle raw packets at the ethernet frame level.

---

## Stack
- Language: C (C11)
- Environment: Linux (Ubuntu 24.04 via WSL2)
- Tools: GCC 13, POSIX socket API, Wireshark (for packet inspection)
