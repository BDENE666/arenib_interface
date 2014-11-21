

#ifndef TERRAIN__
#define TERRAIN__

#include <SFML/Graphics.hpp>

class Terrain : public sf::Drawable
{
  public:
    Terrain(const sf::Vector2f& size, sf::RenderWindow& window);
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const=0;
    
    inline sf::Vector2f getSize() const {
      return _view.getSize();
    }
    
    inline const sf::View& getView() const{
      return _view;
    }
    
    sf::Vector2f toPixelCoords(const sf::Vector2f& tc) const;
    sf::Vector2f toTerrainCoords(const sf::Vector2f& pc) const; 
    sf::Vector2f toPixelSize(const sf::Vector2f& ts) const;
    
  protected:
    sf::View _view;
    sf::RenderWindow* _window;
};

class TerrainRobotMovie : public Terrain
{
  public:
    TerrainRobotMovie(sf::RenderWindow& window);
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
  protected:
    sf::Texture _texture;
    sf::RectangleShape _sprite;
};


#endif