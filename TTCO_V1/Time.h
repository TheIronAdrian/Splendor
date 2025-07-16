#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <chrono>

inline double getTime() {
  static auto start = std::chrono::high_resolution_clock::now();
  auto now = std::chrono::high_resolution_clock::now();
  double time_taken =
      std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count();

  time_taken *= 1e-9;
  return time_taken;
}

#endif // TIME_H_INCLUDED
