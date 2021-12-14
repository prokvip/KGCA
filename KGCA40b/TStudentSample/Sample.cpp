#include "TStudentManager.h"
enum Job {
	Create,Save,Delete,Load,Draw,Quit,
};
// mutable, frined, static_cast<> Çüº¯È¯
void main(int argc, char* argv[])
{		
	/*TStudent* data1 = new TMiddleStudent;
	*data1 << 1 << 4 << 5.4f;*/

	/*TMiddleStudent* pA = new TMiddleStudent;
	THighStudent* pB = new THighStudent;
	TCollegeStudent* pC = new TCollegeStudent;
	TStudent* data[3] = { 0, };
	data[0] = pA;
	data[1] = pB;
	data[2] = pC;
	for (int i = 0; i < 3; i++)
	{
		data[i]->Show();
	}
	for (int i = 0; i < 3; i++)
	{
		delete data[i];
	}	*/

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
		case Create: {
			mgr.Create();
			iSelect = 1;
		}break;
		case Save: {
			mgr.FileSave("save.txt");
		}break;
		case Delete: {
			mgr.DeleteAll();
		}break;
		case Load: {
			mgr.DeleteAll();
			mgr.Load("save.txt");
		}break;
		case Draw: {
			std::cout << mgr;
		}break;
		}
	}

	
}
