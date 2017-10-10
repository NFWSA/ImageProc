#include <iostream>
#include <fstream>
#include "bmpFile.h"

using namespace std;

int main()
{
	BmpFile pic;

	pic.setBitSize(8);
	pic.setImageSize(2, 16);

	unsigned char bits[] = {
		0, 8, //0, 0,
		16, 24, //0, 0,
		32, 40, //0, 0,
		48, 56, //0, 0,
		64, 72, //0, 0,
		80, 88, //0, 0,
		96, 104, //0, 0,
		112, 120, //0, 0,
		128, 136, //0, 0,
		144, 152, //0, 0,
		160, 168, //0, 0,
		176, 184, //0, 0,
		192, 200, //0, 0,
		208, 216, //0, 0,
		234, 242, //0, 0,
		250, 255//, 0, 0
	};

	pic.setImageData(bits);
	ofstream fout("hello.bmp", ios::binary);
	fout << pic;

	fout.close();
	return 0;
}
