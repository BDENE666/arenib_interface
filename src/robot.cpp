

#include "robot.hpp"
#include "robots/echec_critique.hpp"
#include "core.hpp"

#include <sstream>

AbstractRobot::AbstractRobot(const sf::IpAddress& addr, unsigned short port) :
_addr(addr),
_thread(&AbstractRobot::thread_func, this),
_port(port)
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
    
    _widget->setTitleColor(_color);
    _widget->setText(sf::String(s.str()));
  }
}

AbstractRobot* AbstractRobot::createFromName(std::string name,
                                             const sf::IpAddress& addr, 
                                             unsigned short port)
{
	if (name=="EchecCritique")
    return new EchecCritique(addr,port);
	return new Robot(addr,port);
}

bool AbstractRobot::extract(sf::Packet& packet)
{
	sf::Int16 x,y;
	sf::Int16 theta;
  if (! (packet >> _flags)) return false; //uint8
	if (! (packet >> _state)) return false; //uint8
	if (! (packet >> x)) return false;      //int16
	if (! (packet >> y)) return false;      //int16
	if (! (packet >> theta)) return false;  //int16
	if (! (packet >> _color.r)) return false;  //uint8
	if (! (packet >> _color.g)) return false;  //uint8
	if (! (packet >> _color.b)) return false;  //uint8
	
	this->setPosition(sf::Vector2f(x,y)); //en millimetres
	this->setRotation(theta/10.f);        //en dixiemes de degrés
	
	if (!( this->extractExtraInfos(packet))) return false;//selon le robot
	
	this->setupGraphics();
	return true;
}

bool AbstractRobot::pack(sf::Packet& packet)
{
  packet << (sf::Uint16) _flags;              // uint16 
  packet << std::string("Nom de mon robot"); // std::string <-- identifiant du robot gardez tjs le même
  packet << (sf::Uint8) _state;                // uint8 
  /*packet << (sf::Int16) position_x; //mm     // int16 
  packet << (sf::Int16) position_y; //mm     // int16 
  packet << (sf::Int16) theta; //degres*10   // int16 
  packet << (sf::Uint8) color_r; //rouge     // uint8 
  packet << (sf::Uint8) color_g; //vert      // uint8 
  packet << (sf::Uint8) color_b; //bleu      // uint8 */

  return false;
}

void AbstractRobot::thread_func()
{
  sf::UdpSocket socket;
  sf::Packet packet;
  
  while (_flags & 0x3FFFF)
  {
    if (Core::instance().getRobots().size() > 1)
    {
      packet.clear();
      packet << (sf::Uint8) 0x22;  //magic       // uint8
      packet << (sf::Uint8) Core::instance().getRobots().size()-1;
    
      socket.send(packet, _addr, _port);
    }
  }
  
  /*
  
	sf::Int16 x,y;
	sf::Int16 theta;

  sf::Vector2f position=this->getPosition(); //en millimetres
  float theta_f= (this->getRotation())*10;   //en dixiemes de degrés

  x=(sf::Int16)(position.x);
  y=(sf::Int16)(position.y);
  theta=(sf::Int16)theta_f;

	if (! (packet << _state)) return false; //uint8
	if (! (packet << x)) return false;      //int16
	if (! (packet << y)) return false;      //int16
	if (! (packet << theta)) return false;  //int16
	if (! (packet << _color.r)) return false;  //uint8
	if (! (packet << _color.g)) return false;  //uint8
	if (! (packet << _color.b)) return false;  //uint8
	
	*/
}


RobotWidget* Robot::createWidget(std::string name)
{
  if (!_widget) _widget = new RobotWidget(this,name,sf::Vector2f(132,135));
  return _widget;
}
