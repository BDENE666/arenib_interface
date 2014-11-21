
#include "terrain.hpp"
#include "table_texture.hpp"
#include <iostream>

Terrain::Terrain(const sf::Vector2f& size, sf::RenderWindow& window) :
_view(sf::Vector2f(), sf::Vector2f(size.x,-size.y)),
_window(&window)
{

}

sf::Vector2f Terrain::toPixelCoords(const sf::Vector2f& tc) const
{
  sf::Vector2f v;
  v.x = (tc.x+0.5f*_view.getSize().x)/_view.getSize().x;
  v.y = (tc.y+0.5f*_view.getSize().y)/_view.getSize().y;
  return sf::Vector2f(v.x*_window->getSize().x,(v.y*_window->getSize().y));
}

sf::Vector2f Terrain::toTerrainCoords(const sf::Vector2f& pc) const
{
  sf::Vector2f v;
  v.x = (pc.x-_window->getSize().x*0.5f)/(_window->getSize().x);
  v.y = (pc.y-_window->getSize().y*0.5f)/(_window->getSize().y);
  return sf::Vector2f(v.x*_view.getSize().x,v.y*_view.getSize().y);
}

TerrainRobotMovie::TerrainRobotMovie(sf::RenderWindow& window):
Terrain(sf::Vector2f(3000,2000),window),
_sprite(sf::Vector2f(3000,2000))
{
  _texture.loadFromMemory(table_texture_gif,table_texture_gif_len);
  _sprite.setTexture(&_texture,true);
  _sprite.setOrigin(_view.getSize().x*0.5f,-_view.getSize().y*0.5f);
}

void TerrainRobotMovie::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(_sprite,states);
}