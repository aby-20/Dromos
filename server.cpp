#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<cstring>
#include<mutex>
#include<map>
#include<thread>

using namespace std;

map<int,string>clients;
mutex client_mutex;

void broadcast(string message,int sender_socket){
    lock_guard<mutex> lock(client_mutex);
    for(auto& client : clients){
        if(client.first != sender_socket){
            send(client.first, message.c_str(), message.size(), 0);
        }
    }
}
void handle_client(int clientSocket){
    char buffer[1024] = {0};
   int bytes = recv(clientSocket, buffer, 1024, 0);
   if(bytes<=0){
       close(clientSocket);
       return;
    }
    string username(buffer)
    {
        lock_guard<mutex> lock(client_mutex);
        clients[clientSocket] = username;
    }
    string joinMsg = username + " has joined the chat!";
    broadcast(joinMsg, clientSocket);
while(true){
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(bytes <= 0){
        cout<<username<<"disconnected"<<endl;
        {
            lock_guard<mutex> lock(client_mutex);
            clients.erase(clientSocket);
        }
        string leaveMsg = username + " has left the chat!";
        broadcast(leaveMsg, clientSocket);
        close(clientSocket);
        break;
    }
    string message = username + ": " + string(buffer);
    broadcast(message, clientSocket);

}
int main(){
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    cout<<"Server is running on port 8080..."<<endl;
while(true){
    int clientSocket = accept(serverSocket,nullptr,nullptr);
    thread t(handle_client, clientSocket);
    t.detach();
}
close(serverSocket);
return 0;

}





}


