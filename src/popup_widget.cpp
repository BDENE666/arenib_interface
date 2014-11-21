


#include "popup_widget.hpp"
#include "robot.hpp"
#include "core.hpp"
#include <cmath>
#include <iostream>

extern sf::Font globalFont;

void PopupWidget::initBubbleShape(sf::ConvexShape& shape, const sf::Vector2f& size,float radius)
{
  unsigned k=0;
  unsigned arc=(40/4) + 1;
  shape.setPointCount(8+40);
  shape.setPoint(k++, sf::Vector2f(radius, 0.0));
  shape.setPoint(k++, sf::Vector2f(size.x-radius, 0.0));
  
  for (unsigned i=1 ;i < arc; i++)
    shape.setPoint(k++, sf::Vector2f(size.x-radius+radius*sin((float)i*0.5*3.1415/(float)arc), 
                                       radius-radius*cos((float)i*0.5*3.1415/(float)arc)));
  
  shape.setPoint(k++, sf::Vector2f(size.x, radius));
  shape.setPoint(k++, sf::Vector2f(size.x, size.y-radius));
  
  for (unsigned i=1 ;i < arc; i++)
    shape.setPoint(k++, sf::Vector2f(size.x-radius+radius*cos((float)i*0.5*3.1415/(float)arc), 
                                       size.y-radius+radius*sin((float)i*0.5*3.1415/(float)arc)));
  
  shape.setPoint(k++, sf::Vector2f(size.x-radius, size.y));
  shape.setPoint(k++, sf::Vector2f(radius, size.y));
  
  for (unsigned i=1 ;i < arc; i++)
    shape.setPoint(k++, sf::Vector2f(radius-radius*sin((float)i*0.5*3.1415/(float)arc), 
                                       size.y-radius+radius*cos((float)i*0.5*3.1415/(float)arc)));
  
  shape.setPoint(k++, sf::Vector2f(0.0, size.y-radius));
  shape.setPoint(k++, sf::Vector2f(0.0, radius));
  
  for (unsigned i=1 ;i < arc; i++)
    shape.setPoint(k++, sf::Vector2f(radius-radius*cos((float)i*0.5*3.1415/(float)arc), 
                                       radius-radius*sin((float)i*0.5*3.1415/(float)arc)));

}

PopupWidget::PopupWidget(sf::String name, sf::Vector2f size, float radius) :
_name(name,globalFont,radius),
_text("",globalFont,radius-2),
_size(size),
_pressed(false)
{
  initBubbleShape(_window,size,radius);       
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
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (onIt(sf::Vector2f(event.mouseButton.x,
                            event.mouseButton.y)))
      {
        _pressed=true;
        _previousMouse=sf::Vector2f(event.mouseButton.x,
                                    event.mouseButton.y);
      }
      else {
        this->outMousePressed(event.mouseButton.x,
                              event.mouseButton.y);
      }
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

RobotWidget::RobotWidget(AbstractRobot* r, sf::String name, sf::Vector2f size, float radius) :
PopupWidget(name,size,radius),
_robot(r),
_glue(true)
{
  initBubbleShape(_gluePlace,size,radius);  
  _gluePlace.setFillColor(sf::Color(0,0,0,0));
  _gluePlace.setOutlineThickness(2.0);
  _gluePlace.setOutlineColor(sf::Color(255,255,255,140));
}

void RobotWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  if (_pressed) target.draw(_gluePlace);
  initDraw(target,states);
}

void RobotWidget::update(float dt)
{
  PopupWidget::update(dt);
  sf::Vector2f w=Core::instance().getTerrain()->toPixelCoords(_robot->getPosition());
  sf::Vector2f v=w + sf::Vector2f(55.0,-0.5*_size.y);
  sf::Vector2u u=Core::instance().getWindow()->getSize()-sf::Vector2u(_size.x,_size.y);
  if (v.x < 0 ) v.x = 0.0;
  if (v.y < 0 ) v.y = 0.0;
  if (v.x > u.x) v.x = w.x-55.f-_size.x;
  if (v.y > u.y) v.y = u.y;
  
  if (_glue) {
    this->setPosition(v);
  }
  _gluePlace.setPosition(v);
}

void RobotWidget::useEvent(const sf::Event& event)
{
  bool p_pressed=_pressed;
  PopupWidget::useEvent(event);
  if (_pressed && !p_pressed)
  {
    _glue=false;
  }
  else if (!_pressed && p_pressed)
  {
    sf::Vector2f p= this->getPosition() + 0.5f*_size;
    if (p.x >= _gluePlace.getPosition().x && 
        p.y >= _gluePlace.getPosition().y &&
        p.x <= _gluePlace.getPosition().x + _size.x && 
        p.y <= _gluePlace.getPosition().y + _size.y)
      _glue=true;
  }
}
void RobotWidget::outMousePressed(int x, int y)
{
  if (_robot->acceptTargetPoint()) {
    sf::Vector2f ppos(x,y);
    sf::Vector2f rpos=Core::instance().getTerrain()->toTerrainCoords(ppos);
    _robot->sendTargetPoint(rpos.x,rpos.y,_robot->getRotation()*10);
    _targetPointClock.restart();
  }
}