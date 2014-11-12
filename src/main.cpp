
#include "robot.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "table_texture.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>

int main()
{
    // Create the main window
    
    sf::Vector2f terrain_size(3000,2000);
    sf::RenderWindow window(sf::VideoMode(terrain_size.x/4, 
                                          terrain_size.y/4), "Arenib Interface");

    sf::View table_view(sf::Vector2f(0,0), sf::Vector2f(terrain_size.x,-terrain_size.y));
	sf::Texture table_texture;
	table_texture.loadFromMemory(table_texture_gif,table_texture_gif_len);
	sf::RectangleShape table_sprite(terrain_size);
	table_sprite.setTexture(&table_texture,true);
	table_sprite.setOrigin(terrain_size/2.f);
    
    
    Robot robot;
    sf::Clock clock;
    
    sf::UdpSocket socket;
    if (socket.bind(2222) != sf::Socket::Done)
    {
      std::cerr << "olol cannot bind to 2222" << std::endl;
      return 1;
    }
    
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        float a=clock.getElapsedTime().asSeconds()*50;
        
        robot.setRotation(a);
        robot.setPosition(1000*cos(a*2.f*3.1415f/360.f),1000*sin(a*2.f*3.1415f/360.f));
        
        // Clear screen
        window.clear();
        
        window.setView(table_view);
		window.draw(table_sprite);
        window.draw(robot);
        // Update the window
        window.display();
    }
    return 0;
}