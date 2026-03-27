#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<cstring>
#include<string>
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
    char buffer[1024];
    string data;
    string username;
while(true){
   int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
   if(bytes<=0){
       close(clientSocket);
       return;
    }
    data.append(buffer, bytes);
    size_t pos = data.find('\n');
    if(pos != string::npos){
        username = data.substr(0, pos);
        data.erase(0, pos + 1);
        
        data.clear();
        break;
    }

       
}

    {
        lock_guard<mutex> lock(client_mutex);
        clients[clientSocket] = username;
    }

  string joinMsg = username + " has joined the chat!\n";
  cout<<joinMsg;
    broadcast(joinMsg, clientSocket);
     size_t pos2;
        while((pos2=data.find('\n')) != string::npos){
            string msg = data.substr(0, pos2);
            string fullMsg = username + ": " + msg + "\n";
            cout<<fullMsg;
            broadcast(fullMsg, clientSocket);
            data.erase(0, pos2 + 1);
        }
    
    
while(true){
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    if(bytes <= 0){
        cout<<username<<"disconnected"<<endl;
        {
            lock_guard<mutex> lock(client_mutex);
            clients.erase(clientSocket);
        }
        string leaveMsg = username + " has left the chat!\n";
        broadcast(leaveMsg, clientSocket);
        close(clientSocket);
        break;
    
    }
    data.append(buffer, bytes);
    size_t pos;
    while((pos = data.find('\n')) != string::npos){
        
        string msg = data.substr(0, pos);
        if(msg.empty()){
            data.erase(0, pos + 1);
            continue;

        } 
        string fullMsg = username + ": " + msg + "\n";  
        cout<<fullMsg;
        broadcast(fullMsg, clientSocket);
        data.erase(0, pos + 1);
    }

}
}
int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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








