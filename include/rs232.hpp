#ifndef _RS232
#define _RS232

#include <windows.h>
#include <SFML/System.hpp>

class RS232 
{
private:
    //Variables de la communication
    DCB _config;
    HANDLE _handle;
    COMMTIMEOUTS _timeout;
    std::string _port_str;
    //var des threads
    sf::Mutex _mutex;

public:

  RS232(std::string port="COM1",BYTE parity=NOPARITY,int Speed=9600,int Data=8,BYTE stop=ONESTOPBIT,int TimeOut=500);
  ~RS232(){CloseHandle(_handle);}
  int send(const char*,unsigned int);
  int recv(char*, unsigned int);


};
#endif

