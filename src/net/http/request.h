/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_NET_HTTP_REQUEST_H_
#define SRC_NET_HTTP_REQUEST_H_

#include <iostream>
#include <string>
#include <map>

#include "net/socket.h"
#include "net/http/util.h"

namespace rdv {
namespace Http {

class Request {
 public:
  using HeaderMap = std::map<std::string, std::string>;

  Request(Socket* socket, const std::string& message) : socket_(socket) {}
  ~Request() {
    socket_ = nullptr;
  }

  const Method GetMethod() const { return method_; }
  const Protocol GetProtocol() const { return protocol_; }
  const std::string& GetHostName() const { return host_name_; }
  const std::string& GetUserAgent() const { return user_agent_; }
  const std::string& GetURL() const { return url_; }
  const HeaderMap& GetAllHeaders() const { return headers_; }
  const std::string& GetHeader(const std::string& key) const {
    if (headers_.find(key) == headers_.end()) {
      return "";
    }
    return headers_.at(key);
  }

 private:
  Socket* socket_;

  Method method_;
  Protocol protocol_;
  std::string host_name_;
  std::string user_agent_;
  std::string url_;

  HeaderMap headers_;
  std::string body_;
};

}  // namespace Http
}  // namespace rdv

#endif  // SRC_NET_HTTP_REQUEST_H_
