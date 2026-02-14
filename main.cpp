#include<iostream>
#include <cstring>
#include <vector>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <arpa/inet.h>
#include <mutex> 



std::mutex mtx;



void checkPort(std::string ip, int port,std::vector<std::string> &vectorConnections)
{
        int ourSocket = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in serverStruct{};
        serverStruct.sin_family = AF_INET;
        serverStruct.sin_port = htons(port);
        serverStruct.sin_addr.s_addr = inet_addr(ip.c_str());

        int connection = connect(ourSocket,(struct sockaddr*)&serverStruct,sizeof(serverStruct));

        if (connection < 0)
        {
            //para que no se escriban entre hilos arriba de vectorConnections
            std::lock_guard<std::mutex> lock(mtx);
            vectorConnections.push_back(std::to_string(port)+ ":"+"NO");
        }
        else
        {
            std::lock_guard<std::mutex> lock(mtx);
            vectorConnections.push_back(std::to_string(port)+ ":"+"SI");
        }
        close(ourSocket);
}



int main()
{
    std::vector<std::string> vecConnecions{};
    std::string ipToConnect{};
    int puertoInicial{};
    int puertoFinal{};

    std::cout<<"Ingrese la ip a mapear: ";
    std::cin>>ipToConnect;
    std::cout<<'\n';
    std::cout<<"Ingrese desde que puerto a que puerto quiere escanear (ej 20 100): ";
    std::cin>>puertoInicial>>puertoFinal;

    if (puertoInicial>puertoFinal)
    {
        std::cerr<<"El puerto inicial debe ser menor que el final.";
        return 1;
    }
    if (std::abs(puertoFinal-puertoInicial)>=1200)
    {
        std::cerr<<"La maxima cantidad de puertos a escanear es 1200\n"<<"(ej 1 1000)";
        return 1;
    }

    std::vector<std::thread> hilos{};

    for (int i{puertoInicial};i<=puertoFinal;i++)
    {
        hilos.emplace_back(checkPort, ipToConnect,i,std::ref(vecConnecions));
    }

    for (auto &hilo : hilos)
    {
        hilo.join();
    }

    for (auto const con : vecConnecions)
    {
        std::cout<<con<<'\n';
    }

    return 0;
}



