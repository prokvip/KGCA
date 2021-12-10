#include "TStudentManager.h"
const int GetData1()
{
	int iValue = 4;
	return iValue;
}
const int *  GetData2()
{
	int iValue = 4;
	return &iValue;
}

const int* const  GetData3()
{
	int iValue = 4;
	return &iValue;
}
int* const  GetData4()
{
	int iValue = 4;
	return &iValue;
}
// mutable, frined, static_cast<> 형변환
void main(int argc, char* argv[])
{
	int iRet1 = GetData1();
	int iRet2 = 4;
	const int* pRet = GetData2();
	//*pRet = 88; // error
	// int iRet3 = GetData3();
	const int* iRet3 = GetData3();
	//*iRet3 = 9; // error
	iRet3 = &iRet2;
	const int* const iRet4 = GetData3();
	//*iRet4 = 9; // error
	//iRet4 = &iRet2;// error
	const int* iRet5 = GetData4();
	//*iRet5 = 9; // error
	iRet5 = &iRet2;// error
	int* const iRet6 = GetData4();
	*iRet6 = 9; // error
	//iRet6 = &iRet2;// error

	int data1 = 1;
	int data2 = 4;	
	const int* dataConst1 = &data1;
	const int const* dataConst3 = &data1;
	int * const dataConst4 = &data1;
	//// 데이터 
	//*dataConst1 = 4; // error
	//*dataConst3 = 4; // error
	*dataConst4 = 4;
	// 주소
	dataConst1 = &data2;
	int * const dataConst2 = &data1;
	*dataConst2 = 7;
	//dataConst2 = &data2;// error

	const int* const dataConst9 = &data1;
	//*dataConst9 = 7; // error
	//dataConst9 = &data2; // error

	TStudentManager mgr;
	int iSelect = 0;	
	while (iSelect < 99 )
	{
		int iSelect = 0;
		printf("\n->Create(0),Save(1),Delete(2),Load(3),Draw(4),Quit:9 : ");
		scanf_s("%d", &iSelect);
		if (iSelect == 9) break;

		switch (iSelect)
		{
		case 0: {
			mgr.Create();
			iSelect = 1;
		}break;
		case 1: {
			mgr.FileSave();
		}break;
		case 2: {
			mgr.DeleteAll();
		}break;
		case 3: {
			mgr.DeleteAll();
			mgr.Load();
		}break;
		case 4: {
			mgr.Draw();
		}break;
		}
	}

	
}
