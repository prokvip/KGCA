#include "TMath.h"
//https://docs.microsoft.com/en-us/windows/win32/dxmath/pg-xnamath-migration-d3dx
namespace TBASIS_EX
{
		const TVector2 TVector2::Zero = { 0.f, 0.f };
		const TVector2 TVector2::One = { 1.f, 1.f };
		const TVector2 TVector2::UnitX = { 1.f, 0.f };
		const TVector2 TVector2::UnitY = { 0.f, 1.f };

		const TVector3 TVector3::Zero = { 0.f, 0.f, 0.f };
		const TVector3 TVector3::One = { 1.f, 1.f, 1.f };
		const TVector3 TVector3::UnitX = { 1.f, 0.f, 0.f };
		const TVector3 TVector3::UnitY = { 0.f, 1.f, 0.f };
		const TVector3 TVector3::UnitZ = { 0.f, 0.f, 1.f };
		const TVector3 TVector3::Up = { 0.f, 1.f, 0.f };
		const TVector3 TVector3::Down = { 0.f, -1.f, 0.f };
		const TVector3 TVector3::Right = { 1.f, 0.f, 0.f };
		const TVector3 TVector3::Left = { -1.f, 0.f, 0.f };
		const TVector3 TVector3::Forward = { 0.f, 0.f, -1.f };
		const TVector3 TVector3::Backward = { 0.f, 0.f, 1.f };

		const TVector4 TVector4::Zero = { 0.f, 0.f, 0.f, 0.f };
		const TVector4 TVector4::One = { 1.f, 1.f, 1.f, 1.f };
		const TVector4 TVector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
		const TVector4 TVector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
		const TVector4 TVector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
		const TVector4 TVector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

		const TMatrix TMatrix::Identity = { 1.f, 0.f, 0.f, 0.f,
										  0.f, 1.f, 0.f, 0.f,
										  0.f, 0.f, 1.f, 0.f,
										  0.f, 0.f, 0.f, 1.f };

		const TQuaternion TQuaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
	

	TVector2 TRectangle::Location() const
	{
		return TVector2(float(x), float(y));
	}

	TVector2 TRectangle::Center() const
	{
		return TVector2(float(x) + float(width / 2.f), float(y) + float(height / 2.f));
	}

	bool TRectangle::Contains(const TVector2& point) const
	{
		return (float(x) <= point.x) && (point.x < float(x + width)) && (float(y) <= point.y) && (point.y < float(y + height));
	}

