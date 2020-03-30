#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();

	// ���ޕϐ�
	int x;
	int y;

	// ������Z�q
	VECTOR2 &operator = (const VECTOR2& vec);
	
	// �Y�������Z�q
	int &operator [] (int i);

	// ��r���Z�q
	bool operator == (const VECTOR2& vec)const;
	bool operator != (const VECTOR2& vec)const;
	bool operator <	 (const VECTOR2& vec)const;
	bool operator <= (const VECTOR2& vec)const;
	bool operator >	 (const VECTOR2& vec)const;
	bool operator >= (const VECTOR2& vec)const;


	// �P�����Z�q
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2 operator + () const;
	VECTOR2 operator - () const;

};

// �޸�ق̉��Z
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
// ����
int dot (const VECTOR2 & u, const VECTOR2 & v);
// �O��
int cross (const VECTOR2 & u, const VECTOR2 & v);

