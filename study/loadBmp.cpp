#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "windows.h"
#include "bmpFile.h"

using namespace std;

void imageData(const unsigned char *bits, int width, int height, int bit = 1)
{
	int sum = width * height * bit / 8 + (width * bit % 32==0 ? 0 : (32 - width * bit % 32) / 8 * height);
	int w = sum / height, h = height;
	sum -= w;
	cout << "图片数据区域：" << endl;
	if(16==bit){

	}
	if(bit<16){
		for(int i=0; i<h; ++i){
			cout << setw(8) << i << ": ";
			for(int j=0; j<w; ++j){
				cout << setfill('0') << hex << setw(2) << static_cast<short>(bits[sum-i*w+j]) << dec << "  " << setfill(' ');
			}
			cout << dec << endl;
		}
	}
	if(bit>16){
		for(int i=0; i<h; ++i){
			cout << setw(8) << i << ": ";
			for(int j=0; j<w; j+=3){
				cout << setfill('0') << hex << setw(2) << static_cast<short>(bits[sum-i*w+j+2])
					 << setw(2) << static_cast<short>(bits[sum-i*w+j+1]) << setw(2) << static_cast<short>(bits[sum-i*w+j])
					 << dec << " " << setfill(' ');
			}
			cout << dec << endl;
		}
	}
}

int main()
{
	int bitNum = 1, clrNum = 2;
	BITMAPFILEHEADER bmfileHeader;
	BITMAPINFOHEADER bminfoHeader;
	string filename;
	BmpFile file;
	cout << "输入文件名(.bmp)：";
	cin >> filename;
	ifstream fin((filename + ".bmp").c_str(), ios::binary);
	if(!fin){
		cerr << "文件不存在！" << endl;
		return -1;
	}
	//fin >> file;
	file.fileHeader(cout);
	file.infoHeader(cout);
	file.setBitSize(2);
	file.colorTable(cout);


	bitNum = bminfoHeader.biBitCount;
	clrNum = bminfoHeader.biClrUsed;

/*	RGBQUAD *bmpColor = new RGBQUAD[clrNum];
	int bitsNum = bitNum * bminfoHeader.biWidth * bminfoHeader.biHeight / 8 + (bminfoHeader.biWidth * bitNum % 32==0 ? 0 : (32 - bminfoHeader.biWidth * bitNum % 32) / 8 * bminfoHeader.biHeight);
	unsigned char *bits = new unsigned char[bitsNum];

	fin.read(reinterpret_cast<char*>(bits), bitsNum);
	fin.close();
	imageData(bits, bminfoHeader.biWidth, bminfoHeader.biHeight, bitNum);
	*/
	return 0;
}
