/// Syslog.h
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include <syslog.h>
  #include <iostream>

#pragma once

  #define SYSOG__IDENTIFIER_LENGTH (50)

namespace rad::log {

  enum class Syslog_Priority {
    Emerg   = LOG_EMERG,
    Alert   = LOG_ALERT,
    Crit    = LOG_CRIT,
    Err     = LOG_ERR,
    Warning = LOG_WARNING,
    Notice  = LOG_NOTICE,
    Info    = LOG_INFO,
    Debug   = LOG_DEBUG
  };

  class Syslog : public std::basic_streambuf<char, std::char_traits<char> > {
    public:
      explicit Syslog(std::string identifier, int facility);
      static void initialize(std::string name)
      {
        std::clog.rdbuf(new Syslog(name.c_str(), LOG_LOCAL0));
      };

    protected:
      int sync();
      int overflow(int c);

    private:
      friend std::ostream& operator<< (std::ostream& os, const Syslog_Priority& log_priority);
      std::string buffer_;
      int facility_;
      int priority_;
      char identifier_[SYSOG__IDENTIFIER_LENGTH];
  };

  std::ostream& operator<< (std::ostream& os, const Syslog_Priority& log_priority);

} // namespace

/// *EOF*
