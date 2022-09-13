#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
namespace udp
{
    class UdpServer
    {
    private:
        int port;
        char IP_ADDR[20];
        SOCKET sock;
        sockaddr_in addr;
    public:
        UdpServer(const char* IP_ADDR, int port);
        ~UdpServer();
        void run();
        void close();
    };
    UdpServer::UdpServer(const char* IP_ADDR, int port)
    {
        WSADATA wsadata;
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        this->port = port;
        strcpy_s(this->IP_ADDR, sizeof(IP_ADDR), IP_ADDR);
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        // addr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);
        inet_pton(AF_INET, IP_ADDR, &addr.sin_addr);
        int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR)
        {
            std::cout << "bind error" << std::endl;
            return;
        }
        return;
    }

    UdpServer::~UdpServer()
    {
        close();
    }

    void UdpServer::run()
    {
        char buf[1024];
        while (true)
        {
            struct sockaddr_in client_addr;
            int len = sizeof(client_addr);
            // int ret = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
            int ret = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)&client_addr, &len);
            if (ret == SOCKET_ERROR)
            {
                std::cout << "recv error" << std::endl;
                return;
            }
            // std::cout << inet_ntoa(client_addr.sin_addr) << std::endl;
            // std::cout << ntohs(client_addr.sin_port) << std::endl;
            std::cout << buf << std::endl;
        }
    }

    void UdpServer::close()
    {
        closesocket(sock);
    }


    class UdpClient
    {
    private:
        SOCKET client_sock;
        sockaddr_in addr;
    public:
        UdpClient();
        UdpClient(const char* IP_dst, const int port_dst);
        ~UdpClient();
        void send(const char* buf);
        void send(const char* IP_dst, int port_dst, const char* buf);
        void close();
    };

    UdpClient::UdpClient()
    {
        WSADATA wsadata;
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        return;
    }

    UdpClient::UdpClient(const char* IP_dst, const int port_dst)
    {
        WSADATA wsadata;
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        client_sock = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_dst);
        // addr.sin_addr.S_un.S_addr = inet_addr(IP_dst);
        inet_pton(AF_INET, IP_dst, &addr.sin_addr);
        return;
    }
    UdpClient::~UdpClient()
    {
        close();
    }

    void UdpClient::send(const char* IP_dst, int port_dst, const char* buf)
    {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_dst);
        // addr.sin_addr.S_un.S_addr = inet_addr(IP_dst);
        inet_pton(AF_INET, IP_dst, &addr.sin_addr);
        int ret = sendto(client_sock, buf, (int)strlen(buf) + 1, 0, (sockaddr*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR)
        {
            std::cout << "send error" << std::endl;
            return;
        }
        return;
    }

    void UdpClient::send(const char *buf)
    {
        int ret = sendto(client_sock, buf, (int)strlen(buf), 0, (sockaddr*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR)
        {
            std::cout << "send error" << std::endl;
            return;
        }
        return;
    }

    void UdpClient::close()
    {
        closesocket(client_sock);
        WSACleanup();
    }

} // namespace udp
