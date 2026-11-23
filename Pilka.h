#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Paletka.h"

class Pilka {
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:
    Pilka(float sx, float sy, float svx, float svy, float sradius) {
        x = sx;
        y = sy;
        vx = svx;
        vy = svy;
        radius = sradius;

        shape.setRadius(radius);
        shape.setOrigin({ radius, radius });
        shape.setPosition({ x, y });
        shape.setFillColor(sf::Color::Green);
    }

    void move() {
        x += vx;
        y += vy;
        shape.setPosition({ x, y });
    }

    void bounceX() { vx = -vx; }
    void bounceY() { vy = -vy; }

    void collideWalls(float width, float height) {
        
        if (x - radius <= 0) {
            bounceX();
            x = radius;
            std::cout << "HIT LEFT WALL\n";
        }
        
        if (x + radius >= width) {
            bounceX();
            x = width - radius;
            std::cout << "HIT RIGHT WALL\n";
        }
        
        if (y - radius <= 0) {
            bounceY();
            y = radius;
            std::cout << "HIT TOP WALL\n";
        }
       
        shape.setPosition({ x, y });
    }

    bool collidePaddle(const Paletka& p) {
        float palX = p.getX();
        float palY = p.getY();
        float palW = p.getSzerokosc();
        float palH = p.getWysokosc();

        bool poziomo = (x >= palX - palW / 2) && (x <= palX + palW / 2);
        bool pionowo = ((y + radius) >= (palY - palH / 2)) && ((y - radius) < (palY - palH / 2));

        if (poziomo && pionowo) {
            vy = -std::abs(vy);
            y = (palY - palH / 2) - radius;
            shape.setPosition({ x, y });
            return true;
        }
        return false;
    }

    void draw(sf::RenderTarget& target) {
        target.draw(shape);
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getVx() const { return vx; }
    float getVy() const { return vy; }
    float getRadius() const { return radius; }
};
