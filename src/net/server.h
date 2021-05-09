/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SERVER_H_
#define SRC_NET_SERVER_H_

#include <map>
#include <atomic>

#include "net/socket.h"
#include "net/epoll.h"
#include "net/session.h"

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
  bool CreateNewSession();

  ServerSocket socket_;
  Epoll epoll_;
  ServerOption option_;

  using SessionMap = std::map<int32_t, Session*>;
  SessionMap session_map_;

  std::atomic<bool> running_;
};

}  // namespace rdv

#endif  // SRC_NET_SERVER_H_
