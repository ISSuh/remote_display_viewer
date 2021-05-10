/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SERVER_H_
#define SRC_NET_SERVER_H_

#include <atomic>
#include <map>

#include "net/epoll.h"
#include "net/session.h"
#include "net/socket.h"

namespace rdv {

class ServerOption {
 public:
  SocketOption server_socket_option_;
};

class Server {
 public:
  explicit Server(ServerOption option);
  ~Server();

  bool Initial();
  void Run();

 private:
  bool IsListenEvent(int32_t socket_fd) {
    return socket_.GetSocketFd() == socket_fd;
  }
  bool CreateNewSession();
  void HandleRequest(int32_t socket_fd);

  ServerSocket socket_;
  Epoll epoll_;
  ServerOption option_;

  using SocketMap = std::map<int32_t, Socket*>;
  SocketMap client_socket_map_;

  std::atomic<bool> running_;
};

}  // namespace rdv

#endif  // SRC_NET_SERVER_H_
