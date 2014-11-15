

#ifndef TERRAIN__
#define TERRAIN__

#include <SFML/Graphics.hpp>

class Terrain
{
  public:
    Terrain(const sf::Vector2f& size);
    
    
    inline const sf::Vector2f& getSize() {
      return _sprite.getSize();
    }
    
  protected:
    sf::Texture _texture;
    sf::RectangleShape _sprite;
};


#endif