
#include "widget.hpp"
#include <iostream>

Widget::Widget()
{
  WidgetManager::instance()._widgets.insert(this);
  if (!WidgetManager::instance()._focus) {
    WidgetManager::instance()._focus=this;
  }
}

Widget::~Widget()
{
  if (WidgetManager::instance()._focus==this)
      WidgetManager::instance()._focus=0;
  WidgetManager::instance()._widgets.erase(this);
}

WidgetManager::WidgetManager() :
_focus(0)
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
  if (_focus) target.draw(*_focus);
  for (iterator i=_widgets.begin();i != _widgets.end();i++)
  {
    if (*i != _focus)
      target.draw(**i);
  }
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
          sf::Event e;
          e.type=sf::Event::LostFocus;
          _focus->useEvent(e);
          _focus=(*i);
          e.type=sf::Event::GainedFocus;
          _focus->useEvent(e);
          return;
        }
      }
    }
  }

  if (_focus) {
    _focus->useEvent(event);
  }
}