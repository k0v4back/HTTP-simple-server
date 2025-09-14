#include <iostream>
#include <cstring>
#include <memory>

#include "src/http.h"
#include "src/http_parser/include/http_parser.h"
#include "src/config_parser/include/config_parser.h"
#include "src/net/net.h"

#include "libs/Thread-pool/include/thread_pool.h"

template<typename T, typename U, typename V>
void print_hashmap(std::map<T, std::map<U, V>> const &ht)
{
    for (auto const &pair: ht) {
        std::cout << "{ Address: " << pair.first << " }\n";
    }
}

int main() {
    config::config_parser config;
    config.parse_config("conf/server.conf");

    net::tcp_server_info tcp_server_info;
    tcp_server_info.set_local_ip(config.get_server_name());
    tcp_server_info.set_local_port(config.get_listen_port());

    std::cout << "config.get_root() = " << config.get_root() << std::endl;

    std::unique_ptr<HTTP> server(new HTTP(tcp_server_info));

    tp::ThreadPoll threadPool {std::thread::hardware_concurrency()};

    // server->handleHttp("/", HTTPMethod::GET, "index.html");
    // server->handleHttp("/about", HTTPMethod::GET, "about.html");
    // server->handleHttp("/favicon.ico", HTTPMethod::GET, "../icons/favicon.ico");
    // server->handleHttp("/", HTTPMethod::POST, "");

    server->handleHttp("/", HTTPMethod::GET, config.get_root());

#ifdef DEBUG
    print_hashmap(server->getHT());
    std::cout << "Host address = " << config.get_server_name() << " Host port = " << config.get_listen_port() << std::endl;
#endif

    try {
        server->listenHttp(threadPool, config);
    } catch (std::exception &ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return -1;
    }

    threadPool.WaitAll();
    threadPool.Stop();

    return 0;
}