#ifndef TUF_CONSOLELOGGER_H
#define TUF_CONSOLELOGGER_H

#include <glog/logging.h>

class CConsole;

class CConsoleLogger :
  public google::LogSink
{
  private:
    CConsole* m_pConsole;

  public:
    CConsoleLogger();
    CConsoleLogger(CConsole* pConsole);

  public:
    void send(google::LogSeverity severity, const char* full_filename,
              const char* base_filename, int line,
              const struct ::tm* tm_time,
              const char* message, size_t message_len);
    void SetConsole(CConsole* pConsole);
};

#endif
