/// Logger.cc
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include "packages/log/Logger.h"
  #include "packages/common/util.h"
  #include <iostream>
  #include <iomanip>
  #include <cstring>

namespace rad::log {

  #define LOGGER__DEFAULT_LOG_LEVEL (Syslog_Priority::Info)

  Logger::Logger(std::string name)
    : verbose_(false)
  {
    Syslog::initialize(name);
    std::clog << LOGGER__DEFAULT_LOG_LEVEL;
  }

  Logger::~Logger()
  {
  }

  Logger& Logger::operator<<(std::string value)
  {
    if(verbose_) 
    {
      std::ios::fmtflags flags(std::cout.flags());
      std::cout << value;
      std::cout.flags(flags);
    }
    std::clog << value;
    return *this;
  }

  Logger& Logger::operator<<(std::stringstream &ss)
  {
    const auto value = ss.str();
    if(verbose_) 
    {
      std::ios::fmtflags flags(std::cout.flags());
      std::cout << value;
      std::cout.flags(flags);
    }
    std::clog << value;
    *this << std::endl;
    ss.str(std::string());
    ss.clear();
    return *this;
  }

  Logger& Logger::operator<<(std::ostream& (*f)(std::ostream&))
  {
    if(verbose_) f(std::cout);
    std::clog.flush();
    return *this;
  }

  Logger& Logger::operator<<(const Syslog_Priority& log_priority)
  {
    std::clog << log_priority;
    return *this;
  }

} // namespace

  using namespace rad::log;

/// *EOF*
