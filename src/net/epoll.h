/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_EPOLL_H_
#define SRC_NET_EPOLL_H_

#include <sys/epoll.h>
#include <vector>

#include "net/socket.h"

namespace rdv {

class Epoll {
 public:
  using Event = epoll_event;
  const int DEFAULT_EVENTS_SIZE = 1024;

  Epoll();
  ~Epoll();

  bool Create(Socket* sock);
  void Close(Socket* sock);
  std::vector<Event> Wait();

  int32_t GetHandle() const { return epoll_fd_; }

 private:
  bool SetEpollEvnet(Socket* sock);

  int32_t epoll_fd_;
  std::vector<Event> events_;

  int32_t timeout_;
};

}  // namespace rdv

#endif  // SRC_NET_EPOLL_H_
