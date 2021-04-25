#include "vector.h"
#include "draw.h"

int randomgen(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor, const Vector lightPosition, int R)
{
	//printf("x = %d, y = %d\n", x, y);
	Vector p(x, y, 0);
	p = p + Bump(p, R) * 100;
	//PRINT_VEC(p);
	if (p.x_ * p.x_ + p.y_ * p.y_ > R * R)
		printf("BADDDDDD\n"); //{ DrawPixel(x, y, Vector()); continue; }
	//printf("R^2 - ... = %lg\n", R * R - p.x_ * p.x_ - p.y_ * p.y_);
	p.z_ = sqrt(R * R - p.x_ * p.x_ - p.y_ * p.y_);
	//PRINT_VEC(p);
	
	Vector N = p.normalize() + Bump(p, R);
	//PRINT_VEC(N);


	Vector V = (viewPos - p).normalize();
	//PRINT_VEC(V);
	
	Vector L = (lightPosition - p).normalize();
	//PRINT_VEC(L);

	double diffuse = N.dot(L);
	if (diffuse < 0) diffuse = 0;

	Vector Lr = 2 * (N.dot(L)) * N - L;
	//PRINT_VEC(Lr);
	
	double spec = Lr.dot(V);
	if (spec < 0) spec = 0;
	double specular = pow(spec, 25);


	Vector color = ambientColor * materialColor;
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

Vector PhongLightning(const Vector& p, const Vector& N, const Vector& V,
					  const Vector& lightPos, const Vector& lightColor,
					  const Vector& materialColor, const Vector& ambientColor)
{
	Vector L = (lightPos - p).normalize();

	double diffuse = sc_dot(N, L); //N.dot(L);
	if (diffuse < 0) return ambientColor;

	Vector Lr = 2 * diffuse * N - L;
	double specular = pow(Lr.dot(V), 25);

	return diffuse * lightColor * materialColor +
		specular * lightColor + ambientColor;
}

Vector Bump(const Vector& p, double r)
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

	return Vector(0, 0, 0);
}
