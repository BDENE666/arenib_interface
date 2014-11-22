
#include "widget.hpp"
#include <iostream>

Widget::Widget()
{
  WidgetManager::instance()._widgets.insert(this);
}

Widget::~Widget()
{
  if (WidgetManager::instance()._focus==this)
      WidgetManager::instance()._focus=0;
  WidgetManager::instance()._widgets.erase(this);
}

WidgetManager::WidgetManager() :
_view(),
_focus(0),
_clickOn(0)
{
}

WidgetManager::~WidgetManager() 
{
  iterator i=_widgets.begin();
  while ( i != _widgets.end() ) 
  {
    delete *i;//Bcp trop propre ^^
    i=_widgets.begin();
  }
}

void WidgetManager::update() {
  float dt=_clock.getElapsedTime().asSeconds();
  _clock.restart();
  for (iterator i=_widgets.begin();i != _widgets.end();i++)
  {
    (*i)->update(dt);
  }
}

void WidgetManager::drawAll(sf::RenderTarget &target) const
{
  for (iterator i=_widgets.begin();i != _widgets.end();i++)
  {
    if (*i != _focus)
      target.draw(**i);
  }
  if (_focus) target.draw(*_focus);
}

void WidgetManager::useEvent(const sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      for (iterator i=_widgets.begin();i != _widgets.end();i++)
      {
        if (_focus != (*i) && 
           (*i)->onIt(sf::Vector2f(event.mouseButton.x,
                                    event.mouseButton.y))) {
          _clickOn=(*i);
          return;
        }
      }
    }
  } else if (event.type == sf::Event::MouseButtonReleased && _clickOn)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      for (iterator i=_widgets.begin();i != _widgets.end();i++)
      {
        if (_clickOn->onIt(sf::Vector2f(event.mouseButton.x,
                                        event.mouseButton.y))) {
          setFocus(_clickOn);
          return;
        }
      }
    }
  } else if (event.type == sf::Event::Resized)
  {
    _view = sf::View(sf::Vector2f(event.size.width*0.5,event.size.height*0.5), 
                     sf::Vector2f(event.size.width,event.size.height));
    return;
  }

  if (_focus) {
    _focus->useEvent(event);
  }
}
void WidgetManager::setFocus(Widget* w)
{
  if (!w) return;
  sf::Event e;
  if (_focus) {
    e.type=sf::Event::LostFocus;
    _focus->useEvent(e);
  }
  _focus=w;
  e.type=sf::Event::GainedFocus;
  _focus->useEvent(e);
}