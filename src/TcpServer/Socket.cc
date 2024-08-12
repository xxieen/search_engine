#include "../../include/TcpServer/Socket.h"

Socket::Socket() : _sockfd(0) {
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd < 0) {
    perror("scoket");
  }
}

Socket::~Socket() { close(_sockfd); }

Socket::Socket(int fd) : _sockfd(fd) {}

int Socket::fd() const { return _sockfd; }

void Socket::shutdownWrite() // 关闭写通道
{
  shutdown(_sockfd, SHUT_WR);
}
