
#include "robot.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "font_ibm.hpp"
#include "terrain.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <map>
#include <cmath>

#include "popup_widget.hpp"
#include "icon_texture.hpp"

sf::Font globalFont;


int main(int argc, char* argv[])
{
  globalFont.loadFromMemory(font_ibm,font_ibm_len);
  sf::RenderWindow window(sf::VideoMode(750,500), "Arenib Interface");
  window.setIcon(88, 88, icon_texture_png);
        
  Terrain* terrain=new TerrainRobotMovie(window);

  sf::View widget_view(sf::Vector2f(window.getSize().x*0.5,window.getSize().y*0.5), 
                       sf::Vector2f(window.getSize().x,window.getSize().y));
  

  std::map<std::string, AbstractRobot*> robots;

  sf::Clock clock;

  sf::UdpSocket socket;
  if (socket.bind(2222) != sf::Socket::Done)
  {
    std::cerr << "olol cannot bind to 2222" << std::endl;
    return 1;
  }
  socket.setBlocking(false);
  window.setFramerateLimit(60);

  std::cout << "Welcome to Arenib Interface" << std::endl;
  std::cout << "Server local IP: " << sf::IpAddress::getLocalAddress().toString() << std::endl;
  if (!(argc > 1 && (std::string(argv[1]) == "-ncp" ||  std::string(argv[1]) == "--no-check-public"))) {
    std::cout << "Server net   IP: " << sf::IpAddress::getPublicAddress().toString() << std::endl;
  }

  while (window.isOpen())
  {
    ///Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized)
      {
          widget_view = sf::View(sf::Vector2f(window.getSize().x*0.5,window.getSize().y*0.5), 
                                 sf::Vector2f(window.getSize().x,window.getSize().y));
      }
      WidgetManager::instance().useEvent(event);
    }

    
    ///Widgets 
    WidgetManager::instance().update();
    
    ///Networking 
    sf::IpAddress remote;
    unsigned short port;
    sf::Uint8 magic;
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
          if (!(packet >> name)) break;
          robot=robots.find(name);
          if (robot != robots.end())
          {
            if (!robot->second->extract(packet))
              std::cerr << "receive bad packet failed to update " << name << std::endl;
          }
          else 
          {
            robots[name] = AbstractRobot::createFromName(name);
            Widget* w = robots[name]->createWidget(name);
            if (!robots[name]->extract(packet))
              std::cerr << "receive bad packet failed to update " << name << std::endl;
            else {
              sf::Vector2f v = terrain->toPixelCoords(robots[name]->getPosition());
              if (v.x >=0.9*window.getSize().x) v.x = 0.9*window.getSize().x;
              if (v.y >=0.9*window.getSize().y) v.y = 0.9*window.getSize().y;
              w->setPosition(v.x, v.y);
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
    window.clear();
    
    //Robots
    window.setView(terrain->getView());
    window.draw(*terrain);
    for (std::map<std::string, AbstractRobot*>::iterator it=robots.begin();
         it != robots.end(); it++)
    {
      window.draw(*(it->second));
    }
    
    //Widgets
    window.setView(widget_view);
    WidgetManager::instance().drawAll(window);
    
    window.display();
  }
  return 0;
}
