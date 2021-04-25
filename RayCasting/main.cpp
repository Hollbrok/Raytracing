#include "vector.h"
#include "draw.h"

using namespace sf;

#define SC_Width 640
#define SC_Height 480
#define BG_COLOR Color::Blue

#define SP_RAD 100.f
#define SP_X_CENTRE SC_Width / 2 - SP_RAD
#define SP_Y_CENTRE SC_Height / 2 - SP_RAD
#define SP_COLOR Color::Red

// Цвет материала сферы
#define MATER_COLOR 0.0, 1.0, 0.0

// Цвет источника
#define LIGHT_COLOR 1.0, 0.7, 0.0

// Окружающий цвет
#define AMBIENT_COLOR 0.2, 0.2, 0.2

#define ALL_VECTORS viewPos, materialColor, lightColor, ambientColor

// Vector color будет с значениями от 0 до 1
// Нужно как по сделать буффер размера [SC_Width][SC_Heigh] и есть в окрестности сферы входит, то рассчитать по формуле цвет

int main()
{

	RenderWindow window(VideoMode(SC_Width, SC_Height), "");
	
	window.clear(BG_COLOR);
	
	// orwindow.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	CircleShape sphere(SP_RAD);
	sphere.setFillColor(SP_COLOR);
	sphere.setPosition(SP_X_CENTRE, SP_Y_CENTRE);


	const Vector viewPos(0, 0, (double)(+5) * (double)SP_RAD);

	const Vector materialColor(MATER_COLOR);
	const Vector lightColor(LIGHT_COLOR);
	const Vector ambientColor(AMBIENT_COLOR);

	/* creating image */
	Image buffer;
	buffer.create(SC_Width, SC_Height);

	/* creating texture */
	Texture texture;
	texture.loadFromImage(buffer);
	
	/* Creating sprite with texture */
	Sprite sprite(texture);

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	float fps;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		start = std::chrono::high_resolution_clock::now();
		window.clear(BG_COLOR);

		// CHANGE buffer, then
		for (int x = 0; x < SC_Width; x++)
			for (int y = 0; y < SC_Height; y++)
			{
				if (pow(x - SP_X_CENTRE + SP_RAD, 2) + pow(y - SP_Y_CENTRE + SP_RAD, 2) > pow(SP_RAD, 2))
					continue;

				Vector color = get_true_color(x, y, ALL_VECTORS);
				//Color color();
				
				buffer.setPixel(x, y, Color(color.x_, color.y_, color.z_));//Color(randomgen(0, 255), randomgen(0, 255), randomgen(0, 255)));
			}
		texture.loadFromImage(buffer);
		window.draw(sprite);

		window.display();
		end = std::chrono::high_resolution_clock::now();

		fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		
		const std::string title = "Shooter simulator (" + std::to_string((int)fps) + " fps)";

		window.setTitle(title);
	
	}	

	return 0;

}