	void TRectangle::Inflate(long horizAmount, long vertAmount)
	{
		x -= horizAmount;
		y -= vertAmount;
		width += horizAmount;
		height += vertAmount;
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	TRectangle TRectangle::Intersect(const TRectangle& ra, const TRectangle& rb)
	{
		long righta = ra.x + ra.width;
		long rightb = rb.x + rb.width;

		long bottoma = ra.y + ra.height;
		long bottomb = rb.y + rb.height;

		long maxX = ra.x > rb.x ? ra.x : rb.x;
		long maxY = ra.y > rb.y ? ra.y : rb.y;

		long minRight = righta < rightb ? righta : rightb;
		long minBottom = bottoma < bottomb ? bottoma : bottomb;

		TRectangle result;

		if ((minRight > maxX) && (minBottom > maxY))
		{
			result.x = maxX;
			result.y = maxY;
			result.width = minRight - maxX;
			result.height = minBottom - maxY;
		}
		else
		{
			result.x = 0;
			result.y = 0;
			result.width = 0;
			result.height = 0;
		}

		return result;
	}

	RECT TRectangle::Intersect(const RECT& rcta, const RECT& rctb)
	{
		long maxX = rcta.left > rctb.left ? rcta.left : rctb.left;
		long maxY = rcta.top > rctb.top ? rcta.top : rctb.top;

		long minRight = rcta.right < rctb.right ? rcta.right : rctb.right;
		long minBottom = rcta.bottom < rctb.bottom ? rcta.bottom : rctb.bottom;

		RECT result;

		if ((minRight > maxX) && (minBottom > maxY))
		{
			result.left = maxX;
			result.top = maxY;
			result.right = minRight;
			result.bottom = minBottom;
		}
		else
		{
			result.left = 0;
			result.top = 0;
			result.right = 0;
			result.bottom = 0;
		}

		return result;
	}

	TRectangle TRectangle::Union(const TRectangle& ra, const TRectangle& rb)
	{
		long righta = ra.x + ra.width;
		long rightb = rb.x + rb.width;

		long bottoma = ra.y + ra.height;
		long bottomb = rb.y + rb.height;

		int minX = ra.x < rb.x ? ra.x : rb.x;
		int minY = ra.y < rb.y ? ra.y : rb.y;

		int maxRight = righta > rightb ? righta : rightb;
		int maxBottom = bottoma > bottomb ? bottoma : bottomb;

		TRectangle result;
		result.x = minX;
		result.y = minY;
		result.width = maxRight - minX;
		result.height = maxBottom - minY;
		return result;
	}

	RECT TRectangle::Union(const RECT& rcta, const RECT& rctb)
	{
		RECT result;
		result.left = rcta.left < rctb.left ? rcta.left : rctb.left;
		result.top = rcta.top < rctb.top ? rcta.top : rctb.top;
		result.right = rcta.right > rctb.right ? rcta.right : rctb.right;
		result.bottom = rcta.bottom > rctb.bottom ? rcta.bottom : rctb.bottom;
		return result;
	}


	/****************************************************************************
	*
	* TVector2
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TVector2::operator == (const TVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		return XMVector2Equal(v1, v2);
	}

	bool TVector2::operator != (const TVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		return XMVector2NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TVector2& TVector2::operator+= (const TVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	TVector2& TVector2::operator-= (const TVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	TVector2& TVector2::operator*= (const TVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat2(this, X);
		return *this;
	}

	TVector2& TVector2::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat2(this, X);
		return *this;
	}

	TVector2& TVector2::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat2(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TVector2 operator+ (const TVector2& V1, const TVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	TVector2 operator- (const TVector2& V1, const TVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	TVector2 operator* (const TVector2& V1, const TVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	TVector2 operator* (const TVector2& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	TVector2 operator/ (const TVector2& V1, const TVector2& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V1);
		XMVECTOR v2 = XMLoadFloat2(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	TVector2 operator* (float S, const TVector2& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector2 R;
		XMStoreFloat2(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool TVector2::InBounds(const TVector2& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&Bounds);
		return XMVector2InBounds(v1, v2);
	}

	float TVector2::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Length(v1);
		return XMVectorGetX(X);
	}

	float TVector2::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2LengthSq(v1);
		return XMVectorGetX(X);
	}

	float TVector2::Dot(const TVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR X = XMVector2Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void TVector2::Cross(const TVector2& V, TVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR R = XMVector2Cross(v1, v2);
		XMStoreFloat2(&result, R);
	}

	TVector2 TVector2::Cross(const TVector2& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&V);
		XMVECTOR R = XMVector2Cross(v1, v2);

		TVector2 result;
		XMStoreFloat2(&result, R);
		return result;
	}

	void TVector2::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Normalize(v1);
		XMStoreFloat2(this, X);
	}

	void TVector2::Normalize(TVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR X = XMVector2Normalize(v1);
		XMStoreFloat2(&result, X);
	}

	void TVector2::Clamp(const TVector2& vmin, const TVector2& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&vmin);
		XMVECTOR v3 = XMLoadFloat2(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat2(this, X);
	}

	void TVector2::Clamp(const TVector2& vmin, const TVector2& vmax, TVector2& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(this);
		XMVECTOR v2 = XMLoadFloat2(&vmin);
		XMVECTOR v3 = XMLoadFloat2(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat2(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	float TVector2::Distance(const TVector2& v1, const TVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector2Length(V);
		return XMVectorGetX(X);
	}

	float TVector2::DistanceSquared(const TVector2& v1, const TVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector2LengthSq(V);
		return XMVectorGetX(X);
	}

	void TVector2::Min(const TVector2& v1, const TVector2& v2, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Min(const TVector2& v1, const TVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Max(const TVector2& v1, const TVector2& v2, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Max(const TVector2& v1, const TVector2& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Lerp(const TVector2& v1, const TVector2& v2, float t, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Lerp(const TVector2& v1, const TVector2& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::SmoothStep(const TVector2& v1, const TVector2& v2, float t, TVector2& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::SmoothStep(const TVector2& v1, const TVector2& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Barycentric(const TVector2& v1, const TVector2& v2, const TVector2& v3, float f, float g, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Barycentric(const TVector2& v1, const TVector2& v2, const TVector2& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::CatmullRom(const TVector2& v1, const TVector2& v2, const TVector2& v3, const TVector2& v4, float t, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR x4 = XMLoadFloat2(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::CatmullRom(const TVector2& v1, const TVector2& v2, const TVector2& v3, const TVector2& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&v2);
		XMVECTOR x3 = XMLoadFloat2(&v3);
		XMVECTOR x4 = XMLoadFloat2(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Hermite(const TVector2& v1, const TVector2& t1, const TVector2& v2, const TVector2& t2, float t, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&t1);
		XMVECTOR x3 = XMLoadFloat2(&v2);
		XMVECTOR x4 = XMLoadFloat2(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Hermite(const TVector2& v1, const TVector2& t1, const TVector2& v2, const TVector2& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat2(&v1);
		XMVECTOR x2 = XMLoadFloat2(&t1);
		XMVECTOR x3 = XMLoadFloat2(&v2);
		XMVECTOR x4 = XMLoadFloat2(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Reflect(const TVector2& ivec, const TVector2& nvec, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Reflect(i, n);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Reflect(const TVector2& ivec, const TVector2& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Reflect(i, n);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Refract(const TVector2& ivec, const TVector2& nvec, float refractionIndex, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Refract(const TVector2& ivec, const TVector2& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat2(&ivec);
		XMVECTOR n = XMLoadFloat2(&nvec);
		XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Transform(const TVector2& v, const TQuaternion& quat, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4((DirectX::XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Transform(const TVector2& v, const TQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4((DirectX::XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	void TVector2::Transform(const TVector2& v, const TMatrix& m, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4 *)&m);
		XMVECTOR X = XMVector2TransformCoord(v1, M);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::Transform(const TVector2& v, const TMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4 *)&m);
		XMVECTOR X = XMVector2TransformCoord(v1, M);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void TVector2::Transform(const TVector2* varray, size_t count, const TMatrix& m, TVector2* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4 *)&m);
		XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
	}

	void TVector2::Transform(const TVector2& v, const TMatrix& m, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4 *)&m);
		XMVECTOR X = XMVector2Transform(v1, M);
		XMStoreFloat4((DirectX::XMFLOAT4  *)&result, X);
	}

	_Use_decl_annotations_
		void TVector2::Transform(const TVector2* varray, size_t count, const TMatrix& m, TVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4  *)&m);
		XMVector2TransformStream((DirectX::XMFLOAT4*)resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
	}

	void TVector2::TransformNormal(const TVector2& v, const TMatrix& m, TVector2& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformNormal(v1, M);
		XMStoreFloat2(&result, X);
	}

	TVector2 TVector2::TransformNormal(const TVector2& v, const TMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector2TransformNormal(v1, M);

		TVector2 result;
		XMStoreFloat2(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void TVector2::TransformNormal(const TVector2* varray, size_t count, const TMatrix& m, TVector2* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((DirectX::XMFLOAT4X4*)&m);
		XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
	}


	/****************************************************************************
	*
	* TVector3
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool TVector3::operator == (const TVector3& V) const
	{
		DirectX::XMVECTOR v1 = XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = XMLoadFloat3(&V);
		return DirectX::XMVector3Equal(v1, v2);
	}

	bool TVector3::operator != (const TVector3& V) const
	{
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		return XMVector3NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TVector3& TVector3::operator+= (const TVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	TVector3& TVector3::operator-= (const TVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	TVector3& TVector3::operator*= (const TVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat3(this, X);
		return *this;
	}

	TVector3& TVector3::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat3(this, X);
		return *this;
	}

	TVector3& TVector3::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat3(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	TVector3 TVector3::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVectorNegate(v1);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TVector3 operator+ (const TVector3& V1, const TVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	TVector3 operator- (const TVector3& V1, const TVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	TVector3 operator* (const TVector3& V1, const TVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	TVector3 operator* (const TVector3& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	TVector3 operator/ (const TVector3& V1, const TVector3& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V1);
		XMVECTOR v2 = XMLoadFloat3(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	TVector3 operator* (float S, const TVector3& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector3 R;
		XMStoreFloat3(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool TVector3::InBounds(const TVector3& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&Bounds);
		return XMVector3InBounds(v1, v2);
	}

	float TVector3::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Length(v1);
		return XMVectorGetX(X);
	}

	float TVector3::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3LengthSq(v1);
		return XMVectorGetX(X);
	}

	float TVector3::Dot(const TVector3& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR X = XMVector3Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void TVector3::Cross(const TVector3& V, TVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR R = XMVector3Cross(v1, v2);
		XMStoreFloat3(&result, R);
	}

	TVector3 TVector3::Cross(const TVector3& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&V);
		XMVECTOR R = XMVector3Cross(v1, v2);

		TVector3 result;
		XMStoreFloat3(&result, R);
		return result;
	}

	void TVector3::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Normalize(v1);
		XMStoreFloat3(this, X);
	}

	void TVector3::Normalize(TVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR X = XMVector3Normalize(v1);
		XMStoreFloat3(&result, X);
	}

	void TVector3::Clamp(const TVector3& vmin, const TVector3& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&vmin);
		XMVECTOR v3 = XMLoadFloat3(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat3(this, X);
	}

	void TVector3::Clamp(const TVector3& vmin, const TVector3& vmax, TVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(this);
		XMVECTOR v2 = XMLoadFloat3(&vmin);
		XMVECTOR v3 = XMLoadFloat3(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat3(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------
	float TVector3::Distance(const TVector3& v1, const TVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector3Length(V);
		return XMVectorGetX(X);
	}

	float TVector3::DistanceSquared(const TVector3& v1, const TVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector3LengthSq(V);
		return XMVectorGetX(X);
	}

	void TVector3::Min(const TVector3& v1, const TVector3& v2, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Min(const TVector3& v1, const TVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Max(const TVector3& v1, const TVector3& v2, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Max(const TVector3& v1, const TVector3& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Lerp(const TVector3& v1, const TVector3& v2, float t, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Lerp(const TVector3& v1, const TVector3& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::SmoothStep(const TVector3& v1, const TVector3& v2, float t, TVector3& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::SmoothStep(const TVector3& v1, const TVector3& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Barycentric(const TVector3& v1, const TVector3& v2, const TVector3& v3, float f, float g, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Barycentric(const TVector3& v1, const TVector3& v2, const TVector3& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::CatmullRom(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TVector3& v4, float t, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR x4 = XMLoadFloat3(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::CatmullRom(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TVector3& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&v2);
		XMVECTOR x3 = XMLoadFloat3(&v3);
		XMVECTOR x4 = XMLoadFloat3(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Hermite(const TVector3& v1, const TVector3& t1, const TVector3& v2, const TVector3& t2, float t, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&t1);
		XMVECTOR x3 = XMLoadFloat3(&v2);
		XMVECTOR x4 = XMLoadFloat3(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Hermite(const TVector3& v1, const TVector3& t1, const TVector3& v2, const TVector3& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat3(&v1);
		XMVECTOR x2 = XMLoadFloat3(&t1);
		XMVECTOR x3 = XMLoadFloat3(&v2);
		XMVECTOR x4 = XMLoadFloat3(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Reflect(const TVector3& ivec, const TVector3& nvec, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Reflect(i, n);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Reflect(const TVector3& ivec, const TVector3& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Reflect(i, n);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Refract(const TVector3& ivec, const TVector3& nvec, float refractionIndex, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Refract(const TVector3& ivec, const TVector3& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat3(&ivec);
		XMVECTOR n = XMLoadFloat3(&nvec);
		XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Transform(const TVector3& v, const TQuaternion& quat, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4((XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Transform(const TVector3& v, const TQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4((XMFLOAT4*)&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	void TVector3::Transform(const TVector3& v, const TMatrix& m, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformCoord(v1, M);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::Transform(const TVector3& v, const TMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformCoord(v1, M);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void TVector3::Transform(const TVector3* varray, size_t count, const TMatrix& m, TVector3* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
	}

	void TVector3::Transform(const TVector3& v, const TMatrix& m, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3Transform(v1, M);
		XMStoreFloat4(&result, X);
	}

	_Use_decl_annotations_
		void TVector3::Transform(const TVector3* varray, size_t count, const TMatrix& m, TVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformStream((XMFLOAT4*)resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
	}

	void TVector3::TransformNormal(const TVector3& v, const TMatrix& m, TVector3& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformNormal(v1, M);
		XMStoreFloat3(&result, X);
	}

	TVector3 TVector3::TransformNormal(const TVector3& v, const TMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector3TransformNormal(v1, M);

		TVector3 result;
		XMStoreFloat3(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void TVector3::TransformNormal(const TVector3* varray, size_t count, const TMatrix& m, TVector3* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
	}


	///****************************************************************************
	//*
	//* TVector4
	//*
	//****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TVector4::operator == (const TVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		return XMVector4Equal(v1, v2);
	}

	bool TVector4::operator != (const TVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		return XMVector4NotEqual(v1, v2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TVector4& TVector4::operator+= (const TVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorAdd(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	TVector4& TVector4::operator-= (const TVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	TVector4& TVector4::operator*= (const TVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		XMStoreFloat4(this, X);
		return *this;
	}

	TVector4& TVector4::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorScale(v1, S);
		XMStoreFloat4(this, X);
		return *this;
	}

	TVector4& TVector4::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.0f);
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorScale(v1, 1.f / S);
		XMStoreFloat4(this, X);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	TVector4 TVector4::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVectorNegate(v1);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TVector4 operator+ (const TVector4& V1, const TVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorAdd(v1, v2);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	TVector4 operator- (const TVector4& V1, const TVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorSubtract(v1, v2);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	TVector4 operator* (const TVector4& V1, const TVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorMultiply(v1, v2);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	TVector4 operator* (const TVector4& V, float S)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	TVector4 operator/ (const TVector4& V1, const TVector4& V2)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V1);
		XMVECTOR v2 = XMLoadFloat4(&V2);
		XMVECTOR X = XMVectorDivide(v1, v2);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	TVector4 operator* (float S, const TVector4& V)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&V);
		XMVECTOR X = XMVectorScale(v1, S);
		TVector4 R;
		XMStoreFloat4(&R, X);
		return R;
	}

	//------------------------------------------------------------------------------
	// Vector operations
	//------------------------------------------------------------------------------

	bool TVector4::InBounds(const TVector4& Bounds) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&Bounds);
		return XMVector4InBounds(v1, v2);
	}

	float TVector4::Length() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Length(v1);
		return XMVectorGetX(X);
	}

	float TVector4::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4LengthSq(v1);
		return XMVectorGetX(X);
	}

	float TVector4::Dot(const TVector4& V) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&V);
		XMVECTOR X = XMVector4Dot(v1, v2);
		return XMVectorGetX(X);
	}

	void TVector4::Cross(const TVector4& v1, const TVector4& v2, TVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(this);
		XMVECTOR x2 = XMLoadFloat4(&v1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR R = XMVector4Cross(x1, x2, x3);
		XMStoreFloat4(&result, R);
	}

	TVector4 TVector4::Cross(const TVector4& v1, const TVector4& v2) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(this);
		XMVECTOR x2 = XMLoadFloat4(&v1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR R = XMVector4Cross(x1, x2, x3);

		TVector4 result;
		XMStoreFloat4(&result, R);
		return result;
	}

	void TVector4::Normalize()
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Normalize(v1);
		XMStoreFloat4(this, X);
	}

	void TVector4::Normalize(TVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR X = XMVector4Normalize(v1);
		XMStoreFloat4(&result, X);
	}

	void TVector4::Clamp(const TVector4& vmin, const TVector4& vmax)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&vmin);
		XMVECTOR v3 = XMLoadFloat4(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat4(this, X);
	}

	void TVector4::Clamp(const TVector4& vmin, const TVector4& vmax, TVector4& result) const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(this);
		XMVECTOR v2 = XMLoadFloat4(&vmin);
		XMVECTOR v3 = XMLoadFloat4(&vmax);
		XMVECTOR X = XMVectorClamp(v1, v2, v3);
		XMStoreFloat4(&result, X);
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	float TVector4::Distance(const TVector4& v1, const TVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector4Length(V);
		return XMVectorGetX(X);
	}

	float TVector4::DistanceSquared(const TVector4& v1, const TVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR V = XMVectorSubtract(x2, x1);
		XMVECTOR X = XMVector4LengthSq(V);
		return XMVectorGetX(X);
	}

	void TVector4::Min(const TVector4& v1, const TVector4& v2, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Min(const TVector4& v1, const TVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMin(x1, x2);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Max(const TVector4& v1, const TVector4& v2, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Max(const TVector4& v1, const TVector4& v2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorMax(x1, x2);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Lerp(const TVector4& v1, const TVector4& v2, float t, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Lerp(const TVector4& v1, const TVector4& v2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::SmoothStep(const TVector4& v1, const TVector4& v2, float t, TVector4& result)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::SmoothStep(const TVector4& v1, const TVector4& v2, float t)
	{
		using namespace DirectX;
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t*(3.f - 2.f*t);
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR X = XMVectorLerp(x1, x2, t);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Barycentric(const TVector4& v1, const TVector4& v2, const TVector4& v3, float f, float g, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Barycentric(const TVector4& v1, const TVector4& v2, const TVector4& v3, float f, float g)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::CatmullRom(const TVector4& v1, const TVector4& v2, const TVector4& v3, const TVector4& v4, float t, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR x4 = XMLoadFloat4(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::CatmullRom(const TVector4& v1, const TVector4& v2, const TVector4& v3, const TVector4& v4, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&v2);
		XMVECTOR x3 = XMLoadFloat4(&v3);
		XMVECTOR x4 = XMLoadFloat4(&v4);
		XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Hermite(const TVector4& v1, const TVector4& t1, const TVector4& v2, const TVector4& t2, float t, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&t1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR x4 = XMLoadFloat4(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Hermite(const TVector4& v1, const TVector4& t1, const TVector4& v2, const TVector4& t2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(&v1);
		XMVECTOR x2 = XMLoadFloat4(&t1);
		XMVECTOR x3 = XMLoadFloat4(&v2);
		XMVECTOR x4 = XMLoadFloat4(&t2);
		XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Reflect(const TVector4& ivec, const TVector4& nvec, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Reflect(i, n);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Reflect(const TVector4& ivec, const TVector4& nvec)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Reflect(i, n);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Refract(const TVector4& ivec, const TVector4& nvec, float refractionIndex, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Refract(const TVector4& ivec, const TVector4& nvec, float refractionIndex)
	{
		using namespace DirectX;
		XMVECTOR i = XMLoadFloat4(&ivec);
		XMVECTOR n = XMLoadFloat4(&nvec);
		XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Transform(const TVector2& v, const TQuaternion& quat, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Transform(const TVector2& v, const TQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat2(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Transform(const TVector3& v, const TQuaternion& quat, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Transform(const TVector3& v, const TQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat3(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Transform(const TVector4& v, const TQuaternion& quat, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Transform(const TVector4& v, const TQuaternion& quat)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMVECTOR q = XMLoadFloat4(&quat);
		XMVECTOR X = XMVector3Rotate(v1, q);
		X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	void TVector4::Transform(const TVector4& v, const TMatrix& m, TVector4& result)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector4Transform(v1, M);
		XMStoreFloat4(&result, X);
	}

	TVector4 TVector4::Transform(const TVector4& v, const TMatrix& m)
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(&v);
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVECTOR X = XMVector4Transform(v1, M);

		TVector4 result;
		XMStoreFloat4(&result, X);
		return result;
	}

	_Use_decl_annotations_
		void TVector4::Transform(const TVector4* varray, size_t count, const TMatrix& m, TVector4* resultArray)
	{
		using namespace DirectX;
		XMMATRIX M = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
	}


	/****************************************************************************
	*
	* TMatrix
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TMatrix::operator == (const TMatrix& M) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4Equal(x1, y1)
			&& XMVector4Equal(x2, y2)
			&& XMVector4Equal(x3, y3)
			&& XMVector4Equal(x4, y4)) != 0;
	}

	bool TMatrix::operator != (const TMatrix& M) const
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		return (XMVector4NotEqual(x1, y1)
			|| XMVector4NotEqual(x2, y2)
			|| XMVector4NotEqual(x3, y3)
			|| XMVector4NotEqual(x4, y4)) != 0;
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TMatrix::TMatrix(const XMFLOAT3X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
	}

	TMatrix::TMatrix(const XMFLOAT4X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
	}

	TMatrix& TMatrix::operator= (const XMFLOAT3X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
		return *this;
	}

	TMatrix& TMatrix::operator= (const XMFLOAT4X3& M)
	{
		_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
		_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
		_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
		_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
		return *this;
	}

	TMatrix& TMatrix::operator+= (const TMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorAdd(x1, y1);
		x2 = XMVectorAdd(x2, y2);
		x3 = XMVectorAdd(x3, y3);
		x4 = XMVectorAdd(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	TMatrix& TMatrix::operator-= (const TMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorSubtract(x1, y1);
		x2 = XMVectorSubtract(x2, y2);
		x3 = XMVectorSubtract(x3, y3);
		x4 = XMVectorSubtract(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	TMatrix& TMatrix::operator*= (const TMatrix& M)
	{
		using namespace DirectX;
		XMMATRIX M1 = XMLoadFloat4x4(this);
		XMMATRIX M2 = XMLoadFloat4x4((XMFLOAT4X4*)&m);
		XMMATRIX X = XMMatrixMultiply(M1, M2);
		XMStoreFloat4x4(this, X);
		return *this;
	}

	TMatrix& TMatrix::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	TMatrix& TMatrix::operator/= (float S)
	{
		using namespace DirectX;
		assert(S != 0.f);
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		float rs = 1.f / S;

		x1 = XMVectorScale(x1, rs);
		x2 = XMVectorScale(x2, rs);
		x3 = XMVectorScale(x3, rs);
		x4 = XMVectorScale(x4, rs);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	TMatrix& TMatrix::operator/= (const TMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	TMatrix TMatrix::operator- () const
	{
		using namespace DirectX;
		XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
		XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
		XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
		XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

		v1 = XMVectorNegate(v1);
		v2 = XMVectorNegate(v2);
		v3 = XMVectorNegate(v3);
		v4 = XMVectorNegate(v4);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TMatrix operator+ (const TMatrix& M1, const TMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorAdd(x1, y1);
		x2 = XMVectorAdd(x2, y2);
		x3 = XMVectorAdd(x3, y3);
		x4 = XMVectorAdd(x4, y4);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	TMatrix operator- (const TMatrix& M1, const TMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorSubtract(x1, y1);
		x2 = XMVectorSubtract(x2, y2);
		x3 = XMVectorSubtract(x3, y3);
		x4 = XMVectorSubtract(x4, y4);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	TMatrix operator* (const TMatrix& M1, const TMatrix& M2)
	{
		using namespace DirectX;
		XMMATRIX m1 = XMLoadFloat4x4(&M1);
		XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)&M2);
		XMMATRIX X = XMMatrixMultiply(m1, m2);

		TMatrix R;
		XMStoreFloat4x4(&R, X);
		return R;
	}

	TMatrix operator* (const TMatrix& M, float S)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	TMatrix operator/ (const TMatrix& M, float S)
	{
		using namespace DirectX;
		assert(S != 0.f);

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		float rs = 1.f / S;

		x1 = XMVectorScale(x1, rs);
		x2 = XMVectorScale(x2, rs);
		x3 = XMVectorScale(x3, rs);
		x4 = XMVectorScale(x4, rs);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	TMatrix operator/ (const TMatrix& M1, const TMatrix& M2)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorDivide(x1, y1);
		x2 = XMVectorDivide(x2, y2);
		x3 = XMVectorDivide(x3, y3);
		x4 = XMVectorDivide(x4, y4);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	TMatrix operator* (float S, const TMatrix& M)
	{
		using namespace DirectX;

		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

		x1 = XMVectorScale(x1, S);
		x2 = XMVectorScale(x2, S);
		x3 = XMVectorScale(x3, S);
		x4 = XMVectorScale(x4, S);

		TMatrix R;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
		return R;
	}

	//------------------------------------------------------------------------------
	// TMatrix operations
	//------------------------------------------------------------------------------

	bool TMatrix::Decompose(TVector3& scale, TQuaternion& rotation, TVector3& translation)
	{
		using namespace DirectX;

		XMVECTOR s, r, t;

		if (!XMMatrixDecompose(&s, &r, &t, *(XMMATRIX*)this))
			return false;

		XMStoreFloat3(&scale, s);
		XMStoreFloat4(&rotation, r);
		XMStoreFloat3(&translation, t);

		return true;
	}

	TMatrix TMatrix::Transpose() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranspose(M));
		return R;
	}

	void TMatrix::Transpose(TMatrix& result) const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		XMStoreFloat4x4(&result, XMMatrixTranspose(M));
	}

	TMatrix TMatrix::Invert() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		TMatrix R;
		XMVECTOR det;
		XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
		return R;
	}

	void TMatrix::Invert(TMatrix& result) const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		XMVECTOR det;
		XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
	}

	float TMatrix::Determinant() const
	{
		using namespace DirectX;
		FXMMATRIX M = XMLoadFloat4x4(this);
		return XMVectorGetX(XMMatrixDeterminant(M));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	_Use_decl_annotations_
		TMatrix TMatrix::CreateBillboard(const TVector3& object, const TVector3& cameraPosition, const TVector3& cameraUp, const TVector3* cameraForward)
	{
		using namespace DirectX;
		XMVECTOR O = XMLoadFloat3(&object);
		XMVECTOR C = XMLoadFloat3(&cameraPosition);
		XMVECTOR Z = XMVectorSubtract(O, C);

		XMVECTOR N = XMVector3LengthSq(Z);
		if (XMVector3Less(N, g_XMEpsilon))
		{
			if (cameraForward)
			{
				XMVECTOR F = XMLoadFloat3(cameraForward);
				Z = XMVectorNegate(F);
			}
			else
				Z = g_XMNegIdentityR2;
		}
		else
		{
			Z = XMVector3Normalize(Z);
		}

		XMVECTOR up = XMLoadFloat3(&cameraUp);
		XMVECTOR X = XMVector3Cross(up, Z);
		X = XMVector3Normalize(X);

		XMVECTOR Y = XMVector3Cross(Z, X);

		XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = XMVectorSetW(O, 1.f);

		TMatrix R;
		XMStoreFloat4x4(&R, M);
		return R;
	}

	_Use_decl_annotations_
		TMatrix TMatrix::CreateConstrainedBillboard(const TVector3& object, const TVector3& cameraPosition, const TVector3& rotateAxis,
			const TVector3* cameraForward, const TVector3* objectForward)
	{
		using namespace DirectX;

		static const XMVECTORF32 s_minAngle = { { { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f } } }; // 1.0 - XMConvertToRadians( 0.1f );

		XMVECTOR O = XMLoadFloat3(&object);
		XMVECTOR C = XMLoadFloat3(&cameraPosition);
		XMVECTOR faceDir = XMVectorSubtract(O, C);

		XMVECTOR N = XMVector3LengthSq(faceDir);
		if (XMVector3Less(N, g_XMEpsilon))
		{
			if (cameraForward)
			{
				XMVECTOR F = XMLoadFloat3(cameraForward);
				faceDir = XMVectorNegate(F);
			}
			else
				faceDir = g_XMNegIdentityR2;
		}
		else
		{
			faceDir = XMVector3Normalize(faceDir);
		}

		XMVECTOR Y = XMLoadFloat3(&rotateAxis);
		XMVECTOR X, Z;

		XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
		if (XMVector3Greater(dot, s_minAngle))
		{
			if (objectForward)
			{
				Z = XMLoadFloat3(objectForward);
				dot = XMVectorAbs(XMVector3Dot(Y, Z));
				if (XMVector3Greater(dot, s_minAngle))
				{
					dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
					Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
				}
			}
			else
			{
				dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
				Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
			}

			X = XMVector3Cross(Y, Z);
			X = XMVector3Normalize(X);

			Z = XMVector3Cross(X, Y);
			Z = XMVector3Normalize(Z);
		}
		else
		{
			X = XMVector3Cross(Y, faceDir);
			X = XMVector3Normalize(X);

			Z = XMVector3Cross(X, Y);
			Z = XMVector3Normalize(Z);
		}

		XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = XMVectorSetW(O, 1.f);

		TMatrix R;
		XMStoreFloat4x4(&R, M);
		return R;
	}

	TMatrix TMatrix::CreateTranslation(const TVector3& position)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
		return R;
	}

	TMatrix TMatrix::CreateTranslation(float x, float y, float z)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
		return R;
	}

	TMatrix TMatrix::CreateScale(const TVector3& scales)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
		return R;
	}

	TMatrix TMatrix::CreateScale(float xs, float ys, float zs)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
		return R;
	}

	TMatrix TMatrix::CreateScale(float scale)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
		return R;
	}

	TMatrix TMatrix::CreateRotationX(float radians)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
		return R;
	}

	TMatrix TMatrix::CreateRotationY(float radians)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
		return R;
	}

	TMatrix TMatrix::CreateRotationZ(float radians)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
		return R;
	}

	TMatrix TMatrix::CreateFromAxisAngle(const TVector3& axis, float angle)
	{
		using namespace DirectX;
		TMatrix R;
		XMVECTOR a = XMLoadFloat3(&axis);
		XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
		return R;
	}

	TMatrix TMatrix::CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
		return R;
	}

	TMatrix TMatrix::CreatePerspective(float width, float height, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveLH(width, height, nearPlane, farPlane));
		return R;
	}

	TMatrix TMatrix::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane));
		return R;
	}

	TMatrix TMatrix::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixOrthographicLH(width, height, zNearPlane, zFarPlane));
		return R;
	}

	TMatrix TMatrix::CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNearPlane, zFarPlane));
		return R;
	}

	TMatrix TMatrix::CreateLookAt(const TVector3& eye, const TVector3& target, const TVector3& up)
	{
		using namespace DirectX;
		TMatrix R;
		XMVECTOR eyev = XMLoadFloat3(&eye);
		XMVECTOR targetv = XMLoadFloat3(&target);
		XMVECTOR upv = XMLoadFloat3(&up);
		XMStoreFloat4x4(&R, XMMatrixLookAtLH(eyev, targetv, upv));
		return R;
	}

	TMatrix TMatrix::CreateWorld(const TVector3& position, const TVector3& forward, const TVector3& up)
	{
		using namespace DirectX;
		XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
		XMVECTOR yaxis = XMLoadFloat3(&up);
		XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
		yaxis = XMVector3Cross(zaxis, xaxis);

		TMatrix R;
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
		R._14 = R._24 = R._34 = 0.f;
		R._41 = position.x; R._42 = position.y; R._43 = position.z;
		R._44 = 1.f;
		return R;
	}

	TMatrix TMatrix::CreateFromQuaternion(const TQuaternion& rotation)
	{
		using namespace DirectX;
		TMatrix R;
		XMVECTOR quatv = XMLoadFloat4(&rotation);
		XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
		return R;
	}

	TMatrix TMatrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		using namespace DirectX;
		TMatrix R;
		XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		return R;
	}

	TMatrix TMatrix::CreateShadow(const TVector3& lightDir, const TPlane& plane)
	{
		using namespace DirectX;
		TMatrix R;
		XMVECTOR light = XMLoadFloat3(&lightDir);
		XMVECTOR planev = XMLoadFloat4(&plane);
		XMStoreFloat4x4(&R, XMMatrixShadow(planev, light));
		return R;
	}

	TMatrix TMatrix::CreateReflection(const TPlane& plane)
	{
		using namespace DirectX;
		TMatrix R;
		XMVECTOR planev = XMLoadFloat4(&plane);
		XMStoreFloat4x4(&R, XMMatrixReflect(planev));
		return R;
	}

	void TMatrix::Lerp(const TMatrix& M1, const TMatrix& M2, float t, TMatrix& result)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorLerp(x1, y1, t);
		x2 = XMVectorLerp(x2, y2, t);
		x3 = XMVectorLerp(x3, y3, t);
		x4 = XMVectorLerp(x4, y4, t);

		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
	}

	TMatrix TMatrix::Lerp(const TMatrix& M1, const TMatrix& M2, float t)
	{
		using namespace DirectX;
		XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
		XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
		XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
		XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

		XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
		XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
		XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
		XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

		x1 = XMVectorLerp(x1, y1, t);
		x2 = XMVectorLerp(x2, y2, t);
		x3 = XMVectorLerp(x3, y3, t);
		x4 = XMVectorLerp(x4, y4, t);

		TMatrix result;
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
		XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
		return result;
	}

	void TMatrix::Transform(const TMatrix& M, const TQuaternion& rotation, TMatrix& result)
	{
		using namespace DirectX;
		XMVECTOR quatv = XMLoadFloat4(&rotation);

		XMMATRIX M0 = XMLoadFloat4x4(&M);
		XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

		XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
	}

	TMatrix TMatrix::Transform(const TMatrix& M, const TQuaternion& rotation)
	{
		using namespace DirectX;
		XMVECTOR quatv = XMLoadFloat4(&rotation);

		XMMATRIX M0 = XMLoadFloat4x4(&M);
		XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

		TMatrix result;
		XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
		return result;
	}


	/****************************************************************************
	*
	* TPlane
	*
	****************************************************************************/

	TPlane::TPlane(const TVector3& point1, const TVector3& point2, const TVector3& point3)
	{
		using namespace DirectX;
		XMVECTOR P0 = XMLoadFloat3(&point1);
		XMVECTOR P1 = XMLoadFloat3(&point2);
		XMVECTOR P2 = XMLoadFloat3(&point3);
		XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
	}

	TPlane::TPlane(const TVector3& point, const TVector3& normal)
	{
		using namespace DirectX;
		XMVECTOR P = XMLoadFloat3(&point);
		XMVECTOR N = XMLoadFloat3(&normal);
		XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
	}

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TPlane::operator == (const TPlane& p) const
	{
		using namespace DirectX;
		XMVECTOR p1 = XMLoadFloat4(this);
		XMVECTOR p2 = XMLoadFloat4(&p);
		return XMPlaneEqual(p1, p2);
	}

	bool TPlane::operator != (const TPlane& p) const
	{
		using namespace DirectX;
		XMVECTOR p1 = XMLoadFloat4(this);
		XMVECTOR p2 = XMLoadFloat4(&p);
		return XMPlaneNotEqual(p1, p2);
	}

	//------------------------------------------------------------------------------
	// TPlane operations
	//------------------------------------------------------------------------------

	void TPlane::Normalize()
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMStoreFloat4(this, XMPlaneNormalize(p));
	}

