#include "XVector.h"
float   XVector2::operator | (XVector2 p)
{
	return x * p.x + y * p.y;
}
float   XVector2::DotProduct(XVector2 a, XVector2 b)
{
	return a.x * b.x + a.y * b.y;
}
float    XVector2::Length()
{
	float ret =  x * x + y * y;	
	return sqrt(ret);
}
XVector2 XVector2::Normalize()
{
	XVector2 d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	return d;
}
float   XVector2::ProjectionPoint(
	XVector2 q1, XVector2 q2, XVector2 v)
{
	XVector2 a = q2 - q1;
	XVector2 b = v - q1;
	XVector2 p = a * ( (b | a) / (a | a));
	XVector2 w = b - p;
	return w.Length();
}