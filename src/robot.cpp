

#include "robot.hpp"
#include "robots/echec_critique.hpp"
#include "core.hpp"
#include <iostream>
#include <sstream>

AbstractRobot::AbstractRobot(const sf::IpAddress& addr, unsigned short port) :
_addr(addr),
_thread(&AbstractRobot::thread_func, this),
_port(port),
_running(false)
{
  
}

AbstractRobot::~AbstractRobot() {
  _flags=0;
  _thread.wait();
}

Robot::Robot(const sf::IpAddress& addr, unsigned short port) :
AbstractRobot(addr, port),
_corp(sf::Vector2f(300,200)),
_roueGauche(sf::Vector2f(60,110)),
_roueDroite(sf::Vector2f(60,110)),
_bumper(30),
_widget(0)
{
  _color= sf::Color(217,49,53,255);
  setupGraphics();
}

Robot::~Robot(){
}

void Robot::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(_corp,states);
  target.draw(_roueGauche,states);
  target.draw(_roueDroite,states);
  target.draw(_bumper,states);
}

void Robot::setupGraphics()
{
  _corp.setFillColor(_color);
  _corp.setOutlineColor(sf::Color(127,127,127,200));
  //_corp.setOutlineThickness(30);
  _roueGauche.setFillColor(sf::Color(30,30,30,255));
  _roueGauche.setOutlineColor(sf::Color(127,127,127,200));
  //_roueGauche.setOutlineThickness(10);
  _roueDroite.setFillColor(sf::Color(30,30,30,255));
  _roueDroite.setOutlineColor(sf::Color(127,127,127,200));
  //_roueDroite.setOutlineThickness(10);
  _bumper.setFillColor(sf::Color(50,50,50,255) + _color);
  _bumper.setOutlineColor(sf::Color(127,127,127,255));
  //_bumper.setOutlineThickness(10);
  
  
  _corp.setOrigin(_corp.getSize()*0.5f);
  _roueGauche.setOrigin(sf::Vector2f(_corp.getSize().x*0.5f+_roueGauche.getSize().x*0.8f
                                     ,_roueGauche.getSize().y*0.5f));                                
                                     
  _roueDroite.setOrigin(sf::Vector2f(-_corp.getSize().x*0.5f+_roueDroite.getSize().x*0.2f
                                     ,_roueDroite.getSize().y*0.5f));
                                                                      
  _bumper.setOrigin(sf::Vector2f(_bumper.getRadius(),-(_corp.getSize().y/2)+2*_bumper.getRadius()));
  
  if (_widget) {
    std::stringstream s;
    s << "-----------------" << std::endl;
    s << "Etat: " << (int)_state << std::endl;
    s << "-----------------" << std::endl;
    s << "Coordonnees: " << std::endl;
    s << "X " << this->getPosition().x << "mm" << std::endl;
    s << "Y " << this->getPosition().y << "mm" << std::endl;
    s << "Theta " << this->getRotation() << " (deg)"<< std::endl;
    s << "-----------------" << std::endl;
    s << "-----------------" << std::endl;
    s << "Reseau: " << std::endl;
    s << "Ip: " << _addr.toString() << std::endl;
    s << "Port: " << _port << std::endl;
    s << "-----------------" << std::endl;
    s << "Communication: " << std::endl;
    
    if (threadRunning()) {
      s << "Thread Active" << std::endl;
      s << "Period " << getSendPeriod() << "ms" << std::endl;
    } else 
      s << "Thread Disable" << std::endl;
    if (acceptTargetPoint())
      s << "TP Available " << std::endl;
    else 
      s << "TP Unavailable " << std::endl;
    s << "-----------------" << std::endl;
    
    _widget->setTitleColor(_color);
    _widget->setText(sf::String(s.str()));
  }
}

AbstractRobot* AbstractRobot::createFromName(std::string name,
                                             const sf::IpAddress& addr, 
                                             unsigned short port)
{
	if (name=="EchecCritique" || name=="EchecCritique2")
    return new EchecCritique(addr,port);
	return new Robot(addr,port);
}

