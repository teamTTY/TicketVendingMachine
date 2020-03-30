#include <cmath>
#include "VECTOR2.h"

VECTOR2::VECTOR2()
{
	x = 0;
	y = 0;
}

VECTOR2::VECTOR2(int x, int y)
{
	this->x = x;
	this->y = y;
}


VECTOR2::~VECTOR2()
{
}

// ë„ì¸ââéZéq
VECTOR2 & VECTOR2::operator=(const VECTOR2 & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;
}

// ìYÇ¶éöââéZéq
int & VECTOR2::operator[](int i)
{
	if (i == 0)
	{
		return x;
	}
	else if (i == 1)
	{
		return y;
	}
	else
	{
		return x;
	}
}

// î‰ärââéZéq
bool VECTOR2::operator==(const VECTOR2 & vec) const
{
	return (this->x == vec.x) && (this->y == vec.y);
}

bool VECTOR2::operator!=(const VECTOR2 & vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}

bool VECTOR2::operator<(const VECTOR2 & vec) const
{
	return (this->x < vec.x) && (this->y < vec.y);
}

bool VECTOR2::operator<=(const VECTOR2 & vec) const
{
	return (this->x <= vec.x) && (this->y <= vec.y);
}

bool VECTOR2::operator>(const VECTOR2 & vec) const
{
	return (this->x > vec.x) && (this->y > vec.y);
}

bool VECTOR2::operator>=(const VECTOR2 & vec) const
{
	return (this->x >= vec.x) && (this->y >= vec.y);
}

// íPçÄââéZéq
VECTOR2 & VECTOR2::operator+=(const VECTOR2 & vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

VECTOR2 & VECTOR2::operator-=(const VECTOR2 & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

VECTOR2 & VECTOR2::operator*=(int k)
{
	this->x *= k;
	this->y *= k;
	return *this;
}

VECTOR2 & VECTOR2::operator/=(int k)
{
	this->x /= k;
	this->y /= k;
	return *this;
}

VECTOR2 VECTOR2::operator+() const
{
	return *this;
}

VECTOR2 VECTOR2::operator-() const
{
	return VECTOR2(-this->x,-this->y);
}

// Õﬁ∏ƒŸÇÃââéZ
// VECTOR2 + int 
VECTOR2 operator+(const VECTOR2 & u, int k)
{
	return VECTOR2(u.x + 5, u.y + 5);
}

// VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2 & u, const VECTOR2 & v)
{
	return VECTOR2(u.x + v.x, u.y + v.y);
}

// VECTOR2 - int
VECTOR2 operator-(const VECTOR2 & u, int k)
{
	return VECTOR2(u.x - k, u.y - k);
}

// VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2 & u, const VECTOR2 & v)
{
	return VECTOR2(u.x - v.x, u.y - v.y);
}

// int * VECTOR2
VECTOR2 operator*(int k, const VECTOR2 & v)
{
	return VECTOR2(k * v.x, k * v.y);
}

// VECTOR2 * int 
VECTOR2 operator*(const VECTOR2 & v, int k)
{
	return VECTOR2(v.x * k, v.y * k);
}

// VECTOR2 * VECTOR2
VECTOR2 operator*(const VECTOR2 & u, const VECTOR2 & v)
{
	return VECTOR2(u.x * v.x, u.y * v.y);
}

// VECTOR2 / int
VECTOR2 operator/(const VECTOR2 & v, int k)
{
	return VECTOR2(v.x / k, v.y / k);
}

// VECTOR2 / VECTOR2
VECTOR2 operator/(const VECTOR2 & u, const VECTOR2 & v)
{
	return VECTOR2(u.x / v.x, u.y / v.y);
}

// VECTOR2 % int
VECTOR2 operator%(const VECTOR2 & v, int k)
{
	return VECTOR2(v.x % k, v.y % k);
}

// ì‡êœ
int dot (const VECTOR2 & u, const VECTOR2 & v)
{
	return u.x * v.x + u.y * v.y;
}
// äOêœ
int cross(const VECTOR2 & u, const VECTOR2 & v)
{
	return u.x * v.y - u.y * v.x;
}
