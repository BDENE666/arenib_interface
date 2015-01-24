
#ifndef CORE____
#define CORE____

#include "widget.hpp"
#include "terrain.hpp"
#include "robot.hpp"  


class Core : public Singleton<Core>
{
  friend class Singleton<Core>; 

  public:
    
    void initialize(sf::Vector2f size=sf::Vector2f(750,500));
    
    inline Terrain* getTerrain() {
      return _terrain;
    }
    
    inline sf::RenderWindow* getWindow() {
      return _window;
    }
    
    inline std::map<std::string, AbstractRobot*>& getRobots() {
      return _robots;
    }
    
    int main(int argc, char** argv);
    
    sf::Font globalFont;
   
    void loadUARTRobotList(const std::string& f=std::string("uart_robot_list.txt"));
   
  private:
    Core();
   
    
    sf::RenderWindow* _window;
    Terrain* _terrain;
    std::map<std::string, AbstractRobot*> _robots;
    #ifdef SFML_WINDOWS
    std::map<std::string, RS232*> _rs232;
    #endif

};

#endif
