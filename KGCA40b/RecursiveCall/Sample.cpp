#include <iostream>
using namespace std;
int Factorial(int iValue)
{
	if (iValue == 1) return 1;
	int iRet = Factorial(iValue-1);
	return iValue * iRet;
}
void main()
{
	// ÆÑÅä¸®¾ó 5! = 5*4*3*2*1! = 120
	int iSum = 1;
	for (int i = 1; i <= 4; i++)
	{
		iSum = iSum * i;
	}
	cout << iSum << endl;
	cout << Factorial(20);
}