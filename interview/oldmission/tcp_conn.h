#ifndef TCP_CONN_H
#define TCP_CONN_H

#include "header.h"

class tcp_conn_t {
public:
    tcp_conn_t() {

    }
    ~tcp_conn_t(){
        disconnect();
    }

    bool connect(const std::string& ip_port) {
        auto pos = ip_port.find(':');
        if (pos == std::string::npos) return false;

        auto ip = ip_port.substr(0, pos);
        auto port = std::stoul(ip_port.substr(pos + 1));

        _fd_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_fd_sock < 0) {
            std::cout << "[ERROR] failed to create socket: "
                      << strerror(errno) << std::endl;
            return false;
        }

        sockaddr_in addr_server;
        unsigned int addr_size = sizeof(sockaddr_in);

        addr_server.sin_family = AF_INET;
        addr_server.sin_port = htons(port);
        addr_server.sin_addr.s_addr = inet_addr(ip.c_str());
        bzero(&addr_server.sin_zero, 8);

        auto iconnect = ::connect(_fd_sock, (sockaddr*)&addr_server, addr_size);
        if (iconnect < 0) {
            std::cout << "[ERROR] failed to connect: "
                      << strerror(errno) << std::endl;
            return false;
        }

        return true;
    }
    void disconnect() {
        if (_fd_sock > 0) {
            close(_fd_sock);
            _fd_sock = 0;
        }
    }

    size_t read(char* buf, size_t len) {
        // non-block
        auto ret = recv(_fd_sock, buf, len, MSG_DONTWAIT);
        if (ret < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // no data is available, normal
            } else {
                std::cout << "[WARNING] unexpected return value send(..): "
                          << strerror(errno) << std::endl;
            }
            return 0;
        }
        return ret;
    }

    size_t write(const char* data, size_t len) {
        auto ret = send(_fd_sock, data, len, 0);
        if (ret < 0) {
            std::cout << "[WARNING] unexpected return value send(..): "
                      << strerror(errno) << std::endl;
        }
        return ret;
    }

private:
    int _fd_sock = 0;
};

#endif
