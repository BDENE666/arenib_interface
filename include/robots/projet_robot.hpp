
#ifndef ROBOT_PROJET_ROBOT
#define ROBOT_PROJET_ROBOT

#include "robot.hpp"

class RobotProjet : public AbstractRobot
{
  public:
    #ifdef SFML_SYSTEM_WINDOWS
    RobotProjet(RS232& serialPort);
    #endif
    virtual ~RobotProjet();
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual RobotWidget* createWidget(std::string name);

    static std::string translateState(sf::Uint8);

  protected:
    virtual void setupGraphics();

  private:
    sf::ConvexShape _corp;
    sf::RectangleShape _roueGauche;
    sf::RectangleShape _roueDroite;
    sf::Texture _texture;
    RobotWidget* _widget;
};


#endif