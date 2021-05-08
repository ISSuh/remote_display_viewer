/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <arpa/inet.h>
#include <fcntl.h>

#include "net/socket.h"

#include <utility>

namespace rdv {

Socket* Socket::Create(int32_t socket_fd) {
  if (!socket_fd) {
    int32_t sock = 0;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      return nullptr;
    }
    return new Socket(sock);
  }

  return new Socket(socket_fd);
}

Socket::Socket(int32_t sock) : sock_fd_(sock) {}

Socket::~Socket() {
  close(sock_fd_);
}

bool Socket::SetOption(SocketOption option) {
  if (option.non_blocking) {
    if (SetNonBlocking()) {
      return false;
    }
  }

  if (option.is_server) {
    if (SetSocketOption(option.reuse_addr)) {
      return false;
    }
  }
  return true;
}

bool Socket::SetNonBlocking() {
  int flag = fcntl(sock_fd_, F_GETFL, 0);
  if (fcntl(sock_fd_, F_SETFL, flag) < 0) {
    return false;
  }
  return true;
}

bool Socket::SetSocketOption(int32_t options) {
  if (setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &options,
                 sizeof(options)) < 0) {
    return false;
  }
  return true;
}

ServerSocket::ServerSocket(SocketOption option) : socket_(Socket::Create()) {}

ServerSocket::~ServerSocket() {}

bool ServerSocket::Initial(uint32_t port) {
  if (!socket_->GetSocket()) {
    return false;
  }

  SocketOption option;
  if (socket_->SetOption(option)) {
    return false;
  }

  if (Bind(port)) {
    return false;
  }

  if (Listen(option.listen_back_log)) {
    return false;
  }

  return true;
}

bool ServerSocket::Bind(uint32_t port) {
  SocketAddrIn* sock_addr_in = socket_info_.GetSocketAddrIn();

  sock_addr_in->sin_family = AF_INET;
  sock_addr_in->sin_port = htons(port);
  sock_addr_in->sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_->GetSocket(),
           reinterpret_cast<SocketAddr*>(sock_addr_in),
           sizeof(*sock_addr_in)) < 0) {
    return false;
  }

  return true;
}

bool ServerSocket::Listen(uint32_t listen_back_log) {
  if (listen(socket_->GetSocket(), listen_back_log) < 0) {
    return false;
  }
  return true;
}

Socket* ServerSocket::Accept(SocketInfo* info) {
  int32_t client_socket_fd = accept(socket_->GetSocket(),
                                reinterpret_cast<SocketAddr*>(info->GetSocketAddrIn()),
                                info->GetSocketLen());
  if (client_socket_fd < 0) {
    return nullptr;
  }
  return Socket::Create(client_socket_fd);
}

}  // namespace rdv
