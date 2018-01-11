/// util.cc
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include "packages/common/util.h"
  #include <boost/algorithm/string/predicate.hpp>
  #include <stdexcept>
  #include <regex>
  #include <pthread.h>

namespace rad::log {

  long double operator "" _K(long double value)
  {
    return 1E3 * value;
  }

  long double operator "" _M(long double value)
  {
    return 1E6 * value;
  }

  long double operator "" _G(long double value)
  {
    return 1E9 * value;
  }

  std::pair<const std::string, const std::string> split_pair(std::string str, char delimiter)
  {
    const auto idx    = str.find_first_of(delimiter);
    const auto first  = str.substr(0, idx);
    const auto second = str.substr(idx + 1);
    return std::make_pair(first, second);
  }

  std::time_t to_time_t(const pt::ptime &pt)
  {
    pt::ptime timet_start(boost::gregorian::date(1970,1,1));
    pt::time_duration diff = pt - timet_start;
    return diff.ticks()/pt::time_duration::rep_type::ticks_per_second;
  }

  static constexpr const char * const get_time_format()
  {
    // GCC Bug 78714: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=78714
    return "%d %b %Y %H:%M:%S";
  }

  std::time_t to_time_t(const std::string &str)
  {
    auto format = get_time_format();
    std::tm tm = {};
    std::istringstream is(str);
    is >> std::get_time(&tm, format);
    return std::mktime(&tm);
  }

  std::tm to_tm(const std::string &str)
  {
    auto format = get_time_format();
    std::tm tm = {};
    std::istringstream is(str);
    is >> std::get_time(&tm, format);
    return tm;
  }

  std::string to_string(const std::tm &tm)
  {
    auto format = get_time_format();
    std::stringstream os;
    os << std::put_time(&tm, format);
    std::string timestr = os.str();
    return std::move(timestr);
  }

  std::string to_string(const std::time_t &time)
  {
    auto format = get_time_format();
    std::tm *tm = std::localtime(&time);
    std::stringstream os;
    os << std::put_time(tm, format);
    std::string timestr = os.str();
    return std::move(timestr);
  }

  std::string today()
  {
    auto format = "%d %b %Y";
    const auto now = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::high_resolution_clock::to_time_t(now);
    std::tm *tm = std::localtime(&time);
    std::stringstream os;
    os << std::put_time(tm, format);
    std::string timestr = os.str();
    return std::move(timestr);
  }

  std::string to_human_interval(std::time_t time)
  {
    using namespace std::chrono;
    const auto start = high_resolution_clock::from_time_t(time);
    const auto now = high_resolution_clock::now();
    auto d   = now - start;
    auto hhh = duration_cast<hours>(d);
    d -= hhh;
    auto mm  = duration_cast<minutes>(d);
    d -= mm;
    auto ss  = duration_cast<seconds>(d);
    d -= ss;
    auto ms  = duration_cast<milliseconds>(d);
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(3) << hhh.count() << ':' <<
              std::setfill('0') << std::setw(2) << mm.count()  << ':' << 
              std::setfill('0') << std::setw(2) << ss.count()  << '.' <<
              std::setfill('0') << std::setw(3) << ms.count();
    std::string result = stream.str();
    return result;
  }

  std::time_t absolute_time(const std::string &timestr, std::time_t timeref, const std::string &datestr)
  {
    const auto reference = std::chrono::high_resolution_clock::from_time_t(timeref);
    auto int_fn = [](std::string s) -> uint64_t {
      return std::atoll(s.substr(0, s.length()-1).c_str());
    };
    switch(timestr.back())
    {
      case 'h':
        return std::chrono::high_resolution_clock::to_time_t(reference + std::chrono::hours(int_fn(timestr)));
      case 'm':
        return std::chrono::high_resolution_clock::to_time_t(reference + std::chrono::minutes(int_fn(timestr)));
      case 's':
        return std::chrono::high_resolution_clock::to_time_t(reference + std::chrono::seconds(int_fn(timestr)));
      default:
        return to_time_t( (datestr.empty() ? today() : datestr) + " " + timestr);
    }
  }

  std::time_t absolute_time(const std::string &timestr, const std::string &datestr)
  {
    const auto now = std::chrono::high_resolution_clock::now();
    auto int_fn = [](std::string s) -> uint64_t {
      return std::atoll(s.substr(0, s.length()-1).c_str());
    };
    switch(timestr.back())
    {
      case 'h':
        return std::chrono::high_resolution_clock::to_time_t(now + std::chrono::hours(int_fn(timestr)));
      case 'm':
        return std::chrono::high_resolution_clock::to_time_t(now + std::chrono::minutes(int_fn(timestr)));
      case 's':
        return std::chrono::high_resolution_clock::to_time_t(now + std::chrono::seconds(int_fn(timestr)));
      default:
        return to_time_t( (datestr.empty() ? today() : datestr) + " " + timestr);
    }
  }

  bool is_active_between(std::time_t time_start, std::time_t time_stop)
  {
    const auto now = std::chrono::high_resolution_clock::now();
    const auto is_active = 
        (now >= std::chrono::high_resolution_clock::from_time_t(time_start))
     && (now <= std::chrono::high_resolution_clock::from_time_t(time_stop));
    return is_active;
  }

  bool is_expired(std::time_t time_stop)
  {
    const auto now = std::chrono::high_resolution_clock::now();
    const auto is_expired = std::chrono::high_resolution_clock::from_time_t(time_stop) <= now;
    return is_expired;
  }

  template<>
  bool has_time_expired(std::time_t time, std::chrono::high_resolution_clock::time_point reference)
  {
    const auto now = std::chrono::high_resolution_clock::now();
    const auto expired = now > std::chrono::high_resolution_clock::from_time_t(time);
    return expired;
  }

  template<>
  bool has_time_expired(std::chrono::high_resolution_clock::duration time, std::chrono::high_resolution_clock::time_point reference)
  {
    const auto elapsed = std::chrono::high_resolution_clock::now() - reference;
    const auto expired = elapsed > time;
    return expired;
  }

  std::string ltrim(std::string value)
  {
    value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
    return value;
  }

  long double parse_human(std::string value)
  {
    const auto number = std::stod(value.substr(0, value.length()-1).c_str());
    using namespace std::literals;
    using namespace boost::algorithm;
         if (ends_with(value, "k")) { return number * 1.0_K; }
    else if (ends_with(value, "K")) { return number * 1.0_K; }
    else if (ends_with(value, "M")) { return number * 1.0_M; }
    else if (ends_with(value, "G")) { return number * 1.0_G; }
    else throw std::runtime_error("no numeric conversion found for: " + value);
  }

  int set_name(boost::thread &thread, std::string name)
  {
    auto th = thread.native_handle();
    const auto result = pthread_setname_np(th, name.c_str());
    return result;
  }

  int set_priority(boost::thread &thread, int priority, Scheduling_Policy policy)
  {
    auto th = thread.native_handle();
    struct sched_param param;
    param.__sched_priority = priority;
    const auto sched = to_integral(policy);
    const auto result = pthread_setschedparam(th, sched, &param);
    return result;
  }

} // namespace

  using namespace rad::log;

/// *EOF*
