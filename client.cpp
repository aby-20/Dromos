#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<thread>
using namespace std;

void receiveMessages(int sock){
    char buffer[1024];
    string data;
    while(true){
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if(bytes <= 0){
            cout<<"Disconnected from server"<<endl;
            break;
        }
        data.append(buffer, bytes);

        size_t pos;
        while((pos = data.find('\n')) != string::npos){
            string msg = data.substr(0, pos);
            if(!msg.empty()){
                cout<<msg<<endl;
            }
            data.erase(0, pos + 1);
        }
        
    }
}
void sentMessages(int sock){
    while(true){
        string message;
        getline(cin,message);
        message += "\n";
        send(sock, message.c_str(), message.size(), 0);
    }
}

int main(){
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    if(connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        perror("Connection failed");
        return -1;
    }
    cout<<"Connected to server"<<endl;

    string username;
    cout<<"Enter your username: ";
    getline(cin, username);
    username+="\n";
    send(clientSocket, username.c_str(), username.size(), 0);

    thread receiveThread(receiveMessages, clientSocket);
    thread sendThread(sentMessages, clientSocket);
    receiveThread.join();
    sendThread.detach();
    close(clientSocket);

    return 0;

}




