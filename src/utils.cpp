#include <fstream>
#include <string>
#include <cerrno>

#include "utils.h"

std::string LoadFile(const std::string &strFile)
{
  std::ifstream in(strFile.c_str(), std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}
