# UDP Server-Client System
This project builds a UDP server-client system. The system communicats through the given IP and prot. The server simply echo the message given by client. 

Note:
This project is developed in Linux system, and would not work under windows.
 
## Usage
- Server:
```
server.exe <IP> <Port>
```
- Client:
```
client.exe <IP> <Port> <Message> <Max_try(optional)>
```
## [Demo] To run the system:
1. Build the execution for server and client by:
```
gcc Server.c -o server
gcc Client.c -o client
```
2. Start server:
```
./server 127.0.0.1 4455
```
4. Start another terminal different from server and execute client:
```
./client 127.0.0.1 4455 Hello
```
