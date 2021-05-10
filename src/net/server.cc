/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <vector>

#include "net/server.h"
#include "net/http/request.h"

namespace rdv {

Server::Server(ServerOption option)
    : socket_(ServerSocket(option.server_socket_option_)),
      option_(option),
      running_(false) {}

Server::~Server() {}

bool Server::Initial() {
  std::cout << "Server::Initial\n";

  if (!epoll_.Create(socket_.GetSocket())) {
    return false;
  }

  if (!socket_.Initial(33669)) {
    return false;
  }

  return true;
}

void Server::Run() {
  std::cout << "Server::Run\n";

  running_.store(true);

  while (running_.load()) {
    std::vector<Epoll::Event> events = epoll_.Wait();
    if (events.empty()) {
      continue;
    }

    for (const auto& event : events) {
      int32_t socket_fd = event.data.fd;
      if (IsListenEvent(socket_fd)) {
        CreateNewSession();
      } else {
        HandleRequest(socket_fd);
      }
    }
  }
}

bool Server::CreateNewSession() {
  std::cout << "Server::CreateNewSession\n";

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

  if (client_socket_map_.find(client_socket->GetSocket()) !=
      client_socket_map_.end()) {
    delete client_socket;
    return false;
  }

  client_socket_map_.insert(
      {client_socket->GetSocket(), client_socket});
  return true;
}

void Server::HandleRequest(int32_t socket_fd) {
  switch (Session::Read(client_socket_map_[socket_fd])) {
    case SessionState::DISCONNECT:
      std::cout << socket_fd << "- SessionState::DISCONNECT\n";
      epoll_.Close(client_socket_map_[socket_fd]);
      Session::Close(client_socket_map_[socket_fd]);
      client_socket_map_.erase(socket_fd);
      break;
    case SessionState::READ_ERROR:
      std::cout << socket_fd << "- SessionState::READ_ERROR\n";
      epoll_.Close(client_socket_map_[socket_fd]);
      Session::Close(client_socket_map_[socket_fd]);
      client_socket_map_.erase(socket_fd);
      break;
    default:
      break;
  }
}

}  // namespace rdv
