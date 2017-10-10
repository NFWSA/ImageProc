#include "bmpFile.h"
#include <iostream>
#include <iomanip>

long vex2(long n)
{
	long rlt = 1;
	for(int i=0; i<n; ++i)
		rlt *= 2;
	return rlt;
}

const bool BmpFile::hasImage() const
{
    return _hasImage;
}

std::istream& operator>>(std::istream &fin, BmpFile &file)
{
    if(file._hasImage){
        std::cerr << "bmpFile class has already image data." << std::endl;
        return fin;
    }
    file._hasImage = true;

    fin.read(reinterpret_cast<char*>(&file._file_header), sizeof(file._file_header));
    fin.read(reinterpret_cast<char*>(&file._info_header), sizeof(file._info_header));

    long bitNum = file._info_header.biBitCount;
    long clrNum = file._info_header.biClrUsed;
    if( bitNum<16 && 0==clrNum )
        clrNum = vex2(bitNum);

    file._colors = new RGBQUAD[clrNum];
    long bitsNum = bitNum * file._info_header.biWidth * file._info_header.biHeight / 8 +
					(bitNum * file._info_header.biWidth * file._info_header.biHeight % 8 == 0) ? 0 : 1 +
                    (file._info_header.biWidth * bitNum % 32==0 ? 0 : (32 - file._info_header.biWidth * bitNum % 32) / 8 * file._info_header.biHeight);
    file._data = new byte[bitsNum];
    for(long i=0; i<clrNum; ++i)
        fin.read(reinterpret_cast<char*>(&(file._colors[i])), sizeof(RGBQUAD));

    fin.read(reinterpret_cast<char*>(file._data), bitsNum);
    return fin;
}

std::ostream& operator<<(std::ostream &out, BmpFile &file)
{
	int bitNum = file._info_header.biBitCount, clrNum = (bitNum<16) ? vex2(bitNum) : 0;
    int bitsNum = bitNum * file._info_header.biWidth * file._info_header.biHeight / 8 +
					( (bitNum * file._info_header.biWidth * file._info_header.biHeight % 8 == 0) ? 0 : 1 ) +
                    (file._info_header.biWidth * bitNum % 32==0 ? 0 : (32 - file._info_header.biWidth * bitNum % 32) / 8 * file._info_header.biHeight);
	out.write(reinterpret_cast<const char*>(&(file._file_header)), sizeof(file._file_header));
	out.write(reinterpret_cast<const char*>(&(file._info_header)), sizeof(file._info_header));
	if(clrNum!=0)
		out.write(reinterpret_cast<const char*>(file._colors), sizeof(RGBQUAD) * clrNum);
	out.write(reinterpret_cast<const char*>(file._data), bitsNum);
	return out;
}

std::ostream& BmpFile::fileHeader(std::ostream &out) const
{
	out << "bmpFileHeader部分：" << std::endl;
	out << "\t标志头：" << std::hex << _file_header.bfType << std::dec << std::endl;
	out << "\t保留字：" << std::hex << _file_header.bfReserved1 << " " << _file_header.bfReserved2 << std::dec << std::endl;
	out << "\t数据区偏移量：" << _file_header.bfOffBits << std::endl;
	out << "\t文件大小：" << _file_header.bfSize << std::endl;
    return out;
}

std::ostream& BmpFile::infoHeader(std::ostream &out) const
{
	out << "bmpInfoHeader部分：" << std::endl;
	out << "\t信息头大小：" << _info_header.biSize << std::endl;
	out << "\t图片大小：" << _info_header.biWidth << " x " << _info_header.biHeight << std::endl;
	out << "\t平面数：" << _info_header.biPlanes << std::endl;
	out << "\t位图位数：" << _info_header.biBitCount << std::endl;
	out << "\t压缩方式：" << _info_header.biCompression << std::endl;
	out << "\t图片数据大小：" << _info_header.biSizeImage<< std::endl;
	out << "\tX/Y分辨率：" << _info_header.biXPelsPerMeter << " x " << _info_header.biYPelsPerMeter << std::endl;
	out << "\t使用颜色数量：" << _info_header.biClrUsed << std::endl;
	out << "\t重要颜色量：" << _info_header.biClrImportant << std::endl;
    return out;
}


