
#include <SFML/Graphics.hpp>

#ifndef POPUP_WIDGET
#define POPUP_WIDGET

#include "widget.hpp"
#include <SFML/System.hpp>
class AbstractRobot;

class PopupWidget : public Widget
{
  public:
    PopupWidget(sf::String name, sf::Vector2f size, float radius=14.f);
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
    void setTitleColor(const sf::Color&);
    void setText(const sf::String&);
    virtual void useEvent(const sf::Event& event);
    
    virtual inline bool onIt(const sf::Vector2f& v) const {
      sf::Vector2f p(this->getPosition()-this->getOrigin());
      return (v.x > p.x && 
              v.y > p.y && 
              v.x < p.x + _size.x &&
              v.y < p.y + _size.y); 
    }
    
    
    static void initBubbleShape(sf::ConvexShape& shape, const sf::Vector2f& size,float radius=14.f);
    
  protected:
    inline virtual void outMousePressed( int x, int y) { (void) x; (void) y; } //outside the Popup
    inline virtual void outMouseReleased( int x, int y) { (void) x; (void) y; } //outside the Popup
    inline virtual void outMouseMoved( int x, int y) { (void) x; (void) y; } //outside the Popup
    
    void initDraw(sf::RenderTarget &target, sf::RenderStates states) const;
    sf::ConvexShape _window;
    sf::Text _name;
    sf::Text _text;
    sf::Vector2f _size;
    sf::Vector2f _previousMouse;
    bool _pressed;
    
 };

 
class RobotWidget : public PopupWidget
{
  public:
    RobotWidget(AbstractRobot* r, sf::String name, sf::Vector2f size, float radius=14.0);
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void update(float dt);
    virtual void useEvent(const sf::Event& event);
    
  protected:
  
    virtual void outMousePressed( int x, int y);
    virtual void outMouseReleased( int x, int y);
    virtual void outMouseMoved( int x, int y);
    
    sf::ConvexShape _gluePlace;
    AbstractRobot* _robot;
    bool _glue;
    
    sf::Clock _targetPointClock;
    sf::ConvexShape _targetPoint;
    float _targetAngle;
};

#endif