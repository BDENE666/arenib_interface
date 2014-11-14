
#include <SFML/Graphics.hpp>

#ifndef POPUP_WIDGET
#define POPUP_WIDGET

class PopupWidget : public sf::Drawable, public sf::Transformable
{
  public:
    PopupWidget(sf::String name, sf::Vector2f size, float radius=14.0);
		virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
    void setTitleColor(const sf::Color&);
    void setText(const sf::String&);
    
    inline bool onIt(const sf::Vector2f& v) {
      sf::Vector2f p(this->getPosition()-this->getOrigin());
      return (v.x > p.x && 
              v.y > p.y && 
              v.x < p.x + _size.x &&
              v.y < p.y + _size.x); 
    }
    
  private:
    sf::ConvexShape _window;
    sf::Text _name;
    sf::Text _text;
    sf::Vector2f _size;
    
 };


#endif