	void TPlane::Normalize(TPlane& result) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMPlaneNormalize(p));
	}

	float TPlane::Dot(const TVector4& v) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR v0 = XMLoadFloat4(&v);
		return XMVectorGetX(XMPlaneDot(p, v0));
	}

	float TPlane::DotCoordinate(const TVector3& position) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR v0 = XMLoadFloat3(&position);
		return XMVectorGetX(XMPlaneDotCoord(p, v0));
	}

	float TPlane::DotNormal(const TVector3& normal) const
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(this);
		XMVECTOR n0 = XMLoadFloat3(&normal);
		return XMVectorGetX(XMPlaneDotNormal(p, n0));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	void TPlane::Transform(const TPlane& plane, const TMatrix& M, TPlane& result)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMMATRIX m0 = XMLoadFloat4x4(&M);
		XMStoreFloat4(&result, XMPlaneTransform(p, m0));
	}

	TPlane TPlane::Transform(const TPlane& plane, const TMatrix& M)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMMATRIX m0 = XMLoadFloat4x4(&M);

		TPlane result;
		XMStoreFloat4(&result, XMPlaneTransform(p, m0));
		return result;
	}

	void TPlane::Transform(const TPlane& plane, const TQuaternion& rotation, TPlane& result)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR q = XMLoadFloat4(&rotation);
		XMVECTOR X = XMVector3Rotate(p, q);
		X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
		XMStoreFloat4(&result, X);
	}

	TPlane TPlane::Transform(const TPlane& plane, const TQuaternion& rotation)
	{
		using namespace DirectX;
		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR q = XMLoadFloat4(&rotation);
		XMVECTOR X = XMVector3Rotate(p, q);
		X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

		TPlane result;
		XMStoreFloat4(&result, X);
		return result;
	}


	/****************************************************************************
	*
	* TQuaternion
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TQuaternion::operator == (const TQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMQuaternionEqual(q1, q2);
	}

	bool TQuaternion::operator != (const TQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMQuaternionNotEqual(q1, q2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TQuaternion& TQuaternion::operator+= (const TQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMVectorAdd(q1, q2));
		return *this;
	}

	TQuaternion& TQuaternion::operator-= (const TQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMVectorSubtract(q1, q2));
		return *this;
	}

	TQuaternion& TQuaternion::operator*= (const TQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
		return *this;
	}

	TQuaternion& TQuaternion::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorScale(q, S));
		return *this;
	}

	TQuaternion& TQuaternion::operator/= (const TQuaternion& q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		q2 = XMQuaternionInverse(q2);
		XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	TQuaternion TQuaternion::operator- () const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);

		TQuaternion R;
		XMStoreFloat4(&R, XMVectorNegate(q));
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TQuaternion operator+ (const TQuaternion& Q1, const TQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		TQuaternion R;
		XMStoreFloat4(&R, XMVectorAdd(q1, q2));
		return R;
	}

	TQuaternion operator- (const TQuaternion& Q1, const TQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		TQuaternion R;
		XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
		return R;
	}

	TQuaternion operator* (const TQuaternion& Q1, const TQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);

		TQuaternion R;
		XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
		return R;
	}

	TQuaternion operator* (const TQuaternion& Q, float S)
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(&Q);

		TQuaternion R;
		XMStoreFloat4(&R, XMVectorScale(q, S));
		return R;
	}

	TQuaternion operator/ (const TQuaternion& Q1, const TQuaternion& Q2)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q1);
		XMVECTOR q2 = XMLoadFloat4(&Q2);
		q2 = XMQuaternionInverse(q2);

		TQuaternion R;
		XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
		return R;
	}

	TQuaternion operator* (float S, const TQuaternion& Q)
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(&Q);

		TQuaternion R;
		XMStoreFloat4(&R, XMVectorScale(q1, S));
		return R;
	}

	//------------------------------------------------------------------------------
	// TQuaternion operations
	//------------------------------------------------------------------------------

	float TQuaternion::Length() const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		return XMVectorGetX(XMQuaternionLength(q));
	}

	float TQuaternion::LengthSquared() const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		return XMVectorGetX(XMQuaternionLengthSq(q));
	}

	void TQuaternion::Normalize()
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMQuaternionNormalize(q));
	}

	void TQuaternion::Normalize(TQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionNormalize(q));
	}

	void TQuaternion::Conjugate()
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(this, XMQuaternionConjugate(q));
	}

	void TQuaternion::Conjugate(TQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionConjugate(q));
	}

	void TQuaternion::Inverse(TQuaternion& result) const
	{
		using namespace DirectX;
		XMVECTOR q = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMQuaternionInverse(q));
	}

	float TQuaternion::Dot(const TQuaternion& q) const
	{
		using namespace DirectX;
		XMVECTOR q1 = XMLoadFloat4(this);
		XMVECTOR q2 = XMLoadFloat4(&q);
		return XMVectorGetX(XMQuaternionDot(q1, q2));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	TQuaternion TQuaternion::CreateFromAxisAngle(const TVector3& axis, float angle)
	{
		using namespace DirectX;
		XMVECTOR a = XMLoadFloat3(&axis);

		TQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
		return R;
	}

	TQuaternion TQuaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		using namespace DirectX;
		TQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
		return R;
	}

	TQuaternion TQuaternion::CreateFromRotationMatrix(const TMatrix& M)
	{
		using namespace DirectX;
		XMMATRIX M0 = XMLoadFloat4x4(&M);

		TQuaternion R;
		XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
		return R;
	}

	void TQuaternion::Lerp(const TQuaternion& q1, const TQuaternion& q2, float t, TQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		XMVECTOR dot = XMVector4Dot(Q0, Q1);

		XMVECTOR R;
		if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
		{
			R = XMVectorLerp(Q0, Q1, t);
		}
		else
		{
			XMVECTOR tv = XMVectorReplicate(t);
			XMVECTOR t1v = XMVectorReplicate(1.f - t);
			XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
			XMVECTOR X1 = XMVectorMultiply(Q1, tv);
			R = XMVectorSubtract(X0, X1);
		}

		XMStoreFloat4(&result, XMQuaternionNormalize(R));
	}

	TQuaternion TQuaternion::Lerp(const TQuaternion& q1, const TQuaternion& q2, float t)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		XMVECTOR dot = XMVector4Dot(Q0, Q1);

		XMVECTOR R;
		if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
		{
			R = XMVectorLerp(Q0, Q1, t);
		}
		else
		{
			XMVECTOR tv = XMVectorReplicate(t);
			XMVECTOR t1v = XMVectorReplicate(1.f - t);
			XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
			XMVECTOR X1 = XMVectorMultiply(Q1, tv);
			R = XMVectorSubtract(X0, X1);
		}

		TQuaternion result;
		XMStoreFloat4(&result, XMQuaternionNormalize(R));
		return result;
	}

	void TQuaternion::Slerp(const TQuaternion& q1, const TQuaternion& q2, float t, TQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);
		XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
	}

	TQuaternion TQuaternion::Slerp(const TQuaternion& q1, const TQuaternion& q2, float t)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		TQuaternion result;
		XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
		return result;
	}

	void TQuaternion::Concatenate(const TQuaternion& q1, const TQuaternion& q2, TQuaternion& result)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);
		XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
	}

	TQuaternion TQuaternion::Concatenate(const TQuaternion& q1, const TQuaternion& q2)
	{
		using namespace DirectX;
		XMVECTOR Q0 = XMLoadFloat4(&q1);
		XMVECTOR Q1 = XMLoadFloat4(&q2);

		TQuaternion result;
		XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
		return result;
	}


	/****************************************************************************
	*
	* TColor
	*
	****************************************************************************/

	TColor::TColor(const DirectX::PackedVector::XMCOLOR& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
	}

	TColor::TColor(const DirectX::PackedVector::XMUBYTEN4& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
	}

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool TColor::operator == (const TColor& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorEqual(c1, c2);
	}

	bool TColor::operator != (const TColor& c) const
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		return XMColorNotEqual(c1, c2);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TColor& TColor::operator= (const DirectX::PackedVector::XMCOLOR& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadColor(&Packed));
		return *this;
	}

	TColor& TColor::operator= (const DirectX::PackedVector::XMUBYTEN4& Packed)
	{
		using namespace DirectX;
		XMStoreFloat4(this, PackedVector::XMLoadUByteN4(&Packed));
		return *this;
	}

	TColor& TColor::operator+= (const TColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorAdd(c1, c2));
		return *this;
	}

	TColor& TColor::operator-= (const TColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorSubtract(c1, c2));
		return *this;
	}

	TColor& TColor::operator*= (const TColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorMultiply(c1, c2));
		return *this;
	}

	TColor& TColor::operator*= (float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorScale(c, S));
		return *this;
	}

	TColor& TColor::operator/= (const TColor& c)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(this);
		XMVECTOR c2 = XMLoadFloat4(&c);
		XMStoreFloat4(this, XMVectorDivide(c1, c2));
		return *this;
	}

	//------------------------------------------------------------------------------
	// Urnary operators
	//------------------------------------------------------------------------------

	TColor TColor::operator- () const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		TColor R;
		XMStoreFloat4(&R, XMVectorNegate(c));
		return R;
	}

	//------------------------------------------------------------------------------
	// Binary operators
	//------------------------------------------------------------------------------

	TColor operator+ (const TColor& C1, const TColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		TColor R;
		XMStoreFloat4(&R, XMVectorAdd(c1, c2));
		return R;
	}

	TColor operator- (const TColor& C1, const TColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		TColor R;
		XMStoreFloat4(&R, XMVectorSubtract(c1, c2));
		return R;
	}

	TColor operator* (const TColor& C1, const TColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		TColor R;
		XMStoreFloat4(&R, XMVectorMultiply(c1, c2));
		return R;
	}

	TColor operator* (const TColor& C, float S)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(&C);
		TColor R;
		XMStoreFloat4(&R, XMVectorScale(c, S));
		return R;
	}

	TColor operator/ (const TColor& C1, const TColor& C2)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C1);
		XMVECTOR c2 = XMLoadFloat4(&C2);
		TColor R;
		XMStoreFloat4(&R, XMVectorDivide(c1, c2));
		return R;
	}

	TColor operator* (float S, const TColor& C)
	{
		using namespace DirectX;
		XMVECTOR c1 = XMLoadFloat4(&C);
		TColor R;
		XMStoreFloat4(&R, XMVectorScale(c1, S));
		return R;
	}

	//------------------------------------------------------------------------------
	// TColor operations
	//------------------------------------------------------------------------------

	DirectX::PackedVector::XMCOLOR TColor::BGRA() const
	{
		using namespace DirectX;
		XMVECTOR clr = XMLoadFloat4(this);
		PackedVector::XMCOLOR Packed;
		PackedVector::XMStoreColor(&Packed, clr);
		return Packed;
	}

	DirectX::PackedVector::XMUBYTEN4 TColor::RGBA() const
	{
		using namespace DirectX;
		XMVECTOR clr = XMLoadFloat4(this);
		PackedVector::XMUBYTEN4 Packed;
		PackedVector::XMStoreUByteN4(&Packed, clr);
		return Packed;
	}

	TVector3 TColor::ToVector3() const
	{
		return TVector3(x, y, z);
	}

	TVector4 TColor::ToVector4() const
	{
		return TVector4(x, y, z, w);
	}

	void TColor::Negate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorNegative(c));
	}

	void TColor::Negate(TColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorNegative(c));
	}

	void TColor::Saturate()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMVectorSaturate(c));
	}

	void TColor::Saturate(TColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMVectorSaturate(c));
	}

	void TColor::Premultiply()
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(this, XMVectorMultiply(c, a));
	}

	void TColor::Premultiply(TColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMVECTOR a = XMVectorSplatW(c);
		a = XMVectorSelect(g_XMIdentityR3, a, g_XMSelect1110);
		XMStoreFloat4(&result, XMVectorMultiply(c, a));
	}

	void TColor::AdjustSaturation(float sat)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustSaturation(c, sat));
	}

	void TColor::AdjustSaturation(float sat, TColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustSaturation(c, sat));
	}

	void TColor::AdjustContrast(float contrast)
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(this, XMColorAdjustContrast(c, contrast));
	}

	void TColor::AdjustContrast(float contrast, TColor& result) const
	{
		using namespace DirectX;
		XMVECTOR c = XMLoadFloat4(this);
		XMStoreFloat4(&result, XMColorAdjustContrast(c, contrast));
	}

	//------------------------------------------------------------------------------
	// Static functions
	//------------------------------------------------------------------------------

	void TColor::Modulate(const TColor& c1, const TColor& c2, TColor& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
	}

	TColor TColor::Modulate(const TColor& c1, const TColor& c2)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		TColor result;
		XMStoreFloat4(&result, XMColorModulate(C0, C1));
		return result;
	}

	void TColor::Lerp(const TColor& c1, const TColor& c2, float t, TColor& result)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
	}

	TColor TColor::Lerp(const TColor& c1, const TColor& c2, float t)
	{
		using namespace DirectX;
		XMVECTOR C0 = XMLoadFloat4(&c1);
		XMVECTOR C1 = XMLoadFloat4(&c2);

		TColor result;
		XMStoreFloat4(&result, XMVectorLerp(C0, C1, t));
		return result;
	}


	/****************************************************************************
	*
	* TRay
	*
	****************************************************************************/

	//-----------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------
	bool TRay::operator == (const TRay& r) const
	{
		using namespace DirectX;
		XMVECTOR r1p = XMLoadFloat3(&position);
		XMVECTOR r2p = XMLoadFloat3(&r.position);
		XMVECTOR r1d = XMLoadFloat3(&direction);
		XMVECTOR r2d = XMLoadFloat3(&r.direction);
		return XMVector3Equal(r1p, r2p) && XMVector3Equal(r1d, r2d);
	}

	bool TRay::operator != (const TRay& r) const
	{
		using namespace DirectX;
		XMVECTOR r1p = XMLoadFloat3(&position);
		XMVECTOR r2p = XMLoadFloat3(&r.position);
		XMVECTOR r1d = XMLoadFloat3(&direction);
		XMVECTOR r2d = XMLoadFloat3(&r.direction);
		return XMVector3NotEqual(r1p, r2p) && XMVector3NotEqual(r1d, r2d);
	}

	//-----------------------------------------------------------------------------
	// TRay operators
	//------------------------------------------------------------------------------

	bool TRay::Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const
	{
		return sphere.Intersects(position, direction, Dist);
	}

	bool TRay::Intersects(const BoundingBox& box, _Out_ float& Dist) const
	{
		return box.Intersects(position, direction, Dist);
	}

	bool TRay::Intersects(const TVector3& tri0, const TVector3& tri1, const TVector3& tri2, _Out_ float& Dist) const
	{
		return DirectX::TriangleTests::Intersects(position, direction, tri0, tri1, tri2, Dist);
	}

	bool TRay::Intersects(const TPlane& plane, _Out_ float& Dist) const
	{
		using namespace DirectX;

		XMVECTOR p = XMLoadFloat4(&plane);
		XMVECTOR dir = XMLoadFloat3(&direction);

		XMVECTOR nd = XMPlaneDotNormal(p, dir);

		if (XMVector3LessOrEqual(XMVectorAbs(nd), g_RayEpsilon))
		{
			Dist = 0.f;
			return false;
		}
		else
		{
			// t = -(dot(n,origin) + D) / dot(n,dir)
			XMVECTOR pos = XMLoadFloat3(&position);
			XMVECTOR v = XMPlaneDotNormal(p, pos);
			v = XMVectorAdd(v, XMVectorSplatW(p));
			v = XMVectorDivide(v, nd);
			float dist = -XMVectorGetX(v);
			if (dist < 0)
			{
				Dist = 0.f;
				return false;
			}
			else
			{
				Dist = dist;
				return true;
			}
		}
	}


	/****************************************************************************
	*
	* TViewport
	*
	****************************************************************************/

	//------------------------------------------------------------------------------
	// Comparision operators
	//------------------------------------------------------------------------------

	bool TViewport::operator == (const TViewport& vp) const
	{
		return (x == vp.x && y == vp.y
			&& width == vp.width && height == vp.height
			&& minDepth == vp.minDepth && maxDepth == vp.maxDepth);
	}

	bool TViewport::operator != (const TViewport& vp) const
	{
		return (x != vp.x || y != vp.y
			|| width != vp.width || height != vp.height
			|| minDepth != vp.minDepth || maxDepth != vp.maxDepth);
	}

	//------------------------------------------------------------------------------
	// Assignment operators
	//------------------------------------------------------------------------------

	TViewport& TViewport::operator= (const RECT& rct)
	{
		x = float(rct.left); y = float(rct.top);
		width = float(rct.right - rct.left);
		height = float(rct.bottom - rct.top);
		minDepth = 0.f; maxDepth = 1.f;
		return *this;
	}

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
	TViewport& TViewport::operator= (const D3D11_VIEWPORT& vp)
	{
		x = vp.TopLeftX; y = vp.TopLeftY;
		width = vp.Width; height = vp.Height;
		minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
		return *this;
	}
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
	TViewport& TViewport::operator= (const D3D12_VIEWPORT& vp)
	{
		x = vp.TopLeftX; y = vp.TopLeftY;
		width = vp.Width; height = vp.Height;
		minDepth = vp.MinDepth; maxDepth = vp.MaxDepth;
		return *this;
	}
