/// util.h
///
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include <cstdlib>
  #include <cmath>
  #include <stdio.h>
  #include <cstring>
  #include <utility>
  #include <string>
  #include <chrono>
  #include <boost/date_time.hpp>
  #include <type_traits>
  #include <boost/thread.hpp>

#pragma once

#ifndef NDEBUG
  #define ASSERT(condition, message) \
    do { \
      if (! (condition)) { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                  << " line " << __LINE__ << ": " << message << std::endl; \
        std::terminate(); \
      } \
    } while (false)
#else
  #define ASSERT(condition, message) do { } while (false)
#endif

namespace rad::log {

  namespace pt = boost::posix_time;

  long double operator "" _K(long double value);
  long double operator "" _M(long double value);
  long double operator "" _G(long double value);

  enum class Scheduling_Policy {
    Round_Robin         = SCHED_RR,
    First_In_First_Out  = SCHED_FIFO
  };

  // represents lower and upper bounds
  template<typename T>
  struct Range
  {
    T lo_;
    T hi_;
    Range(T lo, T hi) 
     : lo_(lo), hi_(hi) {};
  }; // Range

  // represents a slice of an array
  template<typename T>
  struct Slice
  {
    T* data_;
    std::size_t n_;
    std::size_t lo_;
    std::size_t hi_;
    T* begin() { return data_ + lo_; }
    T* end()   { return data_ + hi_; }
    Slice(T *data, std::size_t lo, std::size_t hi) 
     : data_(data), n_(hi-lo), lo_(lo), hi_(hi) {};
    Slice(T *data, const Range<std::size_t> &range)
     : data_(data), n_(range.hi_-range.lo_), lo_(range.lo_), hi_(range.hi_) {};
  }; // Slice

  // number of bytes to human readable string
  template<typename T>
  char * const bytes_human(char buffer[], T size)
  {
    constexpr int N_units = 7;
    constexpr auto factor = 1000;
    const char *units[N_units];
    units[0] = "B";
    units[1] = "KB";
    units[2] = "MB";
    units[3] = "GB";
    units[4] = "TB";
    units[5] = "PB";
    units[6] = "EB";
    int k = 0;
    double count = size;
    while (count >= factor && k < N_units)
    {
      ++k;
      count /= factor;
    }
    //std::memset(buffer, '\0', sizeof(buffer));
    if (count - std::floor(count) == 0.0)
    {
      sprintf(buffer, "%ld %s", static_cast<std::size_t>(count), units[k]);
    }
    else
    {
      sprintf(buffer, "%.1f %s", count, units[k]);
    }
    return buffer;
  }

  // numeric value to human readable string with units
  template<typename T>
  std::string format_human(T value, std::string suffix="")
  {
    std::stringstream ss;
    constexpr auto factor = 1000;
    constexpr int N_units = 7;
    const char *units[N_units];
    units[0] = "";
    units[1] = "K";
    units[2] = "M";
    units[3] = "G";
    units[4] = "T";
    units[5] = "P";
    units[6] = "E";
    int k = 0;
    double count = value;
    while (count >= factor && k < N_units)
    {
      ++k;
      count /= factor;
    }
    if (count - std::floor(count) == 0.0)
    {
      ss << static_cast<std::size_t>(count) << " " << units[k] << suffix;
    }
    else
    {
      ss << count << " " << units[k] << suffix;
    }
    return ss.str();
  }

  std::pair<const std::string, const std::string> split_pair(std::string str, char delimiter);

  static constexpr const char * const get_time_format();
  std::time_t to_time_t(const std::string &str);
  std::tm to_tm(const std::string &str);
  std::string to_string(const std::tm &tm);
  std::string to_string(const std::time_t &time);
  std::string today();
  bool is_active_between(std::time_t time_start, std::time_t time_stop);
  bool is_expired(std::time_t time_stop);

  std::time_t absolute_time(const std::string &timestr, const std::string &datestr="");
  std::time_t absolute_time(const std::string &timestr, std::time_t timeref, const std::string &datestr="");
  std::string to_human_interval(std::time_t time);

  template<typename T>
  bool has_time_expired(T time, std::chrono::high_resolution_clock::time_point reference);

  std::string ltrim(std::string value);

  // human readable string to numeric type
  template<typename T>
  T parse_human(std::string value);

  // human readable suffix to numeric representation
  long double parse_human(std::string value);

  // convert any enum class to underlying type
  template<typename E>
  constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type 
  {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

  int set_name(boost::thread &thread, std::string name);
  int set_priority(boost::thread &thread, int priority, Scheduling_Policy policy=Scheduling_Policy::Round_Robin);

} // namespace

/// *EOF*
