#include <iostream>
#include <cstring>

#include "http.h"
#include "http_parser/http_parser.h"
#include "../libs/Thread-pool/include/thread_pool.h"

template<typename T, typename U, typename V>
void print_hashmap(std::map<T, std::map<U, V>> const &ht)
{
    // for (auto const &pair: ht) {
    //     std::cout << "{ Address = " << pair.first << "; Path to page = " << pair.second
    //     << "; HTTP method = " << "}\n";
    // }
}

int main() {
    HTTP* server = new HTTP("127.0.0.1", "7373");
    tp::ThreadPoll threadPool {std::thread::hardware_concurrency()};

    server->handleHttp("/", HTTPMethod::GET, "index.html");
    server->handleHttp("/about", HTTPMethod::GET, "about.html");
    server->handleHttp("/favicon.ico", HTTPMethod::GET, "../icons/favicon.ico");
    server->handleHttp("/", HTTPMethod::POST, "");

    /* For debug */
    print_hashmap(server->getHT());
    std::cout << "hostAddr=" << server->getHostAddr() << std::endl;

    try {
        server->listenHttp(threadPool);
    } catch (std::exception &ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return -1;
    }

    threadPool.WaitAll();

    return 0;
}