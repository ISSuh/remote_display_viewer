/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_EPOLL_H_
#define SRC_NET_EPOLL_H_

#include <sys/epoll.h>
#include <sys/unistd.h>

#include "net/socket.h"

namespace rdv {

class Epoll {
 public:
  const int DEFAULT_EVENTS_SIZE = 1000;

  Epoll() : epoll_fd_(0) {}
  ~Epoll() {
    close(epoll_fd_);
  }

  bool Create() {
    epoll_fd_ = epoll_create(DEFAULT_EVENTS_SIZE);
    if (epoll_fd_ < 0) {
      close(epoll_fd_);
      return false;
    }
    return true;
  }

  int* GetHandle() { return &epoll_fd_; }
  bool SetEpollEvnet(Socket sock) {
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
};

}  // namespace rdv

#endif  // SRC_NET_EPOLL_H_
