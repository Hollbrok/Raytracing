#include "vector.h"

int main()
{

	Vector vec1(1, 1, 1);
	Vector vec2(23, -1, 0);


	Vector vec3 = vec1 ^ vec2;
	Vector vec4 = vec1 - vec2;

	PRINT_VEC(vec3);
	PRINT_VEC(vec4);
	vec1.print();
	vec1 += vec2;
	vec1.print();

	return 0;
}