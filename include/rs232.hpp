#ifndef _RS232
#define _RS232

#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

class AbstractRobot;

class RS232 
{
private:
  void thread_func();
  
  //Variables de la communication
  DCB _config;
  HANDLE _handle;
  COMMTIMEOUTS _timeout;
  std::string _port_str;
  AbstractRobot* _robot;
  //var des threads
  sf::Mutex _mutex;
  sf::Thread _thread;
  bool _ragequit;
  
  int inStruct;
  std::string console;
  sf::Packet packet;
    
public:
  RS232(std::string port="COM1:",BYTE parity=NOPARITY,int Speed=9600,int Data=8,BYTE stop=ONESTOPBIT,int TimeOut=100);
  ~RS232();
  int send(const char*, unsigned int);
  int recv(char*, unsigned int);
  void update();
  
  inline void setRobot(AbstractRobot* r) {
    _mutex.lock();
    _robot=r;
    _mutex.unlock();
  }

};
#endif

