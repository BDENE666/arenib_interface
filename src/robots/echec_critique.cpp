
#include "robots/echec_critique.hpp"

#include <sstream>

EchecCritique::EchecCritique(const sf::IpAddress& addr, unsigned short port) :
AbstractRobot(addr, port),
_corp(8),
_blocMoteur(sf::Vector2f(140,130)),
_roueGauche(sf::Vector2f(28,65)),
_roueDroite(sf::Vector2f(28,65)),
_bumper(12.5),
_widget(0)
{
  _corp.setPoint(0, sf::Vector2f(37, 0));
  _corp.setPoint(1, sf::Vector2f(400-37, 0));
  _corp.setPoint(2, sf::Vector2f(400, 37));
  _corp.setPoint(3, sf::Vector2f(400, 175));
  _corp.setPoint(4, sf::Vector2f(400-115, 290));
  _corp.setPoint(5, sf::Vector2f(115, 290));
  _corp.setPoint(6, sf::Vector2f(0, 175));
  _corp.setPoint(7, sf::Vector2f(0, 37));
  _color= sf::Color(157,172,230,255);
  _blocMoteur.setOutlineThickness(10.f);
  _blocMoteur.setFillColor(sf::Color(70,70,70,255));
  _blocMoteur.setOutlineColor(sf::Color(90,90,90,255));
  setupGraphics();
}

#ifdef SFML_SYSTEM_WINDOWS
EchecCritique::EchecCritique(HANDLE serialPort):
AbstractRobot(serialPort),
_corp(8),
_blocMoteur(sf::Vector2f(140,130)),
_roueGauche(sf::Vector2f(28,65)),
_roueDroite(sf::Vector2f(28,65)),
_bumper(12.5),
_widget(0)
{
  _corp.setPoint(0, sf::Vector2f(37, 0));
  _corp.setPoint(1, sf::Vector2f(400-37, 0));
  _corp.setPoint(2, sf::Vector2f(400, 37));
  _corp.setPoint(3, sf::Vector2f(400, 175));
  _corp.setPoint(4, sf::Vector2f(400-115, 290));
  _corp.setPoint(5, sf::Vector2f(115, 290));
  _corp.setPoint(6, sf::Vector2f(0, 175));
  _corp.setPoint(7, sf::Vector2f(0, 37));
  _color= sf::Color(157,172,230,255);
  _blocMoteur.setOutlineThickness(10.f);
  _blocMoteur.setFillColor(sf::Color(70,70,70,255));
  _blocMoteur.setOutlineColor(sf::Color(90,90,90,255));
  setupGraphics();
}
#endif

EchecCritique::~EchecCritique(){
}

void EchecCritique::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(_roueGauche,states);
  target.draw(_roueDroite,states);
  target.draw(_blocMoteur,states);
  target.draw(_corp,states);
  target.draw(_bumper,states);
}

void EchecCritique::setupGraphics()
{
  this->setScale(sf::Vector2f(1.f,-1.f));
  _corp.setFillColor(sf::Color(_color.r,_color.g,_color.b,40));
  _corp.setOutlineColor(_color);
  _corp.setOutlineThickness(6);
  _roueGauche.setFillColor(sf::Color(30,30,30,255));
  _roueGauche.setOutlineColor(sf::Color(100,100,100,200));
  _roueGauche.setOutlineThickness(4);
  _roueDroite.setFillColor(sf::Color(30,30,30,255));
  _roueDroite.setOutlineColor(sf::Color(100,100,100,200));
  _roueDroite.setOutlineThickness(4);
  _blocMoteur.setOrigin(sf::Vector2f(0.5f*_blocMoteur.getSize().x
                                     ,0.35f*_blocMoteur.getSize().y));  
  _bumper.setFillColor(sf::Color(250,50,50,255));
  
  
  _corp.setOrigin(sf::Vector2f(400*0.5f,290*0.5f));
  _roueGauche.setOrigin(sf::Vector2f(400*0.5f+_roueGauche.getSize().x*0.8f
                                     ,_roueGauche.getSize().y*0.5f));                                
                                     
  _roueDroite.setOrigin(sf::Vector2f(-100+_roueDroite.getSize().x*0.5f,
                                      _roueDroite.getSize().y*0.5f));
  _roueGauche.setOrigin(sf::Vector2f(100+_roueGauche.getSize().x*0.5f,
                                     _roueGauche.getSize().y*0.5f));
                                                                      
  _bumper.setOrigin(sf::Vector2f(-50,-110));
  
  if (_widget) {
    std::stringstream s;
    s << "-----------------" << std::endl;
    s << "Etat: " << translateState(_state) << std::endl;
    s << "-----------------" << std::endl;
    s << "Coordonnees: " << std::endl;
    s << "X " << getPosition().x << "mm" << std::endl;
    s << "Y " << getPosition().y << "mm" << std::endl;
    s << "Theta " << getRotation() << " (deg)"<< std::endl;
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

std::string EchecCritique::translateState(sf::Uint8 e)
{
  switch (e)
  {
    case 1:  return "Stop force";
    case 2:  return "Avance";
    case 3:  return "Tourne";
    case 4:  return "Stop";
    case 5:  return "Position W";
    case 6:  return "Avance Free";
    case 7:  return "Position R";
    case 8:  return "Bloque";
    case 10:  return "Status Rob";
    default: return "non defini";
  }
}

RobotWidget* EchecCritique::createWidget(std::string name)
{
  if (!_widget) _widget = new RobotWidget(this,name,sf::Vector2f(132,230));
  return _widget;
}