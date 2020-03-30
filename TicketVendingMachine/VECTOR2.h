#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();

	// “› ﬁïœêî
	int x;
	int y;

	// ë„ì¸ââéZéq
	VECTOR2 &operator = (const VECTOR2& vec);
	
	// ìYÇ¶éöââéZéq
	int &operator [] (int i);

	// î‰ärââéZéq
	bool operator == (const VECTOR2& vec)const;
	bool operator != (const VECTOR2& vec)const;
	bool operator <	 (const VECTOR2& vec)const;
	bool operator <= (const VECTOR2& vec)const;
	bool operator >	 (const VECTOR2& vec)const;
	bool operator >= (const VECTOR2& vec)const;


	// íPçÄââéZéq
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2 operator + () const;
	VECTOR2 operator - () const;

};

// Õﬁ∏ƒŸÇÃââéZ
// VECTOR2 + int 
VECTOR2 operator + (const VECTOR2 & u, int k);
// VECTOR2 + VECTOR2
VECTOR2 operator + (const VECTOR2 & u, const VECTOR2 & v);
// VECTOR2 - int
VECTOR2 operator - (const VECTOR2 & u, int k);
// VECTOR2 - VECTOR2
VECTOR2 operator - (const VECTOR2 & u, const VECTOR2& v);
// int * VECTOR2
VECTOR2 operator * (int k, const VECTOR2 & v);
// VECTOR2 * int 
VECTOR2 operator * (const VECTOR2 & v, int k);
// VECTOR2 * VECTOR2
VECTOR2 operator * (const VECTOR2 & u, const VECTOR2& v);
// VECTOR2 / int
VECTOR2 operator / (const VECTOR2 & v, int k);
// VECTOR2 / VECTOR2
VECTOR2 operator / (const VECTOR2 & u, const VECTOR2& v);
// VECTOR2 % int
VECTOR2 operator % (const VECTOR2 & v, int k);
// ì‡êœ
int dot (const VECTOR2 & u, const VECTOR2 & v);
// äOêœ
int cross (const VECTOR2 & u, const VECTOR2 & v);

