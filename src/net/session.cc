/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include <thread>
#include <chrono>

#include "net/session.h"

namespace rdv {

SessionState Session::Send(Socket* socket, const std::string& message) {
  std::cout << "Session::Send - " << socket->GetSocket() << std::endl;
  write(socket->GetSocket(), message.c_str(), message.length());
  return SessionState::OK;
}

SessionState Session::Read(Socket* socket) {
  std::cout << "Session::Read - " << socket->GetSocket() << std::endl;

  char data[DEFAULT_READ_SIZE];
  int32_t len = read(socket->GetSocket(), &data, sizeof(data));

  if (len == 0) {
    return SessionState::DISCONNECT;
  } else if (len < 0) {
    return SessionState::READ_ERROR;
  }

  std::cout << socket->GetSocket() << " - \n" << data << std::endl;
  return SessionState::OK;
}

void Session::Close(Socket* socket) {
  close(socket->GetSocket());
  delete socket;
}

}  // namespace rdv
