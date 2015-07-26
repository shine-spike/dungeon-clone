
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include "ResourcePath.hpp"
#include "GameItems.h"
#define WORLD_SIZE 20
#define TILE 32

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(640, 640), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "resources/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Create the game resources
    sf::Texture worldtextures;
    if (!worldtextures.loadFromFile(resourcePath() + "resources/worldtextures.png")) {
        return EXIT_FAILURE;
    }
    Block world[WORLD_SIZE][WORLD_SIZE];
    load(world, "resources/world1.txt", &worldtextures);
    sf::Texture mctexture;
    if (!mctexture.loadFromFile(resourcePath() + "resources/mc.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite mcsprite;
    mcsprite.setTexture(mctexture);
    mcsprite.setOrigin(TILE/2, TILE/2);
    mcsprite.setPosition(TILE * 10.5, TILE * 10.5);
    Mob mc(mcsprite);
    char moves[5000];
    int move_pos = 0;
    Clone clones[30];
    int clone_amount = 0;
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                moves[move_pos++] = '0';
                runClones(clones, clone_amount, moves, world);
                mc.takeAction('0', world);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                moves[move_pos++] = '1';
                runClones(clones, clone_amount, moves, world);
                mc.takeAction('1', world);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                moves[move_pos++] = '2';
                runClones(clones, clone_amount, moves, world);
                mc.takeAction('2', world);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                moves[move_pos++] = '3';
                runClones(clones, clone_amount, moves, world);
                mc.takeAction('3', world);
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        if (move_pos == 10){
            int m[2] {10,10};
            clones[0] = makeClone(m, &mctexture);
            clone_amount = 1;
        }

        // Clear screen
        window.clear();
        
        draw(&window, world);
        // Update the window
        window.draw(mc.getSprite());
        
        for (int i = 0; i < clone_amount; i++){
            window.draw(clones[i].getSprite());
        }
        
        window.display();
    }

    return EXIT_SUCCESS;
}
