
#ifndef ROBOT__HPP
#define ROBOT__HPP

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Robot : public sf::Drawable, public sf::Transformable
{
  public:
    Robot();
    Robot(const sf::Packet& packet);
    virtual ~Robot();
    
    virtual void 	draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
    void setupGraphics();
    
    private:
      sf::Color _color;
      sf::RectangleShape _corp;
      sf::RectangleShape _roueGauche;
      sf::RectangleShape _roueDroite;
      sf::CircleShape _bumper;
      
};


sf::Packet& operator >>(sf::Packet& packet, Robot& robot);

#endif