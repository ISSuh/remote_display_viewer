/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SESSION_H_
#define SRC_NET_SESSION_H_

#include "net/socket.h"

namespace rdv {

class Session {
 public:
  static Session Create(Socket sock) {
    return Session(sock);
  }

  void Send() {}
  void Read() {}

 private:
  explicit Session(Socket sock) : sock_(sock) {}
  ~Session() {}

  Socket sock_;
};

}  // namespace rdv

#endif  // SRC_NET_SESSION_H_
