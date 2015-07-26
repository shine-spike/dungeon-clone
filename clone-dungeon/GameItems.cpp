//
//  GameItems.cpp
//  clone-dungeon
//
//  Created by Jack McKinney on 26/07/2015.
//  Copyright (c) 2015 Jack McKinney. All rights reserved.
//

#include "GameItems.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include "ResourcePath.hpp"
#define WORLD_SIZE 20
#define TILE 32

std::array<int, 2> dirToArr(int dir){
    switch (dir){
        case 0:
            return std::array<int, 2>({0, -1});
            break;
        case 1:
            return std::array<int, 2>({1, 0});
            break;
        case 2:
            return std::array<int, 2>({0, 1});
            break;
        case 3:
            return std::array<int, 2>({-1, 0});
            break;
    }
}

int dirToInt(std::array<int,2 > dir){
    if (dir[0]){
        return 2 - dir[0];
    }
    else{
        return 1 - dir[1];
    }
}


void load(Block world[][WORLD_SIZE], std::string world_name, sf::Texture* worldtextures){
    std::ifstream tilefile(resourcePath() + world_name);
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++){
            std::string str;
            tilefile >> str;
            sf::Vector2i pos((str[0] - '0') * TILE, (str[1] - '0') * TILE);
            sf::Sprite cursprite;
            cursprite.setTexture(*worldtextures);
            cursprite.setOrigin(TILE/2, TILE/2);
            cursprite.setTextureRect(sf::IntRect(pos.x, pos.y, TILE, TILE));
            cursprite.setPosition((y + 0.5) * TILE, (x + 0.5) * TILE);
            world[x][y] = Block((str[0] - '0') * 10 + (str[1] - '0'), cursprite);
        }
    }
}

void draw(sf::RenderWindow *window, Block world[][WORLD_SIZE]){
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            window->draw(world[x][y].getSprite());
        }
    }
}