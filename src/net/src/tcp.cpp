#include "../include/tcp.h"

#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace net
{
    fd_t tcp_sock_api::create_socket()
    {
        fd_t fd;
        int flags = 0;

        if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
            throw std::runtime_error("Failed to create socket");
        }

        /* Set nonblocking for fd */
        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            close_socket(fd);
            throw std::runtime_error("Failed fcntl nonblock socket option");
        }

        return fd;
    }

    int32_t tcp_sock_api::bind_socket(fd_t fd)
    {
        struct sockaddr_in server_addr;

        /* Fill local address structure */
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(local_ip.c_str());
        server_addr.sin_port = htons(atoi(local_port.c_str()));

        /* Bind to the local address and port */
        if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            close_socket(fd);
            throw std::runtime_error("Failed to bind to socket");
        }

        return 1;
    }

    void tcp_sock_api::start_listen(fd_t fd)
    {
        /* Mark the socket so it will listen for incoming connections */
        if (listen(fd, SOMAXCONN) < 0)
        {
            close_socket(fd);
            throw std::runtime_error("Failed to listen on port");
        }
    }

    void tcp_sock_api::close_socket(fd_t fd)
    {
        close(fd);
    }



    void tcp_server::on_connect(fd_t fd) {}
    void tcp_server::on_disconnect(fd_t fd) {}

    void tcp_server::on_socket_create(fd_t fd)
    {
        int optval = 1;

        /* Check for the existence of such a socket in the system for reusing it with same port */
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
        {
            close_socket(fd);
            throw std::runtime_error("Failed to set socket options");
        }

    }

    int32_t tcp_server::on_receive(fd_t fd, char* buf, uint32_t buf_size)
    {
        int count = recv(fd, buf, buf_size, 0);
        return count;
    }

    void tcp_server::send_msg(fd_t client_fd, char* buf, uint32_t buf_size)
    {
        int32_t ret = send(client_fd, buf, buf_size, 0);
        if (ret <= 0)
        {
            close_socket(client_fd);
            // throw std::runtime_error("Failed to send data to client");
        }
    }
}