/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "net/session.h"

namespace rdv {

Session* Session::Create(Socket* sock) {
  return new Session(sock);
}

Session::Session(Socket* sock) : sock_(sock), connect_(true) {}

Session::~Session() {}

void Session::Send() {}

void Session::Read() {
  char data[4096];
  int32_t len = read(sock_->GetSocket(), &data, sizeof(data));
  if (!len) {
    std::cout << data << std::endl;
  } else {
    connect_.store(false);
  }
}

void Session::Close() {

}

}  // namespace rdv
