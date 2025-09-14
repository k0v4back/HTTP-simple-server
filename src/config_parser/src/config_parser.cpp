#include <iostream>
#include <fstream>
#include <regex>

#include "../include/config_parser.h"

namespace config
{
    void config_parser::parse_config(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file with configs");
        }

        std::string line;
        std::regex worker_proc_regex(R"(worker_processes\s+(\d+);)");
        std::regex worker_conn_regex(R"(worker_connections\s+(\d+);)");
        std::regex listen_regex(R"(listen\s+([\w.]+);)");
        std::regex server_name_regex(R"(server_name\s+([\w.]+);)");
        std::regex root_regex(R"(root\s+([^;]+);)");

        std::smatch match;
        bool in_http_block = false;
        bool in_server_block = false;
        bool in_location_block = false;

        while (std::getline(file, line))
        {
            // Remove leading/trailing whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;

            // Check for block openings
            if (line == "http {")
            {
                in_http_block = true;
                continue;
            }

            if (line == "server {")
            {
                in_server_block = true;
                continue;
            }

            if (line.find("location / {") != std::string::npos)
            {
                in_location_block = true;
                continue;
            }

            // Check for block closings
            if (line == "}")
            {
                if (in_location_block)
                {
                    in_location_block = false;
                }
                else if (in_server_block)
                {
                    in_server_block = false;
                }
                else if (in_http_block)
                {
                    in_http_block = false;
                }
                continue;
            }

            // Parse values based on context
            if (!in_http_block)
            {
                if (std::regex_search(line, match, worker_proc_regex))
                {
                    worker_processes = std::stoi(match[1]);
                }
                else if (std::regex_search(line, match, worker_conn_regex))
                {
                    worker_connections = std::stoi(match[1]);
                }
            }
            else if (in_server_block && !in_location_block)
            {
                if (std::regex_search(line, match, listen_regex))
                {
                    listen_port = match[1];
                }
                else if (std::regex_search(line, match, server_name_regex))
                {
                    server_name = match[1];
                }
            }
            else if (in_location_block)
            {
                if (std::regex_search(line, match, root_regex))
                {
                    root = match[1];
                }
            }
        }

        file.close();
    }
}