#ifndef BmpFile_NFWSA
#define BmpFile_NFWSA

#include "windows.h"
#include <iostream>

class BmpFile
//位图处理类
//读入位图类型：全部
//定制位图类型：1位图、8位灰度图、24、32位全彩色图
{
public:
    friend std::istream& operator>>(std::istream &in, BmpFile &file);
    friend std::ostream& operator<<(std::ostream &out, BmpFile &file);

    const bool hasImage() const;

    void setImageSize(const int w, const int h);
    void setBitSize(const int bitNum);
    void setImageData(const byte *data);

    std::ostream& fileHeader(std::ostream &out) const;
    std::ostream& infoHeader(std::ostream &out) const;
    std::ostream& colorTable(std::ostream &out) const;
    std::ostream& imageData(std::ostream &out) const;

    const int getWidth() const;
    const int getRealWidth() const;
    const int getHeight() const;

    BmpFile();
    ~BmpFile();

private:
    typedef unsigned char byte;
    bool _hasImage;
    BITMAPFILEHEADER _file_header;
    BITMAPINFOHEADER _info_header;
    RGBQUAD *_colors;
    byte *_data;
};

#endif //BmpFile_NFWSA
