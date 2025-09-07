#include <iostream>
#include <cstring>
#include <memory>

#include "http.h"
#include "http_parser/include/http_parser.h"
#include "../libs/Thread-pool/include/thread_pool.h"

#include "net/net.h"

template<typename T, typename U, typename V>
void print_hashmap(std::map<T, std::map<U, V>> const &ht)
{
    for (auto const &pair: ht) {
        std::cout << "{ Address: " << pair.first << " }\n";
    }
}

int main() {
    net::tcp_server_info tcp_server_info;
    std::string local_ip = "127.0.0.1";
    std::string local_port = "7373";
    tcp_server_info.set_local_ip(local_ip);
    tcp_server_info.set_local_port(local_port);

    std::unique_ptr<HTTP> server(new HTTP(tcp_server_info));

    tp::ThreadPoll threadPool {std::thread::hardware_concurrency()};

    server->handleHttp("/", HTTPMethod::GET, "index.html");
    server->handleHttp("/about", HTTPMethod::GET, "about.html");
    server->handleHttp("/favicon.ico", HTTPMethod::GET, "../icons/favicon.ico");
    server->handleHttp("/", HTTPMethod::POST, "");

#ifdef DEBUG
    print_hashmap(server->getHT());
    std::cout << "Host address = " << local_ip << " Host port = " << local_port << std::endl;
#endif

    try {
        server->listenHttp(threadPool);
    } catch (std::exception &ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return -1;
    }

    threadPool.WaitAll();
    threadPool.Stop();

    return 0;
}