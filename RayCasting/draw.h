#pragma once

#include "Libraries.h"

int randomgen(int min, int max);

Vector get_true_color(int x, int y, const Vector viewPos, const Vector materialColor, const Vector lightColor, const Vector ambientColor, const Vector lightPosition, int R, int smooth);

Vector PhongLightning(const Vector& p, const Vector& N, const Vector& V,
	const Vector& lightPos, const Vector& lightColor,
	const Vector& materialColor,
	const Vector& ambientColor);

Vector Bump(const Vector& p, double r, int smooth);