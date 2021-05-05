/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef SRC_BASE_TIME_H_
#define SRC_BASE_TIME_H_

#include <chrono>

namespace rdv {

class Time {
 public:
  static uint64_t timestamp() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }

 private:
};

}  // namespace rdv

#endif  // SRC_BASE_TIME_H_
