

#include "core.hpp"
#include <iostream>
#include "font_ibm.hpp"
#include "icon_texture.hpp"
#include "robots/projet_robot.hpp"
#include <fstream>


Core::Core() : 
_window(0),
 _terrain(0)
{
}

void Core::initialize(sf::Vector2f size)
{
  globalFont.loadFromMemory(font_ibm,font_ibm_len);
  loadUARTRobotList();
  _window=new sf::RenderWindow(sf::VideoMode(size.x,size.y), "Arenib Interface");
  _window->setFramerateLimit(60);
  _window->setIcon(128, 128, icon_texture_png);
  _terrain=new TerrainRobotMovie(*_window);
  WidgetManager::instance().initView(sf::View(sf::Vector2f(size.x*0.5,size.y*0.5), 
                                              sf::Vector2f(size.x,size.y)));
}

int Core::main(int argc, char** argv)
{
  initialize();
  sf::UdpSocket socket;
  if (socket.bind(2222) != sf::Socket::Done)
  {
    std::cerr << "olol cannot bind to 2222" << std::endl;
    return 1;
  }
  socket.setBlocking(false);

  std::cout << "Welcome to Arenib Interface" << std::endl;
  std::cout << "Server local IP: " << sf::IpAddress::getLocalAddress().toString() << std::endl;
  if (!(argc > 1 && (std::string(argv[1]) == "-ncp" ||  std::string(argv[1]) == "--no-check-public"))) {
    std::cout << "Server net   IP: " << sf::IpAddress::getPublicAddress().toString() << std::endl;
  }

  while (_window->isOpen())
  {
    ///Process events
    sf::Event event;
    while (_window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        _window->close();
      else if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code>=sf::Keyboard::A && event.key.code<=sf::Keyboard::Z)
        {
        }
      }
      else 
        WidgetManager::instance().useEvent(event);
    }
    //serialPort->update();

    
    ///Widgets 
    WidgetManager::instance().update();
    
    ///Networking 
    sf::IpAddress remote;
    unsigned short port;
    sf::Uint8 magic;
    sf::Uint8 n;
    std::string name;
    sf::Socket::Status status;
    do {
      sf::Packet packet;
      status = socket.receive(packet,remote,port);
      std::map<std::string, AbstractRobot*>::iterator robot;
      switch (status)
      {
        case sf::Socket::Done:
          if (!(packet >> magic) || magic != 0x22) break;
          if (!(packet >> n)) break;
          for (unsigned int i=0;i < n;i++) {
            if (!(packet >>  name)) break;
            robot=_robots.find(name);
            if (robot != _robots.end())
            {
              if (!robot->second->extract(packet))
                std::cerr << "receive bad packet failed to update " << name << std::endl;
              else 
                robot->second->updateRobotEndpoint(remote,port);
            }
            else 
            {
              _robots[name] = AbstractRobot::createFromName(name,remote,port);
              Widget* w = _robots[name]->createWidget(name);
              if (!_robots[name]->extract(packet))
                std::cerr << "receive bad packet failed to update " << name << std::endl;
              if (_robots.size() == 1)
                WidgetManager::instance().setFocus(w);
              /*else {
                sf::Vector2f v = _terrain->toPixelCoords(_robots[name]->getPosition());
                if (v.x >=0.9*_window->getSize().x) v.x = 0.9*_window->getSize().x;
                if (v.y >=0.9*_window->getSize().y) v.y = 0.9*_window->getSize().y;
                w->setPosition(v.x, v.y);
              }*/
            }
          }
          break;
        case sf::Socket::Error:
          std::cerr << "Socket error !!!" << std::endl;
          break;
        case sf::Socket::Disconnected:
          std::cerr << "Socket disconnected !!!" << std::endl;
          break;
        default:
          break;
      }
    }
    while (status==sf::Socket::Done);
    
    ///Drawing
    _window->clear();
    
    //Robots
    _window->setView(_terrain->getView());
    _window->draw(*_terrain);
    for (std::map<std::string, AbstractRobot*>::iterator it=_robots.begin();
         it != _robots.end(); it++)
    {
      _window->draw(*(it->second));
    }
    
    //Widgets
    _window->setView(WidgetManager::instance().getView());
    WidgetManager::instance().drawAll(*_window);
    
    _window->display();
  }
  return 0;
}


void Core::loadUARTRobotList(const std::string& filename)
{
  char line[256];
  char port[256];
  char name[256];
  
  std::ifstream ifs(filename.c_str(), std::ifstream::in);
  if (!ifs) {
    std::cerr << "error unable to open " << filename << std::endl;
    return;
  }
  
  while (ifs.good() && ifs.getline(line, 256, '\n'))
  {
    unsigned int baudrate=9600;
    if (line[0]=='#')
    {
      continue;
    }
    if (sscanf("%s %s %u",line,name,port,&baudrate) >=2)
    {
      const std::string port_str=std::string(port);
      const std::string name_str=std::string(name);
      #ifdef SFML_WINDOWS
      
      RS232* rs232;
      if (_rs232.find(port_str) != _rs232.end())
      {
        rs232=_rs232[port_str];
      } else {
        rs232= new RS232(port_str,NOPARITY,baudrate);
        _rs232[port_str]=rs232;
      }
      AbstractRobot robot=_robots.find(name_str);
      if (robot != _robots.end())
      {
        //set serial Port
      }
      else 
      {
        std::cerr << "registring " << name_str << " with port " << port_str << " and baudrate " << baudrate std::endl;
        _robots[name_str] = AbstractRobot::createFromName(name_str,*rs232);
        Widget* w = _robots[name_str]->createWidget(name_str);
        if (_robots.size() == 1)
          WidgetManager::instance().setFocus(w);
      }
      #endif
    }
    else {
      std::cerr << "Error file: " << filename << " line: \"" << line << "\" too few arguments" << std::endl;
    }
  }
}