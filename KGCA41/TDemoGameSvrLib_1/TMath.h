#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>//std::setprecision
using namespace std;

/** Returns highest of 3 values */
template< class T >
static  T Max3(const T A, const T B, const T C)
{
	return Max(Max(A, B), C);
}

/** Returns lowest of 3 values */
template< class T >
static T Min3(const T A, const T B, const T C)
{
	return Min(Min(A, B), C);
}

/** Multiples value by itself */
template< class T >
static T Square(const T A)
{
	return A * A;
}

/** Clamps X to be between Min and Max, inclusive */
template< class T >
static T Clamp(const T X, const T Min, const T Max)
{
	return X < Min ? Min : X < Max ? X : Max;
}

struct FVector
{
	double X, Y, Z;
	/** A zero vector (0,0,0) */
	static const FVector ZeroVector;
	/** One vector (1,1,1) */
	static const FVector OneVector;
	/** World up vector (0,0,1) */
	static const FVector UpVector;
	/** Unreal forward vector (1,0,0) */
	static const FVector ForwardVector;
	/** Unreal right vector (0,1,0) */
	static const FVector RightVector;
	FVector operator^(const FVector& V) const;
	FVector CrossProduct(const FVector& A, const FVector& B);
	double operator|(const FVector& V) const;
	double DotProduct(const FVector& A, const FVector& B);
	FVector operator+(const FVector& V) const;
	FVector operator-(const FVector& V) const;
	FVector operator-(double Bias) const;
	FVector operator+(double Bias) const;
	FVector operator*(double Scale) const;
	FVector operator/(double Scale) const;
	FVector operator*(const FVector& V) const;
	FVector operator/(const FVector& V) const;
	bool operator==(const FVector& V) const;
	bool operator!=(const FVector& V) const;
	bool Equals(const FVector& V, double Tolerance) const;
	bool AllComponentsEqual(double Tolerance) const;
	FVector operator-() const;
	FVector operator+=(const FVector& V);
	FVector operator-=(const FVector& V);
	FVector operator*=(double Scale);
	FVector operator/=(double V);
	FVector operator*=(const FVector& V);
	FVector operator/=(const FVector& V);
	double& operator[](int Index);
	double operator[](int Index)const;
	void Set(double InX, double InY, double InZ);
	double Size() const;
	double SizeSquared() const;
	bool IsZero() const;
	bool Normalize(double Tolerance);
	double Dist(const FVector &V1, const FVector &V2);
	double DistanceTo(FVector vDesk);
	double DistSquared(const FVector &V1, const FVector &V2);
	FVector() { X = Y = Z = 0.0f; };
	FVector(double InX, double InY, double InZ) { X = InX;  Y = InY;  Z = InZ; };
};
struct FRotator
{
	double X, Y, Z;
	static const FRotator ZeroRotator;
	FRotator() { X = Y = Z = 0.0f; };
	FRotator(double InX, double InY, double InZ) { X = InX;  Y = InY;  Z = InZ; };
};



static ostream& operator<<(ostream &stream, const FVector& vector)
{
	stream.write(reinterpret_cast<const char*>(&vector), sizeof(FVector));
	return stream;
}
static ostream& operator<<(ostream &stream, const FRotator& rot)
{
	stream.write(reinterpret_cast<const char*>(&rot), sizeof(FRotator));
	return stream;
}
static istream& operator>>(istream &stream, FVector& vector)
{
	stream.read(reinterpret_cast<char*>(&vector), sizeof(FVector));
	return stream;
}
static istream& operator>>(istream &stream, FRotator& rot)
{
	stream.read(reinterpret_cast<char*>(&rot), sizeof(FRotator));
	return stream;
}