/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */
#include <sys/unistd.h>
#include <utility>

#include "net/epoll.h"

namespace rdv {

Epoll::Epoll()
    : epoll_fd_(0),
      events_(std::vector<Event>(DEFAULT_EVENTS_SIZE)),
      timeout_(-1) {
}

Epoll::~Epoll() {
  close(epoll_fd_);
}

bool Epoll::Create(Socket* sock) {
  if (!epoll_fd_) {
    epoll_fd_ = epoll_create(DEFAULT_EVENTS_SIZE);
    if (epoll_fd_ < 0) {
      close(epoll_fd_);
      return false;
    }
  }
  return SetEpollEvnet(sock);
}

std::vector<Epoll::Event> Epoll::Wait() {
  int32_t count =
      epoll_wait(epoll_fd_, &events_[0], DEFAULT_EVENTS_SIZE, timeout_);

  if (count < 1) {
    return {};
  }
  return std::vector<Epoll::Event>(&events_[0], &events_[0] + count);
}

bool Epoll::SetEpollEvnet(Socket* sock) {
  Event event;
  event.events = EPOLLIN;
  event.data.fd = sock->GetSocket();

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock->GetSocket(), &event) < 0) {
    return false;
  }
  return true;
}

}  // namespace rdv
