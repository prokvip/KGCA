#include "TVector.h"
float   TVector2::operator | (TVector2 p)
{
	return x * p.x + y * p.y;
}
float   TVector2::DotProduct(TVector2 a, TVector2 b)
{
	return a.x * b.x + a.y * b.y;
}
float    TVector2::Length()
{
	float ret =  x * x + y * y;	
	return sqrt(ret);
}
TVector2 TVector2::Normalize()
{
	TVector2 d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	return d;
}
float   TVector2::ProjectionPoint(
	TVector2 q1, TVector2 q2, TVector2 v)
{
	TVector2 a = q2 - q1;
	TVector2 b = v - q1;
	TVector2 p = a * ( (b | a) / (a | a));
	TVector2 w = b - p;
	return w.Length();
}