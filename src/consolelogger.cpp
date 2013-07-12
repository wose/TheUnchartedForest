#include <iostream>
#include <string>

#include "consolelogger.h"
#include "console.h"

CConsoleLogger::CConsoleLogger()
{
}

CConsoleLogger::CConsoleLogger(CConsole* pConsole)
{
  m_pConsole = pConsole;
}

void CConsoleLogger::send(google::LogSeverity severity,
  const char* full_filename, const char* base_filename, int line,
  const struct ::tm* tm_time, const char* message, size_t message_len)
{
  std::string strMessage(message, message_len);
  switch(severity)
  {
    case google::GLOG_INFO:
      std::cout << "INFO: " << strMessage << std::endl;
      strMessage = "^N" + strMessage;
      break;
    case google::GLOG_FATAL:
      std::cout << "FATAL: " << strMessage << std::endl;
      break;
    case google::GLOG_ERROR:
      std::cout << "ERROR: " << strMessage << std::endl;
      strMessage = "^1" + strMessage;
      break;
    case google::GLOG_WARNING:
      std::cout << "WARNING: " << strMessage << std::endl;
      strMessage = "^3" + strMessage;
      break;
  }
  if(m_pConsole)
    m_pConsole->AddLine(strMessage);
}

void CConsoleLogger::SetConsole(CConsole* pConsole)
{
  m_pConsole = pConsole;
}
