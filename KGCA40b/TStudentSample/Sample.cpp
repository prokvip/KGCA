#include "TStudentManager.h"
// mutable, frined, static_cast<> Çüº¯È¯
void main(int argc, char* argv[])
{	
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
