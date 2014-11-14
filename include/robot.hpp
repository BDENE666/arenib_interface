
#ifndef ROBOT__HPP
#define ROBOT__HPP

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "popup_widget.hpp"

class AbstractRobot : public sf::Drawable, public sf::Transformable
{
	public:
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const = 0;
		static AbstractRobot* createFromName(std::string name);
		bool extract(sf::Packet& packet);
    
    //Create optionnal widget
    virtual inline Widget* createWidget(std::string name) { (void) name; return 0;}
	
	protected:
		
		virtual void setupGraphics()=0;
		inline virtual bool extractExtraInfos(sf::Packet& packet) {
			(void) packet;
			return true;
		}
	  sf::Color _color;
    sf::Uint8 _state;
};

class Robot : public AbstractRobot
{
  public:
    Robot();
    virtual ~Robot();
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
    
  protected:
	virtual void setupGraphics();
	
  private:
    sf::RectangleShape _corp;
    sf::RectangleShape _roueGauche;
    sf::RectangleShape _roueDroite;
    sf::CircleShape _bumper;
};

class EchecCritique : public AbstractRobot
{
  public:
    EchecCritique();
    virtual ~EchecCritique();
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual Widget* createWidget(std::string name);
    
    static std::string translateState(sf::Uint8);
    
  protected:
    virtual void setupGraphics();
	
  private:
    sf::ConvexShape _corp;
    sf::RectangleShape _roueGauche;
    sf::RectangleShape _roueDroite;
    sf::CircleShape _bumper;
    PopupWidget* _widget;
};


#endif