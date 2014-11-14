


#include "popup_widget.hpp"
#include "robot.hpp"
#include <cmath>

extern sf::Font globalFont;

PopupWidget::PopupWidget(sf::String name, sf::Vector2f size, float radius) :
_window(8+40),
_name(name,globalFont,radius),
_text("",globalFont,radius-4),
_size(size)
{
  unsigned k=0;
  unsigned arc=(40/4) + 1;
  
  _window.setPoint(k++, sf::Vector2f(radius, 0.0));
  _window.setPoint(k++, sf::Vector2f(size.x-radius, 0.0));
  
  for (unsigned i=1 ;i < arc; i++)
    _window.setPoint(k++, sf::Vector2f(size.x-radius+radius*sin((float)i*0.5*3.1415/(float)arc), 
                                       radius-radius*cos((float)i*0.5*3.1415/(float)arc)));
  
  _window.setPoint(k++, sf::Vector2f(size.x, radius));
  _window.setPoint(k++, sf::Vector2f(size.x, size.y-radius));
  
  for (unsigned i=1 ;i < arc; i++)
    _window.setPoint(k++, sf::Vector2f(size.x-radius+radius*cos((float)i*0.5*3.1415/(float)arc), 
                                       size.y-radius+radius*sin((float)i*0.5*3.1415/(float)arc)));
  
  _window.setPoint(k++, sf::Vector2f(size.x-radius, size.y));
  _window.setPoint(k++, sf::Vector2f(radius, size.y));
  
  for (unsigned i=1 ;i < arc; i++)
    _window.setPoint(k++, sf::Vector2f(radius-radius*sin((float)i*0.5*3.1415/(float)arc), 
                                       size.y-radius+radius*cos((float)i*0.5*3.1415/(float)arc)));
  
  _window.setPoint(k++, sf::Vector2f(0.0, size.y-radius));
  _window.setPoint(k++, sf::Vector2f(0.0, radius));
  
  for (unsigned i=1 ;i < arc; i++)
    _window.setPoint(k++, sf::Vector2f(radius-radius*cos((float)i*0.5*3.1415/(float)arc), 
                                       radius-radius*sin((float)i*0.5*3.1415/(float)arc)));
                                       
  _window.setFillColor(sf::Color(42,42,42,120));
  _name.setPosition(sf::Vector2f(size.x*0.5f,radius*0.5f));
  _name.setOrigin(sf::Vector2f(_name.getLocalBounds().width*0.5,
                               _name.getLocalBounds().height*0.5));
  
}

void PopupWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(_window,states);
  target.draw(_name,states);
  target.draw(_text,states);
}
void PopupWidget::setTitleColor(const sf::Color& c) {
  _name.setColor(c);
}

void PopupWidget::setText(const sf::String& s) {
  _text.setString(s);
}