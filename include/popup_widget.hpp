
#include <SFML/Graphics.hpp>

#ifndef POPUP_WIDGET
#define POPUP_WIDGET

#include "widget.hpp"

class PopupWidget : public Widget
{
  public:
    PopupWidget(sf::String name, sf::Vector2f size, float radius=14.0);
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
    void setTitleColor(const sf::Color&);
    void setText(const sf::String&);
    virtual void useEvent(const sf::Event& event);
    
    virtual inline bool onIt(const sf::Vector2f& v) const {
      sf::Vector2f p(this->getPosition()-this->getOrigin());
      return (v.x > p.x && 
              v.y > p.y && 
              v.x < p.x + _size.x &&
              v.y < p.y + _size.x); 
    }
    
    
  protected:
    void initDraw(sf::RenderTarget &target, sf::RenderStates states) const;
    
  private:
    sf::ConvexShape _window;
    sf::Text _name;
    sf::Text _text;
    sf::Vector2f _size;
    sf::Vector2f _previousMouse;
    bool _pressed;
    
 };


#endif