
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
#define CLONE_MAX 30

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
    Special worlds[WORLD_SIZE*WORLD_SIZE];
    load_bg(world, "resources/world1.txt", &worldtextures);
    int amount = load_fg(worlds, "resources/world1s.txt", &worldtextures);
    sf::Sprite mcsprite;
    mcsprite.setTexture(worldtextures);
    mcsprite.setTextureRect(sf::IntRect(4*TILE, TILE, TILE, TILE));
    mcsprite.setOrigin(TILE/2, TILE/2);
    mcsprite.setPosition(TILE * 3.5, TILE * 16.5);
    Mob mc(mcsprite);
    char moves[15000];
    int move_pos = 0;
    Clone clones[CLONE_MAX];
    int clone_amount = 0;
    bool action = false;
    
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
                runClones(clones, clone_amount, moves, world, worlds, amount, mc);
                mc.takeAction('0', world, worlds, amount, clones, clone_amount, mc);
                action = true;
                
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                moves[move_pos++] = '1';
                runClones(clones, clone_amount, moves, world, worlds, amount, mc);
                mc.takeAction('1', world, worlds, amount, clones, clone_amount, mc);
                action = true;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                moves[move_pos++] = '2';
                runClones(clones, clone_amount, moves, world, worlds, amount, mc);
                mc.takeAction('2', world, worlds, amount, clones, clone_amount, mc);
                action = true;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                moves[move_pos++] = '3';
                runClones(clones, clone_amount, moves, world, worlds, amount, mc);
                mc.takeAction('3', world, worlds, amount, clones, clone_amount, mc);
                action = true;
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        if (action){
            int nclone_amount = clone_amount;
            for (int i = 0; i < amount; i++){
                bool raa = false;
                if (worlds[i].getPosition() == mc.getPosition()){
                    raa = true;
                }
                for (int x = 0; x < clone_amount; x++){
                    if (worlds[i].getPosition() == clones[x].getPosition()){
                        raa = true;
                    }
                }
                if (raa){
                    worlds[i].steppedOn(worlds, amount, clones, &nclone_amount, &worldtextures);
                    worlds[i].setSteppedOn(true);
                }
                else if (!raa && worlds[i].getSteppedOn()){
                    worlds[i].steppedOff(worlds, amount, clones, &nclone_amount, &worldtextures);
                    worlds[i].setSteppedOn(false);
                }
            }
            clone_amount = nclone_amount;
            action = false;
        }

        // Clear screen
        window.clear();
        
        draw(&window, world, worlds, amount);
        // Update the window
        window.draw(mc.getSprite());
        
        for (int i = 0; i < clone_amount; i++){
            window.draw(clones[i].getSprite());
        }
        
        window.display();
    }

    return EXIT_SUCCESS;
}
