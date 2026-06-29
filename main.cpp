#include <format>
#include <memory>
#include <iostream>

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>


int main(int argc, char const *argv[])
{
    const int s = socket(AF_INET,SOCK_DGRAM,0);

    if (s < 0) {
        std::cerr << std::format("[ERROR] Unable to create connection socket. Errno {} : '{}'\n", errno, strerror(errno));
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = 8333;

    char buf [1];
    sockaddr_in other;
    long otherLen = sizeof(other);

    if(bind(s,(sockaddr*)&addr,sizeof(addr))) {
        std::cerr << std::format("[ERROR] Unable to bin to port 8333. Errno {} : '{}'\n", errno, strerror(errno));
        goto clean;
    }


    recvfrom(s, buf,sizeof(buf),0,(sockaddr*)&other,(socklen_t*)&otherLen);

    std::cout << std::format("Got data from {}:{}", inet_ntoa(other.sin_addr), ntohs(other.sin_port));


clean:
    const int r = close(s);

    if (r) {
        std::cerr << std::format("[WARNING] Error when closing connection. ERRNO {} : '{}'\n",errno, strerror(errno));
    }

    return r ? EXIT_FAILURE : EXIT_SUCCESS;
}
