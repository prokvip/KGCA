#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int iValue = 50;
	float fValue = 3.141592f;
	FILE* fp = fopen("demo.txt", "w");
	{
		fprintf(fp, "%s ", "============ KGCA ==========");
		fprintf(fp, "\n%s ", "홍길동");
		fprintf(fp, "%d %f", iValue, fValue);
		fclose(fp);
	}
	iValue = 99;
	fValue = 6.26f;
	fp = fopen("demo.txt", "a");
	{
		fprintf(fp, "\n%s ", "개통이");
		fprintf(fp, "%d %f", iValue, fValue);
		fclose(fp);
	}

	char buffer[256] = { 0, };
	int iValueRead;
	float fValueRead;

	FILE* fpRead = fopen("demo.txt", "r");
	{
		fgets(buffer, 256, fpRead);
		fscanf(fpRead, "%s %d %f", buffer, &iValueRead, &fValueRead);
		printf("\n%s %d %f", buffer, iValueRead, fValueRead);
		fscanf(fpRead, "%s %d %f", buffer, &iValueRead, &fValueRead);
		printf("\n%s %d %f", buffer, iValueRead, fValueRead);
		fclose(fpRead);
	}

	return 0;
}

