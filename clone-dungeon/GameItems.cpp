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

void draw(sf::RenderWindow *window, Block world[][WORLD_SIZE], Special worlds[], int worlds_size){
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            window->draw(world[x][y].getSprite());
        }
    }
    for (int x = 0; x < worlds_size; x++){
        window->draw(worlds[x].getSprite());
    }
}

void runClones(Clone clones[], int clone_amount, char moves[], Block world[][WORLD_SIZE], Special specs[], int spec_amount, Mob mc){
    for (int i = 0; i < clone_amount; i++){
        clones[i].takeAction(moves[clones[i].move_pos++], world, specs, spec_amount, clones, clone_amount, mc);
    }
}

Clone makeClone(int pos[], sf::Texture *mctexture){
    Clone clone;
    sf::Sprite sprite;
    sprite.setTexture(*mctexture);
    sprite.setOrigin(TILE/2, TILE/2);
    sprite.setPosition(TILE * (pos[0] + 0.5), TILE * (pos[1] + 0.5));
    sprite.setTextureRect(sf::IntRect(4*TILE, TILE, TILE, TILE));
    clone.setSprite(sprite);
    clone.move_pos = 0;
    return clone;
}

Block makeBlock(std::string str, int poss[], sf::Texture *worldtextures){
    sf::Vector2i pos((str[0] - '0') * TILE, (str[1] - '0') * TILE);
    sf::Sprite cursprite;
    cursprite.setTexture(*worldtextures);
    cursprite.setOrigin(TILE/2, TILE/2);
    cursprite.setTextureRect(sf::IntRect(pos.x, pos.y, TILE, TILE));
    cursprite.setPosition((poss[1] + 0.5) * TILE, (poss[0] + 0.5) * TILE);
    return Block((str[0] - '0') * 10 + (str[1] - '0'), cursprite);
    
}

Special makeSpec(int x, int y, int type, sf::Texture *worldtextures, int id){
    sf::Sprite cursprite;
    cursprite.setTexture(*worldtextures);
    cursprite.setOrigin(TILE/2, TILE/2);
    cursprite.setTextureRect(sf::IntRect(type / 10 * 32, type % 10 * 32 , TILE, TILE));
    cursprite.setPosition((y + 0.5) * TILE, (x + 0.5) * TILE);
    return Special(type, cursprite, id);
}

void load_bg(Block world[][WORLD_SIZE], std::string world_name, sf::Texture* worldtextures){
    std::ifstream tilefile(resourcePath() + world_name);
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++){
            std::string str;
            tilefile >> str;
            int n[2] = {x, y};
            world[x][y] = makeBlock(str, n, worldtextures);
        }
    }
}

int load_fg(Special world[], std::string world_name, sf::Texture* worldtextures){
    std::ifstream tilefile(resourcePath() + world_name);
    int amount;
    tilefile >> amount;
    for (int x = 0; x < amount; x++)
    {
        int type, posx, posy, id;
        tilefile >> type;
        tilefile >> posy;
        tilefile >> posx;
        tilefile >> id;
        world[x] = makeSpec(posx, posy, type, worldtextures, id);
    }
    return amount;
}

bool accessible(int y, int x, Block world[][WORLD_SIZE], Clone clones[], int clone_amount, Special specs[], int spec_amount, Mob mc){
    if (!world[x][y].canAccess()){
        return false;
    }
    for (int i = 0; i < spec_amount; i++){
        if(specs[i].getPosition() == sf::Vector2i(y, x) && !specs[i].canAccess()){
            return false;
        }
    }
    for (int i = 0; i < clone_amount; i++){
        if (clones[i].getPosition() == sf::Vector2i(y, x)){
            return false;
        }
    }
    if (mc.getPosition() == sf::Vector2i(y, x)){
        return false;
    }
    return true;
}