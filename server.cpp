#include <cstring>
#include <vector>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

#define BACKLOG 5

// ========SERVER======== //

void manejarCliente(int clientCocket,int idClient)
{
    std::vector<char> buffer(1024);
    while (1)
    {
        size_t bytesRecibidos = recv(clientCocket,buffer.data(),sizeof(buffer), 0);
        if (bytesRecibidos<0 || bytesRecibidos == 0)
        {
            break;
        }
        std::string s{buffer.data(),bytesRecibidos};
        std::cout<<"cliente"<<idClient<<": ";
        std::cout<<s<<'\n';
    }
    return;
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverPort;
    serverPort.sin_port = htons(8080);
    serverPort.sin_addr.s_addr = INADDR_ANY;
    serverPort.sin_family = AF_INET;

    int binding = bind(serverSocket,(struct sockaddr*)&serverPort,sizeof(serverPort));
    if (binding < 0)
    {
        std::cerr<<"el bindeo no se concreto correctamente";
        return 1;
    }

    listen(serverSocket,BACKLOG);

    int id{1};

    while (true)
    {
        int clientSocket = accept(serverSocket,nullptr,nullptr);
        if (clientSocket < 0)
        {
            std::cerr<<"ERROR CLIENTE AL CONECTARSE"<<'\n';
            return 1;
        }
        std::thread t1(manejarCliente,clientSocket,id);
        t1.detach(); // separa al hilo de la ejecucion, asi se ejecuta aislado.
        id+=1;
    }

    close(serverSocket);
    return 0;
}
