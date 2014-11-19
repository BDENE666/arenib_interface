
#include "robots/echec_critique.hpp"

#include <sstream>

EchecCritique::EchecCritique(const sf::IpAddress& addr, unsigned short port) :
AbstractRobot(addr, port),
_corp(8),
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
  _color= sf::Color(157,172,230,50);
  setupGraphics();
}

EchecCritique::~EchecCritique(){
}

void EchecCritique::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(_roueGauche,states);
  target.draw(_roueDroite,states);
  target.draw(_corp,states);
  target.draw(_bumper,states);
}

void EchecCritique::setupGraphics()
{
  this->setScale(sf::Vector2f(1.f,-1.f));
  _corp.setFillColor(_color);
  _corp.setOutlineColor(sf::Color(_color.r,_color.g,_color.b,255));
  _corp.setOutlineThickness(6);
  _roueGauche.setFillColor(sf::Color(30,30,30,255));
  _roueGauche.setOutlineColor(sf::Color(100,100,100,200));
  _roueGauche.setOutlineThickness(4);
  _roueDroite.setFillColor(sf::Color(30,30,30,255));
  _roueDroite.setOutlineColor(sf::Color(100,100,100,200));
  _roueDroite.setOutlineThickness(4);
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
    s << "X " << this->getPosition().x << "mm" << std::endl;
    s << "Y " << this->getPosition().y << "mm" << std::endl;
    s << "Theta " << this->getRotation() << " (deg)"<< std::endl;
    s << "-----------------" << std::endl;
    
    _widget->setTitleColor(sf::Color(156,156,156,255));
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
  if (!_widget) _widget = new RobotWidget(this,name,sf::Vector2f(132,135));
  return _widget;
}