#endif

	//------------------------------------------------------------------------------
	// TViewport operations
	//------------------------------------------------------------------------------

	float TViewport::AspectRatio() const
	{
		if (width == 0.f || height == 0.f)
			return 0.f;

		return (width / height);
	}

	TVector3 TViewport::Project(const TVector3& p, const TMatrix& proj, const TMatrix& view, const TMatrix& world) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection,
			(CXMMATRIX)view, (CXMMATRIX)world);
		TVector3 result;
		XMStoreFloat3(&result, v);
		return result;
	}

	void TViewport::Project(const TVector3& p, const TMatrix& proj, const TMatrix& view, const TMatrix& world, TVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Project(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		XMStoreFloat3(&result, v);
	}

	TVector3 TViewport::Unproject(const TVector3& p, const TMatrix& proj, const TMatrix& view, const TMatrix& world) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		TVector3 result;
		XMStoreFloat3(&result, v);
		return result;
	}

	void TViewport::Unproject(const TVector3& p, const TMatrix& proj, const TMatrix& view, const TMatrix& world, TVector3& result) const
	{
		using namespace DirectX;
		XMVECTOR v = XMLoadFloat3(&p);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		v = XMVector3Unproject(v, x, y, width, height, minDepth, maxDepth, projection, (CXMMATRIX)view, (CXMMATRIX)world);
		XMStoreFloat3(&result, v);
	}
}