bool AbstractRobot::extract(sf::Packet& packet)
{
	sf::Int16 x,y;
	sf::Int16 theta;
  sf::Lock lock(mutex);
  sf::Uint16 temp_flags;
  if (! (packet >> temp_flags)) return false; //uint8
	if (! (packet >> _state)) return false; //uint8
	if (! (packet >> x)) return false;      //int16
	if (! (packet >> y)) return false;      //int16
	if (! (packet >> theta)) return false;  //int16
	if (! (packet >> _color.r)) return false;  //uint8
	if (! (packet >> _color.g)) return false;  //uint8
	if (! (packet >> _color.b)) return false;  //uint8
	
  _flags=temp_flags;
	this->setPosition(sf::Vector2f(x,y)); //en millimetres
	this->setRotation(theta/10.f);        //en dixiemes de degrés
  
  if (!_running && _flags & 0x3FF) _thread.launch(); //(Re) launch the thread if flags say that robot want other infos
	
	if (!( this->extractExtraInfos(packet))) return false;//selon le robot
	
	this->setupGraphics();
	return true;
}

void AbstractRobot::pack(sf::Packet& packet)
{
  sf::Lock lock(mutex);
  packet << (sf::Uint16) _flags;          
  packet << (sf::Uint8) _state;             
  packet << (sf::Int16) getPosition().x; //mm  
  packet << (sf::Int16) getPosition().y; //mm  
  packet << (sf::Int16) getRotation()*10.f; //degres*10   
  packet << (sf::Uint8) _color.r; //rouge     
  packet << (sf::Uint8) _color.g; //vert      
  packet << (sf::Uint8) _color.b; //bleu      

  if (_flags & 0x8000) this->packExtraInfos(packet);  
}

void AbstractRobot::thread_func()
{
  _running=true;
  sf::Packet packet;
  
  while (_flags & 0x3FF )
  {
    if (Core::instance().getRobots().size() > 1)
    {
      packet.clear();
      packet << (sf::Uint8) 0x22;  //magic
      packet << (sf::Uint8) Core::instance().getRobots().size()-1;
    
      
      std::map<std::string, AbstractRobot*>::iterator it=Core::instance().getRobots().begin();
      for (;it != Core::instance().getRobots().end(); it++)
      {
        if (it->second != this) {
          packet << it->first; 
          it->second->pack(packet);
        }        
      }
      mutex.lock();
      _socket.send(packet, _addr, _port);
      mutex.unlock();
    }
    sf::sleep(sf::milliseconds(_flags & 0x3FF));
  }
  _running=false;
}


RobotWidget* Robot::createWidget(std::string name)
{
  if (!_widget) _widget = new RobotWidget(this,name,sf::Vector2f(132,230));
  return _widget;
}

void AbstractRobot::sendTargetPoint(sf::Int16 x, sf::Int16 y, sf::Int16 theta )
{
  if (!acceptTargetPoint()) {
     std::cerr << "Error cannot send target position IP " <<  _addr.toString() << " port " << _port << " not accepting target message" << std::endl; 
    return;
  }
  sf::Lock lock(mutex);
  sf::Packet packet;
  packet << (sf::Uint8) 0x21;  //magic
  packet << (sf::Int16) x;
  packet << (sf::Int16) y;
  packet << (sf::Int16) theta;
  std::cout << "Send target position x: " << x << " y:" << y << " to IP " <<  _addr.toString() << " port " << _port << std::endl; 
  _socket.send(packet, _addr, _port);
}

#ifdef SFML_SYSTEM_WINDOWS
AbstractRobot::AbstractRobot(RS232& serialPort):
_serialPort(&serialPort),
_thread(&AbstractRobot::thread_func, this),
_running(false)
{
}

Robot::Robot(RS232& serialPort):
AbstractRobot(serialPort),
_corp(sf::Vector2f(300,200)),
_roueGauche(sf::Vector2f(60,110)),
_roueDroite(sf::Vector2f(60,110)),
_bumper(30),
_widget(0)
{
  _color= sf::Color(217,49,53,255);
  setupGraphics();
}




#endif


