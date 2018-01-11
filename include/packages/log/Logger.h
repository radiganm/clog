/// Logger.h
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include "packages/log/Syslog.h"
  #include <atomic>
  #include <sstream>

#pragma once

namespace rad::log {

  class Logger
  {
    public:
      Logger(std::string name);
      virtual ~Logger();
      void set_verbose(bool value) { verbose_ = value; }
      Logger& operator<<(std::string value);
      Logger& operator<<(std::stringstream &ss);
      Logger& operator<<(std::ostream& (*f)(std::ostream&));
      Logger& operator<<(const Syslog_Priority& log_priority);
    private:
      std::atomic<bool> verbose_;
  };

} // namespace

/// *EOF*
