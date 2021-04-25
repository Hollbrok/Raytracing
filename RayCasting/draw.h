#pragma once

#include "Libraries.h"

int randomgen(int min, int max);

sf::RectangleShape addPixel(sf::Vector2f position, sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue);

Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor);