std::ostream& BmpFile::colorTable(std::ostream &out) const
{
    long bitNum = _info_header.biBitCount;
    long clrNum = _info_header.biClrUsed;
    if( bitNum<16 && bitNum>0 && 0==clrNum )
        clrNum = vex2(bitNum);
    for(long i=0; i<clrNum; ++i){
        out << "调色表序号(dec / hex)：" << std::setw(3) << i+1 << " / " << std::hex << std::setw(2) << i+1 << std::dec << std::endl << std::setfill('0');
    	out << "\t颜色值(RGB)： 0x" << std::hex << std::setw(2) << static_cast<short>(_colors[i].rgbRed) << std::setw(2) << static_cast<short>(_colors[i].rgbGreen)
    													 << std::setw(2) << static_cast<short>(_colors[i].rgbBlue) << std::dec << std::endl;
    	out << "\t保留值：" << static_cast<short>(_colors[i].rgbReserved) << std::endl << std::setfill(' ');
    }
    return out;
}
std::ostream& BmpFile::imageData(std::ostream &out) const
{
    return out;
}

void BmpFile::setImageSize(const int w, const int h)
{
    if(_hasImage){
        std::cerr << "bmpFile class has already image data." << std::endl;
        return;
    }
    _info_header.biWidth = w;
    _info_header.biHeight = h;
}

void BmpFile::setBitSize(const int bitNum)
{
    if(_hasImage){
        std::cerr << "bmpFile class has already image data." << std::endl;
        return;
    }
    if(_colors!=nullptr){
        std::cerr << "bmpFile class has already color table data." << std::endl;
        return;
    }
    _info_header.biBitCount = bitNum;
    long clrNum = (bitNum<16) ? vex2(bitNum) : 0;
    _file_header.bfOffBits += sizeof(RGBQUAD) * clrNum;
    _colors = new RGBQUAD[clrNum];
    for(int i=0; i<clrNum; ++i){
        _colors[i].rgbReserved = 0;
        _colors[i].rgbRed = i * 255 / (clrNum - 1);
        _colors[i].rgbGreen = i * 255 / (clrNum - 1);
        _colors[i].rgbBlue = i * 255 / (clrNum - 1);
    }
}

void BmpFile::setImageData(const byte *data)
{
    if(_hasImage){
        std::cerr << "bmpFile class has already image data." << std::endl;
        return;
    }
	int bitNum = _info_header.biBitCount;
    int bitsNum = bitNum * _info_header.biWidth * _info_header.biHeight / 8 +
					( (bitNum * _info_header.biWidth * _info_header.biHeight % 8 == 0) ? 0 : 1 ) +
                    (_info_header.biWidth * bitNum % 32==0 ? 0 : (32 - _info_header.biWidth * bitNum % 32) / 8 * _info_header.biHeight);
	int w = bitNum * _info_header.biWidth / 8 +
					( (bitNum * _info_header.biWidth % 8 == 0) ? 0 : 1 ),
		h = _info_header.biHeight;
	int width = w + (_info_header.biWidth * bitNum % 32==0 ? 0 : (32 - _info_header.biWidth * bitNum % 32) / 8);
	_data = new byte[bitsNum];
	for(int i=0; i<h; ++i){
		for(int j=0; j<w; ++j)
			_data[i*width+j] = data[i*w+j];
		for(int j=w; j<width; ++j)
			_data[i*width+j] = 0;
	}
	_info_header.biSizeImage = bitsNum;
	_file_header.bfSize = _file_header.bfOffBits + bitsNum;
	return;
}

const int BmpFile::getWidth() const
{
	return _info_header.biBitCount * _info_header.biWidth / 8 +
			( (_info_header.biBitCount * _info_header.biWidth % 8 == 0) ? 0 : 1 );
}

const int BmpFile::getRealWidth() const
{
	return _info_header.biBitCount * _info_header.biWidth / 8 +
			( (_info_header.biBitCount * _info_header.biWidth % 8 == 0) ? 0 : 1 ) +
			(_info_header.biWidth * _info_header.biBitCount % 32==0 ? 0 : (32 - _info_header.biWidth * _info_header.biBitCount % 32) / 8);
}

const int BmpFile::getHeight() const
{
	return _info_header.biHeight;
}

BmpFile::BmpFile() :
    _colors(nullptr), _data(nullptr), _hasImage(false)
{
    _file_header.bfType = 0x4D42;
    _file_header.bfReserved1 = _file_header.bfReserved2 = 0;
    _file_header.bfOffBits = sizeof(_file_header) + sizeof(_info_header);

	_info_header.biSize = sizeof(_info_header);
    _info_header.biWidth = _info_header.biHeight = 0;
	_info_header.biPlanes = 1;
    _info_header.biBitCount = 0;
	_info_header.biCompression = 0;
    //todo: _info_header.biSizeImage = 0;
	_info_header.biXPelsPerMeter = _info_header.biYPelsPerMeter = 3779;
	_info_header.biClrUsed = _info_header.biClrImportant = 0;

}

BmpFile::~BmpFile()
{
    if(!_hasImage)
        return;
    delete[] _colors;
    delete[] _data;
}
