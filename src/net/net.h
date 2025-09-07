#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <list>

namespace net
{
    enum connType
    {
        STREAM = 0,
        MESSAGE
    };

    typedef int32_t fd_t;
}