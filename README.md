# 💬 C++ Multi-Client Chat Server

A real-time multi-client chat application built using **C++ sockets**, designed to work reliably over LAN with proper handling of TCP stream behavior.

---

## 🚀 Features

* 🔹 Multi-client support (thread per client)
* 🔹 Real-time message broadcasting
* 🔹 Username-based messaging
* 🔹 Works across devices (LAN: phone ↔ laptop)
* 🔹 Proper TCP stream handling (no message corruption)
* 🔹 Thread-safe client management using mutex

---

## 🧠 Key Learning

This project focuses on understanding how **TCP actually works** beyond localhost testing.

### Important concepts implemented:

* Handling **partial reads** from `recv()`
* Using **delimiter-based protocol (****`\n`****)** for message boundaries
* Avoiding reliance on null-terminated strings
* Buffer accumulation and parsing
* Synchronizing shared resources using `mutex`
* Debugging real-world networking issues (LAN vs localhost)

---

## ⚠️ Problem Faced

Initially, the chat system worked perfectly on `localhost`, but failed over LAN.

### Issues encountered:

* Messages getting split or merged
* Corrupted output (`ined the chat!`)
* Connection refused errors
* Inconsistent behavior across devices

### Root cause:

> TCP is a **stream protocol**, not message-based.

---

## ✅ Solution

Implemented a proper message handling strategy:

* Accumulate incoming data into a buffer
* Extract complete messages using `\n`
* Process only complete messages
* Preserve leftover data for next read

This ensures correct behavior even when:

* Messages arrive in chunks
* Multiple messages arrive together

---

## 🏗️ Project Structure

```
.
├── server.cpp   # Multi-client threaded server
├── client.cpp   # Chat client with send/receive threads
```

---

## 🛠️ How to Run

### 1. Compile

```bash
g++ server.cpp -o server -pthread
g++ client.cpp -o client -pthread
```

---

### 2. Run Server

```bash
./server
```

---

### 3. Run Client

```bash
./client
```

---

## 🌐 LAN Usage (Phone ↔ Laptop)

1. Connect both devices to the same WiFi
2. Find your laptop IP:

```bash
ipconfig   # Windows
ifconfig   # Linux
```

3. Update client code:

```cpp
inet_pton(AF_INET, "YOUR_IP_HERE", &serverAddr.sin_addr);
```

---

## 🔐 Future Improvements

* [ ] Private messaging (`@username`)
* [ ] Chat rooms / channels
* [ ] End-to-end encryption
* [ ] Message persistence
* [ ] Non-blocking sockets (epoll/select)

---

## 📌 Tech Stack

* C++
* POSIX Sockets
* Multithreading (`std::thread`)
* Mutex synchronization

---

## 💡 What This Project Shows

* Understanding of low-level networking
* Ability to debug real-world system issues
* Strong grasp of TCP behavior
* Clean multi-threaded design

---

## 🤝 Contributions

Open to improvements and suggestions!

---

## 📜 License

This project is licensed under the GNU General Public License v3.0 (GPLv3).
