

#ifndef WIDGET______
#define WIDGET______

#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

template<typename T> class Singleton
 {
   public:
     static T& instance()
     {
         static T theSingleInstance; // suppose que T a un constructeur par d�faut
         return theSingleInstance;
     }
 };

class Widget : public sf::Drawable, public sf::Transformable
{
  public:
    Widget();
    virtual ~Widget();
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const=0;
    virtual void useEvent(const sf::Event& event) = 0;
    virtual bool onIt(const sf::Vector2f& v) const =0;
    virtual void update(float dt) {(void) dt;}
  protected:

};

class WidgetManager : public Singleton<WidgetManager>
{
  public:

    friend class Singleton<WidgetManager>; 
    friend class Widget;
    
    void update();
    void drawAll(sf::RenderTarget &target) const;
    void useEvent(const sf::Event& event);
  
  private:
  
  typedef std::set<Widget*>::iterator iterator;
  
    WidgetManager();
    ~WidgetManager();
    
    std::set<Widget*> _widgets;
    Widget* _focus;
    
    sf::Clock _clock;
};

#endif