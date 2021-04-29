#include "vector.h"
#include "draw.h"
#include "Button.h"

using namespace sf;

const int SPEED = 100;

bool MOVE = true;
bool NEED_MORGEN = false;

int AUTO_SPEED = 1;
int SMOOTH = 10;


#define SC_Width 640
#define SC_Height 480
#define BG_COLOR Color::Blue

#define SP_RAD 100.f
#define SP_X_CENTRE SC_Width / 2 - SP_RAD
#define SP_Y_CENTRE SC_Height / 2 - SP_RAD
#define SP_COLOR Color::Red

// ���� ��������� �����
#define MATER_COLOR 0.96, 0.938, 0.03

// ���� ���������
#define LIGHT_COLOR 0.2, 0.2, 0.6

// ���������� ����
#define AMBIENT_COLOR 0.0, 0.3, 0.3

#define AMBIENT_POSITION -2 * SP_RAD, -SP_RAD, 0

#define ALL_VECTORS viewPos, materialColor, lightColor, ambientColor

constexpr auto ABSORPTION = 2;
constexpr auto POW = 25;

void RayCasting()
{
	RenderWindow window(VideoMode(SC_Width, SC_Height), "");

	window.clear(BG_COLOR);

	// orwindow.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	CircleShape sphere(SP_RAD);
	sphere.setFillColor(SP_COLOR);
	sphere.setPosition(SP_X_CENTRE, SP_Y_CENTRE);


	Vector viewPos(0, 0, (double)(+5) * (double)SP_RAD);

	Vector materialColor(MATER_COLOR);
	const Vector lightColor(LIGHT_COLOR);
	Vector ambientColor(AMBIENT_COLOR);
	const Vector ambientPosition(AMBIENT_POSITION);

	Button movement(100, 20, "MOVE OFF");
	movement.setFillRacktengelColor(155, 40, 50);
	movement.setFillTextColor(20, 255, 155);
	movement.setPosition(10, 10);
	movement.SetTxtPos(100, 100);

	Button smooth(100, 20, "SMOOTH OFF");
	smooth.setFillRacktengelColor(155, 40, 50);
	smooth.setFillTextColor(20, 255, 155);
	smooth.setPosition(10, 45);
	smooth.SetTxtPos(100, 100);

	Button ambient(100, 20, "AMBIENT OFF");
	ambient.setFillRacktengelColor(155, 40, 50);
	ambient.setFillTextColor(20, 255, 155);
	ambient.setPosition(10, 80);
	ambient.SetTxtPos(100, 100);

	Button inc_speed(20, 20, "  +");
	inc_speed.setFillRacktengelColor(155, 40, 50);
	inc_speed.setFillTextColor(20, 255, 0);
	inc_speed.setPosition(55, 115);
	inc_speed.SetTxtPos(100, 100);

	Button red_speed(20, 20, "  -");
	red_speed.setFillRacktengelColor(155, 40, 50);
	red_speed.setFillTextColor(20, 255, 0);
	red_speed.setPosition(20, 115);
	red_speed.SetTxtPos(100, 100);

	Button morgen(100, 20, "MORGEN ON");
	morgen.setFillRacktengelColor(155, 40, 50);
	morgen.setFillTextColor(20, 255, 155);
	morgen.setPosition(500, 80);
	morgen.SetTxtPos(100, 100);


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
	std::chrono::high_resolution_clock::time_point last_click;

	float fps;

	double pi = std::acos(-1);
	double light_move = pi / 2;

	Vector2i mouse = Mouse::getPosition(window);
	Vector2f mousePositon = window.mapPixelToCoords(mouse);//��������� �� � ������� (������ �� ����� ����)

	Vector lightPosition;
	Vector color;


	SoundBuffer SoundBuf;
	SoundBuf.loadFromFile("MORGENSHTERN - ������.ogg");// ��� ��������� � ����� ��� ��
	Sound sound;
	sound.setBuffer(SoundBuf);
	sound.play();

	sf::Image Sphere;
	sf::Color matColor;

	if (!Sphere.loadFromFile("morgen.jpg"))
	{
		printf("Can't open file sphere.jpg\n");
	}

	while (window.isOpen())
	{
		mouse = Mouse::getPosition(window);
		mousePositon = window.mapPixelToCoords(mouse);//��������� �� � ������� (������ �� ����� ����)

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
					if (-viewPos.x_ + SPEED > SC_Width + 50)
						viewPos.x_ = SC_Width - (-viewPos.x_ + SPEED - SC_Width);
					else
						viewPos.x_ -= SPEED;
					break;
				case Keyboard::Right:
					if (viewPos.x_ + SPEED > SC_Width + 50)
						viewPos.x_ = -SC_Width + (viewPos.x_ + SPEED - SC_Width);
					else
						viewPos.x_ += SPEED;
					break;
				case Keyboard::Down:
					if (viewPos.y_ + SPEED > SC_Height + 50)
						viewPos.y_ = -SC_Height + (viewPos.y_ + SPEED - SC_Width);
					else
						viewPos.y_ += SPEED;
					break;
				case Keyboard::Up:
					if (-viewPos.y_ + SPEED > SC_Height + 50)
						viewPos.y_ = SC_Height - (-viewPos.y_ + SPEED - SC_Width);
					else
						viewPos.y_ -= SPEED;
					break;
				}
			}

		}

		if (movement.navediaMouse(event, mousePositon))
		{
			movement.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if (MOVE)
				{
					MOVE = false;
					movement.changeTxt("MOVE ON");
					sound.pause();
				}
				else
				{
					MOVE = true;
					movement.changeTxt("MOVE OFF");
					sound.play();
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			movement.changeFillRacktengelColor(movement.GetColor());

		if (smooth.navediaMouse(event, mousePositon))
		{
			smooth.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if (SMOOTH == 10)
				{
					SMOOTH = 0;
					smooth.changeTxt("SMOOTH ON");
				}
				else
				{
					SMOOTH = 10;
					smooth.changeTxt("SMOOTH OFF");
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			smooth.changeFillRacktengelColor(smooth.GetColor());

		if (ambient.navediaMouse(event, mousePositon))
		{
			ambient.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if ((ambientColor.x_ == 0) && (ambientColor.y_ == 0) && (ambientColor.y_ == 0))
				{
					ambientColor.set(AMBIENT_COLOR);
					ambient.changeTxt("AMBIENT OFF");
				}
				else
				{
					ambientColor.set(0, 0, 0);
					ambient.changeTxt("AMBIENT ON");
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			ambient.changeFillRacktengelColor(ambient.GetColor());

		if (inc_speed.navediaMouse(event, mousePositon))
		{
			inc_speed.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if (AUTO_SPEED < 20)
				{
					AUTO_SPEED++;
					if (AUTO_SPEED == 20) inc_speed.setFillTextColor(255, 0, 0);
					red_speed.setFillTextColor(20, 255, 0);
				}
				else
				{
					inc_speed.setFillTextColor(255, 0, 0);
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			inc_speed.changeFillRacktengelColor(inc_speed.GetColor());

		if (red_speed.navediaMouse(event, mousePositon))
		{
			red_speed.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if (AUTO_SPEED > 0)
				{
					AUTO_SPEED--;
					if (AUTO_SPEED == 0) red_speed.setFillTextColor(255, 0, 0);
					inc_speed.setFillTextColor(20, 255, 0);
				}
				else
				{
					red_speed.setFillTextColor(255, 0, 0);
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			red_speed.changeFillRacktengelColor(red_speed.GetColor());

		if (morgen.navediaMouse(event, mousePositon))
		{
			morgen.changeFillRacktengelColor(255, 255, 0);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				end = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(end - last_click).count()) < 70));
				else if (NEED_MORGEN == true)
				{
					NEED_MORGEN = false;
					morgen.changeTxt("MORGEN OFF");
				}
				else
				{
					NEED_MORGEN = true;
					morgen.changeTxt("MORGEN ON");
				}

				last_click = std::chrono::high_resolution_clock::now();
			}

		}
		else
			morgen.changeFillRacktengelColor(red_speed.GetColor());



		if (movement.pressed(event, mousePositon))
		{
			//	cout << "PRESSED" << endl;
		}

		start = std::chrono::high_resolution_clock::now();
		window.clear(BG_COLOR);

		// CHANGE buffer, then
		

		for (int x = 0; x < SC_Width; x++)
			for (int y = 0; y < SC_Height; y++)
			{
				if (pow(x - SC_Width / 2, 2) + pow(y - SC_Height / 2, 2) > pow(SP_RAD, 2))
					continue;

				if (NEED_MORGEN)
				{
					matColor = Sphere.getPixel(x, y);
					materialColor = Vector(matColor.r / 256.f, matColor.g / 256.f, matColor.b / 256.f);
				}

				lightPosition = Vector(SP_RAD * cos(light_move), SP_RAD * pow(cos(light_move), 2), SP_RAD * sin(light_move));
				color = get_true_color(x - SC_Width / 2, y - SC_Height / 2, ALL_VECTORS,
									   lightPosition, SP_RAD, SMOOTH, ambientPosition
										);
				buffer.setPixel(x, y, Color(255 * color.x_, 255 * color.y_, 255 * color.z_));
			}
		

		texture.loadFromImage(buffer);
		window.draw(sprite);

		movement.draw(window);
		smooth.draw(window);
		ambient.draw(window);
		inc_speed.draw(window);
		red_speed.draw(window);
		morgen.draw(window);


		window.display();
		end = std::chrono::high_resolution_clock::now();

		fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		const std::string title = "Raycasting (" + std::to_string((double)fps) + " fps)";

		window.setTitle(title);

		if (MOVE)
			light_move += AUTO_SPEED / 10.f;
	}
}

int randomgen(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor,
						const Vector lightPosition, int R, int smooth, const Vector ambientPosition)
{
	Vector p(x, y, 0);
	p = p + collision(p, R, smooth);
	
	if (pow(p.x_, 2) + pow(p.y_, 2)  > pow(R, 2))
		return Vector(0, 0, 0);

	p.z_ = sqrt(pow(R, 2) - pow(p.x_, 2) - pow(p.y_, 2));

	// N -- ��������� ������ ������� � �����
	Vector N = p.normalize() + collision(p, R, smooth) / 200;
	

	// V -- ��������� ������ ����������� �� ������ (�����������)
	Vector V = (viewPos - p).normalize();
	
	// L -- ��������� ������ ����������� �� �������� �����
	Vector L = (lightPosition - p).normalize();
	

	double diffuse = N.dot(L);
	if (diffuse < 0) diffuse = 0;

	// Lr -- ��� ��� (Lr + L) = 2N.dot(L) * N (�.�. ������, ������������ N, � �� ����� ������ 2N.dot(L) ) 
	Vector Lr = ABSORPTION * (N.dot(L)) * N - L;
	
	
	// ���� ����� Lr � V
	double spec = Lr.dot(V);
	if (spec < 0) spec = 0;
	double specular = pow(spec, POW); // 25


	Vector color = ambientColor * materialColor * (randomgen(-1, 1) / 10);
	color += diffuse * materialColor * lightColor;
	color += specular * lightColor;
	color += AmbientLightning( p, N, V, ambientPosition,
		ambientColor, materialColor);
	    
	return color;
}

Vector AmbientLightning(const Vector& p, const Vector& N, const Vector& V,
					  const Vector& ambientPosition, const Vector& lightColor,
					  const Vector& materialColor)
{
	Vector L = (ambientPosition - p).normalize();

	double diffuse = sc_dot(N, L);
	if (diffuse < 0) return Vector(0, 0, 0);

	Vector Lr = 2 * diffuse * N - L;
	double specular = pow(Lr.dot(V), POW);

	return diffuse * lightColor * materialColor +
		   specular * lightColor;
}

Vector collision(const Vector& p, double r, int smooth)
{
	Vector bump(0, 0, 0);
	const int sz = 100;

	static Vector env[sz][sz] = {};

	static bool initialize = false;

	if (smooth == 10)
		return Vector(0, 0, 0);
	else	
	{
		#define GET_RAND() randomgen( -1, 1 )
		#define GET_SMOOTH() randomgen(-smooth, smooth)
		#define RAND() (GET_RAND() * 10 * (10 - GET_SMOOTH())) / 100
		#define RAND_SMOOTH() randomgen( -1, 1 )
		
		if (!initialize)
		{
			for (int y = 0; y < sz; y++) for (int x = 0; x < sz; x++)
				env[y][x] = Vector(RAND(), RAND(), RAND());

			initialize = true;
		}


	#undef GET_RAND()
	#undef GET_SMOOTH()
	#undef RAND()
	#undef RAND_SMOOTH()
	}
		
	bump += env[(unsigned)(int)(p.y_ + r) % sz][(unsigned)(int)(p.x_ + r) % sz];

	return bump;
}
