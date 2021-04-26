#include "vector.h"
#include "draw.h"

constexpr auto ABSORPTION = 2;
constexpr auto POW = 35;

int randomgen(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor,
						const Vector lightPosition, int R, int smooth)
{
	Vector p(x, y, 0);
	p = p + Bump(p, R, smooth) * 100;
	
	if (p.x_ * p.x_ + p.y_ * p.y_ > R * R)
		return Vector(0, 0, 0);

	p.z_ = sqrt(R * R - p.x_ * p.x_ - p.y_ * p.y_);


	// N -- единичные вектор нормали в точке
	Vector N = p.normalize() + Bump(p, R, smooth);
	

	// V -- единичный вектор направления на камеру (наблюдателя)
	Vector V = (viewPos - p).normalize();
	
	// L -- единичный вектор направления на источник света
	Vector L = (lightPosition - p).normalize();
	

	double diffuse = N.dot(L);
	if (diffuse < 0) diffuse = 0;

	// Lr -- Так как (Lr + L) = 2N.dot(L) * N (т.к. вектор, коллинеарный N, а по длине равный 2N.dot(L) ) 
	Vector Lr = ABSORPTION * (N.dot(L)) * N - L;
	
	
	// угол между Lr и V
	double spec = Lr.dot(V);
	if (spec < 0) spec = 0;
	double specular = pow(spec, POW); // 25


	Vector color = ambientColor * materialColor * (randomgen(-1, 1) / 10);//(sc_dot(materialColor, Vector(1000,1000 * R,0)));
	//PRINT_VEC(color);

	color += diffuse * materialColor * lightColor;
	//PRINT_VEC(color);

	color += specular * lightColor;
	//PRINT_VEC(color);

	color += PhongLightning( p, N, V, Vector(+2 * R, -1 * R, 0), Vector(0.5, 0.5, 0.5), materialColor, Vector(0, 0, 0));
	
	//PRINT_VEC(color);
    
	return color;

    //return Vector(randomgen(0, 255), randomgen(0, 255), randomgen(0, 255));
}

Vector PhongLightning(const Vector& p,				const Vector& N,		 const Vector& V,
					  const Vector& lightPos,		const Vector& lightColor,
					  const Vector& materialColor,	const Vector& ambientColor)
{
	Vector L = (lightPos - p).normalize();

	double diffuse = sc_dot(N, L); //N.dot(L);
	if (diffuse < 0) return ambientColor;

	Vector Lr = 2 * diffuse * N - L;
	double specular = pow(Lr.dot(V), POW);

	return diffuse * lightColor * materialColor +
		specular * lightColor + ambientColor;
}

Vector Bump(const Vector& p, double r, int smooth)
{
	/*bool bXY	= (GetAsyncKeyState('X') != 0),
		 bXY2	= (GetAsyncKeyState('H') != 0),
		 bZ		= (GetAsyncKeyState('Z') != 0),

	if (!bXY && !bXY2 && !bZ) return Vector();

	const int sz = 100;
	static Vector env[sz][sz] = {};

	static bool init = false;

	if (!init)
	{
		for (int y = 0; y < sz; y++) for (int x = 0; x < sz; x++)
			env[y][x] = Vector(Random(-1, +1), Random(-1, +1), Random(-1, +1));

		init = true;
	}

	Vector bump(0, 0, 0);

	if (bXY)  bump += p * ((sin(p.x_ / 4) + cos(p.y_ / 4)) / 4 + 1.5) / 5000.0;
	if (bXY2) bump += p * ((cos(p.x_ / 4) + sin(p.y_ / 4)) / 2 + 1.0) / 3000.0;
	if (bZ)   bump += p * (sin(p.z_ * p.z_ / 500) / 2 + 1.5) / 5000.0;
	if (bRnd) bump += env[(unsigned)ROUND(p.y_ + r) % sz][(unsigned)ROUND(p.x_ + r) % sz] / 100.0;

	return bump;*/

	Vector bump(0, 0, 0);
	const int sz = 100;
	static Vector env[sz][sz] = {};

	static bool init = false;
//#define GET_RAND() randomgen( -1, 1 )


	//int smooth = 9; // from 0 to 10

	if (smooth == 10)
		return Vector(0, 0, 0);
	else	
	{
		#define GET_RAND() randomgen( -1, 1 )
		#define GET_SMOOTH() randomgen(-smooth, smooth)
		#define RAND() (GET_RAND() * 10 * (10 - GET_SMOOTH())) / 100
		#define RAND_SMOOTH() randomgen( -1, 1 )
		
		if (!init)
		{
			for (int y = 0; y < sz; y++) for (int x = 0; x < sz; x++)
				env[y][x] = Vector(RAND(), RAND(), RAND());

			init = true;
		}
	}
		

	#undef GET_RAND()
	#undef GET_SMOOTH()
	#undef RAND()
	#undef RAND_SMOOTH()

	

	bump += env[(unsigned)(int)(p.y_ + r) % sz][(unsigned)(int)(p.x_ + r) % sz] / 100.0;



	return bump;

	return Vector(0, 0, 0);

}
