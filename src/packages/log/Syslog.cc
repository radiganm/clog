/// Syslog.cc
/// Copyright 2017 Mac Radigan
/// All Rights Reserved

  #include "packages/log/Syslog.h"
  #include <string.h>

namespace rad::log {

  Syslog::Syslog(std::string identifier, int facility) 
  {
    facility_ = facility;
    priority_ = LOG_DEBUG;
    strncpy(identifier_, identifier.c_str(), sizeof(identifier_));
    identifier_[sizeof(identifier_)-1] = '\0';
    openlog(identifier_, LOG_PID, facility_);
  }

  int Syslog::sync() 
  {
    if (buffer_.length()) 
    {
      syslog(priority_, buffer_.c_str());
      buffer_.erase();
      priority_ = LOG_DEBUG;
    }
    return 0;
  }

  int Syslog::overflow(int c) 
  {
    if (c != EOF) 
    {
      buffer_ += static_cast<char>(c);
    } 
    else 
    {
      sync();
    }
    return c;
  }

  std::ostream& operator<< (std::ostream& os, const Syslog_Priority& log_priority) 
  {
    static_cast<Syslog *>(os.rdbuf())->priority_ = (int)log_priority;
    return os;
  }

} // namespace

  using namespace rad::log;

/// *EOF*
