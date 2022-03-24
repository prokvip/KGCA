#include "TMatrix.h"
namespace TMath
{
	TMatrix TMatrix::RotationYawPitchRoll(float yaw, float pitch, float roll)
	{
		TMatrix matY;
		matY.YRotate(yaw);
		TMatrix matX;
		matX.XRotate(pitch);
		TMatrix matZ;
		matZ.ZRotate(roll);
		TMatrix ret = matZ * matX * matY;
		return ret;
	}
}