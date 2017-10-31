#ifndef _LOG_H_
#define _LOG_H_

#include <string>


class Log {
  static const int level = 0;
  template <typename T>
  static void output(T t) {
    std::cout << t << std::endl;
  }
  template <typename T, typename... Args>
  static void output(T t, Args... args) {
    std::cout << t;
    output(args...);
  }
public:
  template <typename T>
  static void error(T t)
  {
    if(level >= 1) {
      std::cout << "[ERROR] " << t << std::endl;
    }
  }
  template<typename T, typename... Args>
  static void error(T t, Args... args) // recursive variadic function
  {
    if(level >= 1) {
      std::cout << "[ERROR] " << t;
      output(args...);
    }
  }

  template <typename T>
  static void warn(T t)
  {
    if(level >= 2) {
      std::cout << "[WARN] " << t << std::endl;
    }
  }
  template<typename T, typename... Args>
  static void warn(T t, Args... args) // recursive variadic function
  {
    if(level >= 2) {
      std::cout << "[WARN] " << t;
      output(args...);
    }
  }

  template <typename T>
  static void info(T t)
  {
    if(level >= 3) {
      std::cout << "[INFO] " << t << std::endl;
    }
  }
  template<typename T, typename... Args>
  static void info(T t, Args... args) // recursive variadic function
  {
    if(level >= 3) {
      std::cout << "[INFO] " << t;
      output(args...);
    }
  }
};

#endif
