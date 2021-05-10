/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SESSION_H_
#define SRC_NET_SESSION_H_

#include <iostream>
#include <string>
#include <atomic>

#include "net/socket.h"

namespace rdv {

enum class SessionState : uint8_t {
  OK,
  CONNECT,
  DISCONNECT,
  READ_ERROR,
  SEND_ERROR,
  LAST = SEND_ERROR
};

class Session {
 public:
  static const uint32_t DEFAULT_READ_SIZE = 4096;

  static SessionState Send(Socket* socket, const std::string& message);
  static SessionState Read(Socket* socket);
  static void Close(Socket* socket);
};

}  // namespace rdv

#endif  // SRC_NET_SESSION_H_
