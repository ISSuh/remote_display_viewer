/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <vector>

#include "net/server.h"

namespace rdv {

Server::Server(ServerOption option)
    : socket_(ServerSocket(option.server_socket_option_)),
      option_(option),
      running_(false) {}

Server::~Server() {}

bool Server::Initial() {
  if (!epoll_.Create(socket_.GetSocket())) {
    return false;
  }

  if (!socket_.Initial(33669)) {
    return false;
  }

  return true;
}

void Server::Run() {
  running_.store(true);

  while (running_.load()) {
    std::vector<Epoll::Event> events = epoll_.Wait();
    if (events.empty()) {
      continue;
    }

    for (const auto& event : events) {
      int32_t fd = event.data.fd;
      std::cout << session_map_.size() << std::endl;
      if (fd == socket_.GetSocketFd()) {
        CreateNewSession();
      } else {
        Session* session = session_map_[fd];
        if (session->IsConnect()) {
          session->Read();
        } else {
          session->Close();
          delete session;
          session_map_.erase(fd);
        }
      }
    }
  }
}

bool Server::CreateNewSession() {
  Socket* client_socket = nullptr;
  SocketInfo client_socket_info;
  SocketOption client_socket_option;

  client_socket_option.non_blocking = true;
  client_socket_option.is_client = true;
  client_socket_option.is_server = false;

  client_socket = socket_.Accept(&client_socket_info);
  if (!client_socket) {
    return false;
  }

  client_socket->SetOption(client_socket_option);

  if (!epoll_.Create(client_socket)) {
    delete client_socket;
    return false;
  }

  if (session_map_.find(client_socket->GetSocket()) != session_map_.end()) {
    delete client_socket;
    return false;
  }

  session_map_.insert(
      {client_socket->GetSocket(), Session::Create(client_socket)});
  return true;
}

}  // namespace rdv
