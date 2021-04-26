#include "vector.h"
#include "draw.h"

using namespace sf;

const int SPEED = 50;
const bool MOVE = true;
int SMOOTH = 10;


#define SC_Width 640
#define SC_Height 480
#define BG_COLOR Color::Blue

#define SP_RAD 100.f
#define SP_X_CENTRE SC_Width / 2 - SP_RAD
#define SP_Y_CENTRE SC_Height / 2 - SP_RAD
#define SP_COLOR Color::Red

// ÷вет материала сферы
#define MATER_COLOR 0.1, 0.1, 0.1

// ÷вет источника
#define LIGHT_COLOR 0.3, 0.8 , 0.3

// ќкружающий цвет
#define AMBIENT_COLOR 1.0, 0.2, 0.3

#define ALL_VECTORS viewPos, materialColor, lightColor, ambientColor

int main()
{

	RenderWindow window(VideoMode(SC_Width, SC_Height), "");
	
	window.clear(BG_COLOR);
	
	// orwindow.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	CircleShape sphere(SP_RAD);
	sphere.setFillColor(SP_COLOR);
	sphere.setPosition(SP_X_CENTRE, SP_Y_CENTRE);


	Vector viewPos(0, 0, (double)(+5) * (double)SP_RAD);

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

	double pi = std::acos(-1);
	double light_move = pi / 2;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			
			if (Keyboard::isKeyPressed)
			{

				switch (event.key.code)
				{
					case Keyboard::Left:
						if (-viewPos.x_ + SPEED > SC_Width)
							viewPos.x_ = SC_Width - (-viewPos.x_ + SPEED - SC_Width);
						else 
							viewPos.x_ -= SPEED;
						break;
					case Keyboard::Right:
						if (viewPos.x_ + SPEED > SC_Width)
							viewPos.x_ = -SC_Width + (viewPos.x_ + SPEED - SC_Width);
						else
							viewPos.x_ += SPEED;
						break;
					case Keyboard::Down:
						if (viewPos.y_ + SPEED > SC_Width)
							viewPos.y_ = -SC_Width + (viewPos.y_ + SPEED - SC_Width);
						else
							viewPos.y_ += SPEED;
						break;
					case Keyboard::Up:
						if (-viewPos.y_ + SPEED > SC_Width)
							viewPos.y_ = SC_Width - (-viewPos.y_ + SPEED - SC_Width);
						else
							viewPos.y_ -= SPEED;
						break;
				}			
				//PRINT_VEC(viewPos);
			}
		
		}
		start = std::chrono::high_resolution_clock::now();
		window.clear(BG_COLOR);

		// CHANGE buffer, then
		for (int x = 0; x < SC_Width; x++)
			for (int y = 0; y < SC_Height; y++)
			{
				if (pow(x - SC_Width / 2, 2) + pow(y - SC_Height / 2, 2) > pow(SP_RAD, 2))
					continue;

				Vector lightPosition = Vector(SP_RAD * cos(light_move), /*SP_RAD * pow(cos(light_move), 2)*/ 0, SP_RAD * sin(light_move));
				Vector color = get_true_color(x - SC_Width / 2, y - SC_Height / 2, ALL_VECTORS, 
											  lightPosition, SP_RAD, SMOOTH
											);
					/*Vector( - 2 * SP_RAD * (1 + cos(light_move)),
																		 - 2 * SP_RAD * cos(light_move),
																		 + 2 * SP_RAD * sin(light_move) )*/
				buffer.setPixel(x, y, Color(255 * color.x_, 255 * color.y_, 255 * color.z_));
			}

		texture.loadFromImage(buffer);
		window.draw(sprite);

		window.display();
		end = std::chrono::high_resolution_clock::now();

		fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		
		const std::string title = "Raycasting (" + std::to_string((int)fps) + " fps)";

		window.setTitle(title);

		if(MOVE)
			light_move += 0.1;
	}	

	return 0;

}
