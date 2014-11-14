


#include "popup_widget.hpp"
#include "robot.hpp"
#include <cmath>
#include <iostream>

extern sf::Font globalFont;

PopupWidget::PopupWidget(sf::String name, sf::Vector2f size, float radius) :
_window(8+40),
_name(name,globalFont,radius),
_text("",globalFont,radius-2),
_size(size),
_pressed(false)
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
                                       
  _window.setFillColor(sf::Color(42,42,42,140));
  _name.setPosition(sf::Vector2f(size.x*0.5f,radius*0.5f));
  _name.setOrigin(sf::Vector2f(_name.getLocalBounds().width*0.5,
                               _name.getLocalBounds().height*0.5));
  
}

void PopupWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  initDraw(target,states);
}

void PopupWidget::initDraw(sf::RenderTarget &target, sf::RenderStates states) const
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
  _text.setPosition(sf::Vector2f(_name.getPosition().y,_name.getPosition().y+_size.y*0.5f));
  _text.setOrigin(sf::Vector2f(0.0,_text.getLocalBounds().height*0.5));
}

void PopupWidget::useEvent(const sf::Event& event){
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left && 
        onIt(sf::Vector2f(event.mouseButton.x,
                          event.mouseButton.y)))
    {
      _pressed=true;
      _previousMouse=sf::Vector2f(event.mouseButton.x,
                                  event.mouseButton.y);
    }
  } else if (event.type == sf::Event::MouseButtonReleased) {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      _pressed=false;
      _previousMouse=sf::Vector2f(event.mouseButton.x,
                                  event.mouseButton.y);
    }
  } else if (event.type == sf::Event::MouseMoved)
  {
    if (_pressed) {
      
      this->move(sf::Vector2f(event.mouseMove.x,event.mouseMove.y)-_previousMouse);
    }
    _previousMouse=sf::Vector2f(event.mouseMove.x,
                                event.mouseMove.y);
  }else if (event.type == sf::Event::Resized) {
    sf::Vector2f limits = this->getPosition() - this->getOrigin() + 0.5f*_size;
    if (limits.x > event.size.width)
      this->setPosition(event.size.width-_size.x,this->getPosition().y);
    if (limits.y > event.size.height)
      this->setPosition(this->getPosition().x,event.size.height-_size.y);
  }
}