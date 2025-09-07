#pragma once

#include <cstdint>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <list>
#include <sys/types.h>

#include "../net.h"

namespace net
{
    /*
     * TCP server connection info
     */
    class tcp_server_info
    {
        public:
            tcp_server_info() = default;
            ~tcp_server_info() = default;

        public:
            void set_local_ip(std::string& ip)
            {
                local_ip = ip;
            }

            std::string get_local_ip() const
            {
                return local_ip;
            }

            void set_local_port(std::string& port)
            {
                local_port = port;
            }

            std::string get_local_port() const
            {
                return local_port;
            }

        private:
            std::string local_ip;
            std::string local_port;
    };

    /*
     * TCP socket API
     */
    class tcp_sock_api
    {
        public:
            tcp_sock_api(tcp_server_info& local_info) :
                local_ip(local_info.get_local_ip()),
                local_port(local_info.get_local_port()),
                conn_type{} {}

        public:
            fd_t create_socket();
            int32_t bind_socket(fd_t fd);
            void start_listen(fd_t fd);
            void close_socket(fd_t fd);

        private:
            std::string local_ip;
            std::string local_port;

            std::string client_ip;
            std::string client_port;

            connType    conn_type;
    };


    /*
     * TCP server functions
     */
    class tcp_server : public tcp_sock_api
    {
        public:
            tcp_server(tcp_server_info& server_info) :
                tcp_sock_api(server_info),
                server_sock_fd{-1} {}

            ~tcp_server()
            {
                close_socket(server_sock_fd);
            }

            void on_connect(fd_t fd);
            void on_disconnect(fd_t fd);
            void on_socket_create(fd_t fd);
            int32_t on_receive(fd_t fd, char* buf, uint32_t buf_size);
            void send_msg(fd_t client_fd, char* buf, uint32_t buf_size);

        private:
            fd_t server_sock_fd;
    };
}