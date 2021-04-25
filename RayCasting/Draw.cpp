#include "vector.h"
#include "draw.h"

int randomgen(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

sf::RectangleShape addPixel(sf::Vector2f position, sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue)
{
    sf::RectangleShape pixel;

    pixel.setSize({ 4.f, 4.f });
    pixel.setFillColor({ red, green, blue });
    pixel.setPosition(position);

    return pixel;
}



Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor)
{
    //Vector p (x, y, 0);
    


    return Vector(randomgen(0, 255), randomgen(0, 255), randomgen(0, 255));
}
