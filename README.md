# 💬 C++ Chat Application (Boost.Asio + Socket Programming)

A real-time **client-server chat application** built in **C++** using **Boost.Asio** and **TCP socket programming**.  
This project demonstrates real-time communication between multiple clients through a server — running locally, across a LAN (Wi-Fi/Ethernet), or even over the Internet using port forwarding.  

---

## ✨ Features
- Real-time messaging between multiple clients  
- Client-Server architecture using TCP sockets  
- Works on WSL Ubuntu / Linux / macOS / Windows  
- Supports:
  - Localhost (same PC with split terminal)  
  - Multiple PCs on the same LAN  
  - Over the Internet using port forwarding 🌍  
- Multithreaded server to handle multiple clients  
- Graceful handling of client disconnections  

---

## 🛠️ Tech Stack
- **C++ (C++11/14)**  
- **Boost.Asio** (asynchronous networking)  
- **TCP Sockets**  
- **Multithreading (std::thread)**  

---

## 📂 Project Structure
chat_app/

├── server.cpp # Server code

├── client.cpp # Client code

└── README.md # Documentation

---

## ⚡ Setup Instructions

### 1️⃣ Install Dependencies
#### On Ubuntu / WSL:
```bash
sudo apt update
sudo apt install g++ libboost-all-dev -y
```

### 2️⃣ Compile
g++ server.cpp -o server -lboost_system -lpthread
g++ client.cpp -o client -lboost_system -lpthread

### 3️⃣ Run

Start the server:
```bash
./server
```

Run clients (in other terminals or computers):
```bash
./client
```
# 🌐 Networking Options

## Localhost (same PC):
Use 127.0.0.1 in client.cpp.

## LAN (Wi-Fi/Ethernet):

### Find server’s local IP:

- **ip addr show**   # Linux
- **ipconfig**     # Windows


### Replace in client.cpp:

socket.connect(tcp::endpoint(
    boost::asio::ip::address::from_string("192.168.x.x"), 1234));


### Over the Internet (Port Forwarding):

- **Enable port forwarding (TCP 1234) on your router**

- **Get your public IP from whatismyip.com**

- **Replace with public IP in client.cpp**

# 🎯 Future Improvements

- **Add usernames for clients**

- **Implement private chats (/msg command)**

- **Add message logging on the server**

- **GUI-based client (Qt / SFML)**


## 🔗 GitHub Repository
https://github.com/kartiiikkk/ChatterBox---Chat-Application


## 🤝 Contributing

Pull requests and feature suggestions are welcome!
