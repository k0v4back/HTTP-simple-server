#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

namespace config
{
    class config_parser
    {
    public:
        config_parser() = default;
        ~config_parser() = default;

    public:
        void parse_config(const std::string& filename);

        uint32_t get_worker_processes() const
        {
            return worker_processes;
        }

        uint32_t get_worker_connections() const
        {
            return worker_connections;
        }

        std::string get_listen_port() const
        {
            return listen_port;
        }

        std::string get_server_name() const
        {
            return server_name;
        }

        std::string get_root() const
        {
            return root;
        }

    private:
        uint32_t worker_processes;
        uint32_t worker_connections;
        std::string listen_port;
        std::string server_name;
        std::string root;
    };
}