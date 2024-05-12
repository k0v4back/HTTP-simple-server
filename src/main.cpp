#include <iostream>
#include <cstring>

#include "http.h"
#include "../libs/Thread-pool/src/thread_pool.h"

template<typename T, typename U>
void print_hashmap(std::unordered_map<T, U> const &ht)
{
    for (auto const &pair: ht) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

int main() {
    HTTP* server = new HTTP("127.0.0.1", "7373");
    tp::ThreadPoll threadPool {std::thread::hardware_concurrency()};

    server->handleHttp("/", "index.html");
    server->handleHttp("/about", "about.html");
    server->handleHttp("/favicon.ico", "../icons/favicon.ico");

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