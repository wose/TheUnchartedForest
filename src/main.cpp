#include "tuf.h"

int main(int argc, char *argv[])
{
  if(theApp.Initialize())
    {
      theApp.Run();
    }
  
  return 0;
}

