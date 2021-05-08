/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SOCKET_H_
#define SRC_NET_SOCKET_H_

#include <sys/socket.h>
#include <sys/unistd.h>

#include <string>
#include <utility>

namespace rdv {

using SocketAddrIn = sockaddr_in;
using SocketAddr = sockaddr;
using SocketLen = socklen_t;

class SocketOption {
 public:
  bool non_blocking = true;
  bool is_server = true;
  bool is_client = false;
  int32_t reuse_addr = true;
  int32_t listen_back_log = 15;
};

class SocketInfo {
 public:
  SocketInfo() : sock_len_(sizeof(sock_addr_in_)) {}
  ~SocketInfo() {}

  SocketAddrIn* GetSocketAddrIn() { return &sock_addr_in_; }
  SocketLen* GetSocketLen() { return &sock_len_; }

 private:
  SocketAddrIn sock_addr_in_;
  SocketLen sock_len_;
};

class Socket {
 public:
  virtual ~Socket();

  static Socket* Create(int32_t socket_fd = 0);

  int32_t GetSocket() { return sock_fd_; }
  bool SetOption(SocketOption option);

 private:
  explicit Socket(int32_t sock);

  bool SetNonBlocking();
  bool SetSocketOption(int32_t options);

  int32_t sock_fd_;
};

class ServerSocket {
 public:
  explicit ServerSocket(SocketOption option);
  ~ServerSocket();

  bool Initial(uint32_t port);
  int32_t GetSocket() { return socket_->GetSocket();}

  Socket* Accept(SocketInfo* info);

 private:
  bool Bind(uint32_t port);
  bool Listen(uint32_t listen_back_log);

  Socket* socket_;
  SocketInfo socket_info_;
};

}  // namespace rdv

#endif  // SRC_NET_SOCKET_H_
