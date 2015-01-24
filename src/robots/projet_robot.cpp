
#include "robots/projet_robot.hpp"

#include <sstream>


#ifdef SFML_SYSTEM_WINDOWS
RobotProjet::RobotProjet(RS232& serialPort):
AbstractRobot(serialPort),
_corp(8),
_roueGauche(sf::Vector2f(26,100)),
_roueDroite(sf::Vector2f(26,100)),
_widget(0)
{
  _corp.setPoint(0, sf::Vector2f(45, 0));
  _corp.setPoint(1, sf::Vector2f(330-45, 0));
  _corp.setPoint(2, sf::Vector2f(330, 45));
  _corp.setPoint(3, sf::Vector2f(330, 330-45));
  _corp.setPoint(4, sf::Vector2f(330-45, 330));
  _corp.setPoint(5, sf::Vector2f(45, 330));
  _corp.setPoint(6, sf::Vector2f(0, 330-45));
  _corp.setPoint(7, sf::Vector2f(0, 45));
  _color= sf::Color(157,172,230,255);
  _texture.loadFromFile("projet_robot.png");
  _corp.setTexture(&_texture,true);
  setupGraphics();
}
#endif

RobotProjet::~RobotProjet(){
}

void RobotProjet::draw (sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(_roueGauche,states);
  target.draw(_roueDroite,states);
  target.draw(_corp,states);
}

void RobotProjet::setupGraphics()
{
  this->setScale(sf::Vector2f(1.f,-1.f));
  _corp.setFillColor(sf::Color(255,255,255,200));
  _corp.setOutlineColor(_color);
  _corp.setOutlineThickness(0);
  _roueGauche.setFillColor(sf::Color(255,127,39,255));
  _roueGauche.setOutlineColor(sf::Color(100,100,100,200));
  _roueGauche.setOutlineThickness(4);
  _roueDroite.setFillColor(sf::Color(255,127,39,255));
  _roueDroite.setOutlineColor(sf::Color(100,100,100,200));
  _roueDroite.setOutlineThickness(4);
  
  _corp.setOrigin(sf::Vector2f(330*0.5f,260));
  _roueGauche.setOrigin(sf::Vector2f(200+_roueGauche.getSize().x*0.5f
                                     ,_roueGauche.getSize().y*0.5f));                                                                
  _roueDroite.setOrigin(sf::Vector2f(-200+_roueDroite.getSize().x*0.5f,
                                      _roueDroite.getSize().y*0.5f));
  
  if (_widget) {
    std::stringstream s;
    s << "-----------------" << std::endl;
    s << "Etat: " << RobotProjet::translateState(_state) << std::endl;
    s << "-----------------" << std::endl;
    s << "Coordonnees: " << std::endl;
    s << "X " << getPosition().x << "mm" << std::endl;
    s << "Y " << getPosition().y << "mm" << std::endl;
    s << "Theta " << getRotation() << " (deg)"<< std::endl;
    s << "-----------------" << std::endl;
    s << "Commandes: " << std::endl;
    s << "Z: Avance" << std::endl;
    s << "S: Recule" << std::endl;
    s << "Q: Gauche" << std::endl;
    s << "D: Droite" << std::endl;
    s << "A: Stop" << std::endl;
    s << "-----------------" << std::endl;
    
    _widget->setTitleColor(_color);
    _widget->setText(sf::String(s.str()));
  }
  
}

std::string RobotProjet::translateState(sf::Uint8 e)
{
  switch (e)
  {
    case 0:  return "Stop";
    case 1:  return "Avance";
    case 2:  return "Recule";
    case 3:  return "Droite";
    case 4:  return "Gauche";
    default: return "non defini";
  }
}

RobotWidget* RobotProjet::createWidget(std::string name)
{
  if (!_widget) _widget = new RobotWidget(this,name,sf::Vector2f(132,230));
  return _widget;
}