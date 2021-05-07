/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SOCKET_H_
#define SRC_NET_SOCKET_H_

#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <string>

namespace rdv {

class SocketAddr {
 public:
  SocketAddr() {}
  ~SocketAddr() {}

 private:
  sockaddr_in sock_addr_;
};

class Socket {
 public:
  Socket() : sock_fd_(0) {}
  explicit Socket(int sock) : sock_fd_(sock) {}
  virtual ~Socket() {
    close(sock_fd_);
  }

  int* GetSocket() { return &sock_fd_; }

 private:
  bool SetNonBlocking() {}
  bool SetSocketOption() {}
  bool Bind() {}
  bool Listen() {}

  int sock_fd_;
};

class Epoll {
 public:
  const int DEFAULT_EVENTS_SIZE = 1000;

  Epoll() : epoll_fd_(0) {}
  ~Epoll() {
    close(epoll_fd_);
  }

  bool Initialize() {
    epoll_fd_ = epoll_create(DEFAULT_EVENTS_SIZE);
    if (epoll_fd_ < 0) {
      close(epoll_fd_);
      return false;
    }
    return true;
  }

  int* GetHandle() { return &epoll_fd_; }
  bool AddEpoll(Socket sock) {
    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = *sock.GetSocket();

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, *sock.GetSocket(), &event) < 0) {
      return false;
    }

    return true;
  }

 private:
  int epoll_fd_;
  struct epoll_event evnets_;
}



}  // namespace rdv

#endif  // SRC_NET_SOCKET_H_
