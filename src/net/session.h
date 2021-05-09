/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_SESSION_H_
#define SRC_NET_SESSION_H_

#include <iostream>
#include <atomic>

#include "net/socket.h"

namespace rdv {

class Session {
 public:
  static Session* Create(Socket* sock);
  ~Session();

  void Send();
  void Read();
  void Close();

  bool IsConnect() const { return connect_.load(); }

 private:
  explicit Session(Socket* sock);

  Socket* sock_;
  std::atomic<bool> connect_;
};

}  // namespace rdv

#endif  // SRC_NET_SESSION_H_
