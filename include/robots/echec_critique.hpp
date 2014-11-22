
#ifndef ROBOT_ECHEC_CRITIQUE
#define ROBOT_ECHEC_CRITIQUE

#include "robot.hpp"

class EchecCritique : public AbstractRobot
{
  public:
    #ifdef SFML_SYSTEM_WINDOWS
    EchecCritique(HANDLE serialPort);
    #endif
    EchecCritique(const sf::IpAddress& addr, unsigned short port);
    virtual ~EchecCritique();
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual RobotWidget* createWidget(std::string name);

    static std::string translateState(sf::Uint8);

  protected:
    virtual void setupGraphics();

  private:
    sf::ConvexShape _corp;
    sf::RectangleShape _blocMoteur;
    sf::RectangleShape _roueGauche;
    sf::RectangleShape _roueDroite;
    sf::CircleShape _bumper;
    RobotWidget* _widget;
};


#endif