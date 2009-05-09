#include "Base.h"

#ifndef _IMAGE_
#define _IMAGE_


class Image
{	
protected:
	int w;
	int h;
	
	double *pixels1;
	double *pixels2;
	double *pixels3;

	int PixelPos(int x, int y, int w);
	//void PutPixels(TSG_Pixel* source, TSG_Pixel* target, int length);
	//void CopyPixels(TSG_Pixel* source, TSG_Pixel* target, int length);
	//void CopyPixelsBW(TSG_Pixel* source, TSG_Pixel* target, int length);
public:
	//TSG_Image(int w, int h, void* pixels = NULL);
	//TSG_Image(const TSG_Image* Image);
	//TSG_Image(); // NULL-image
	Image(char* filename);
	~Image();

	int Width();
	int Height();
	bool Inside(int x, int y);

	int Size();
	//void DrawToImageQ(const int &srcx, const int &srcy, const int &srcw, const int &srch,
	//	     	TSG_Image* trgimage, const int &trgx, const int &trgy);
	//void DrawToImage(TSG_Image* trgimage, const int &trgx, const int &trgy);
	//void DrawToImageQ(TSG_Image* trgimage, const int &trgx, const int &trgy);
	//void Scroll(int x, int y);
	//void Clear(char r, char g, char b, char t);
	double getValue1(int x, int y);
	double getValue2(int x, int y);
	double getValue3(int x, int y);
};



// METHODS


// -----------------  protected methods --------------------

inline int Image::PixelPos(int x, int y, int w)
{
	return (x+w*y);
}
/*
void TSG_Image::PutPixels(TSG_Pixel* source, TSG_Pixel* target, int length)
{
	for(int i=0;i<length;i++)
	{
		(target+i)->Get(*(source+i));
	}
}
void TSG_Image::CopyPixels(TSG_Pixel* source, TSG_Pixel* target, int length)
{
	for(int i=0;i<length;i++)
	{
		*(target+i) = *(source+i);
	}
}
void TSG_Image::CopyPixelsBW(TSG_Pixel* source, TSG_Pixel* target, int length)
{
	for(int i=length-1;i>=0;i=i-1)
	{
		*(target+i) = *(source+i);
	}
}
*/
// -----------------  constructor methods --------------------
/*
TSG_Image::TSG_Image(int w, int h, void* pixels) : TSG_Rect(w, h)
{
	this->pixels = (TSG_Pixel*)pixels;
	if (pixels == NULL) this->pixels = new TSG_Pixel[w*h];
}
TSG_Image::TSG_Image(const TSG_Image* Image) : TSG_Rect(Image->w, Image->h)
{
	this->pixels = new TSG_Pixel[w*h];
	CopyPixels(Image->pixels, this->pixels, w*h);
}
TSG_Image::TSG_Image() : TSG_Rect(0, 0)
{
	this->pixels = NULL;
}
*/
// LoadBmp
Image::Image(char* filename)
{
	long in;
	int i;

	std::ifstream ImageFile;
	
	if(filename != NULL) ImageFile.open((const char*)filename, std::ios::binary);

	//ifstream ImageFile((const char*)filename,ios::binary);

	//double* pixels = NULL;

	for(i=0;i<18;i++) {
		ImageFile.read((char*)&in,sizeof(char));
	}

	// read width
	ImageFile.read((char*)&w,sizeof(int));
	// read height
	ImageFile.read((char*)&h,sizeof(int));

	for(i=0;i<28;i++) {
		ImageFile.read((char*)&in,sizeof(char));
	}
	
	pixels1 = new double[w*h];
	pixels2 = new double[w*h];
	pixels3 = new double[w*h];

	unsigned char v1, v2, v3;
	
	//ImageFile.read((char*)&v1,sizeof(char));
	//ImageFile.read((char*)&v1,sizeof(char));
	//ImageFile.read((char*)&v1,sizeof(char));

	int x;
	for(int y=0;y<h;y=y+1)
	{
		//ImageFile.read((char*)&v1,sizeof(char));
		//ImageFile.read((char*)&v1,sizeof(char));
		

		for(x=0;x<w;x=x+1)
		{
			i = (y*w+x);
			// In a bitmap file: BGR
			// Reading to memory as: RGB
			ImageFile.read((char*)&v2,sizeof(char));
			ImageFile.read((char*)&v3,sizeof(char));
			ImageFile.read((char*)&v1,sizeof(char));
			
			//cout << (unsigned int)v1 << "  " << (unsigned int)v2 << "  " << (unsigned int)v3 << endl;

			*(pixels1+i) = ((double)v1/256.0);
			*(pixels2+i) = ((double)v2/256.0);
			*(pixels3+i) = ((double)v3/256.0);

			//cout << *(pixels+i) << "  ";
			
			//ImageFile.read((char*)(pixels+i+3),sizeof(char));
			//if ((*(pixels+i+0) == 255) && (*(pixels+i+1) == 255) && (*(pixels+i+2) == 255)) *(pixels+i+3) = 255;
			//else *(pixels+i+3) = 0;
		}
		//ImageFile.read((char*)&v1,sizeof(char));
			//ImageFile.read((char*)&v2,sizeof(char));
			//ImageFile.read((char*)&v3,sizeof(char));
	}
	
	ImageFile.close();
	//this->pixels = (TSG_Pixel*)pixels;

}

// destructor method
Image::~Image()
{
	delete pixels1;
	delete pixels2;
	delete pixels3;
}


// ---------------------  Public methods  ------------------------------

inline int Image::Width()
{
	return w;
}
inline int Image::Height()
{
	return h;
}

inline bool Image::Inside(int x, int y)
{
	return (Between(x,0,w) && Between(y,0,h));
}

inline int Image::Size()
{
	return w*h;
}
/*
void TSG_Image::DrawToImageQ(const int &srcx, const int &srcy, const int &srcw, const int &srch,
			TSG_Image* trgimage, const int &trgx, const int &trgy) // can only handle correct areas
{
	for(int row=0;row<srch;row++)
	{
		PutPixels(this->pixels+PixelPos(srcx,srcy+row,this->w),trgimage->pixels+PixelPos(trgx,trgy+row,trgimage->w),srcw);
	}
}
void TSG_Image::DrawToImage(TSG_Image* trgimage, const int &trgx, const int &trgy)
{
	bool draw = true; // boolean to store if draw should be done or not
	int srcx, srcw; // sourcex (the xposition in this image where to draw from)
	int srcy, srch;

	// ----------- Check X --------------
	if (trgx < 0) //Left corner is left of inside
	{
		if (trgx+this->w < 0) //right corner is left of inside
		{
			draw = false;
		}
		else if (trgx+this->w > trgimage->w) //Right corner is right of inside
		{
			srcx = trgx*(-1);
			srcw = trgimage->w;
		}
		else // Right corner is inside
		{
			srcx = trgx*(-1);
			srcw = this->w-srcx;
		}
	}
	else if (trgx > trgimage->w) // Left corner is right of inside
	{
		draw = false;
	}
	else	// Left corner is inside
	{
		if (trgx+this->w > trgimage->w) // right corner is right of inside
		{
			srcx = 0;
			srcw = trgimage->w-trgx;
		}
		else	//right corner is inside
		{
			srcx = 0;
			srcw = this->w;
		}
	}	
	
	// ---------- Check Y ---------------
	if (trgy < 0) //Bottom is below
	{
		if (trgy+this->h < 0) //top is below
		{
			draw = false;
		}
		else if (trgy+this->h > trgimage->h) //top is above
		{
			srcy = trgy*(-1);
			srch = trgimage->h;
		}
		else // top is inside
		{
			srcy = trgy*(-1);
			srcw = this->h-srcy;
		}
	}
	else if (trgy > trgimage->h) // bottom is above
	{
		draw = false;
	}
	else	// bottom is inside
	{
		if (trgy+this->h > trgimage->h) // top is above
		{
			srcy = 0;
			srch = trgimage->h-trgy;
		}
		else	//top is inside
		{
			srcy = 0;
			srch = this->h;
		}
	}	


	if (draw) DrawToImageQ(srcx,srcy,srcw,srch,trgimage,trgx+srcx,trgy+srcy);
}
void TSG_Image::DrawToImageQ(TSG_Image* trgimage, const int &trgx, const int &trgy)
{
	for(int row=0;row<this->h;row++)
	{
		PutPixels(this->pixels+PixelPos(0,row,this->w),trgimage->pixels+PixelPos(trgx,trgy+row,trgimage->w),this->w);
	}
}

void TSG_Image::Scroll(int x, int y)
{
	int srcx, srcw;
	int srcy, srch;
	int trgx, trgy;
	int row;

	if (x<0)
	{
		//scroll left
		srcx = x*(-1);
		srcw = this->w-srcx;
		trgx = 0;
		if (y<0)
		{
			//scroll down
			srcy = y*(-1);
			srch = this->h-srcy;
			trgy = 0;
			for(row=0;row<srch;row++)
			{
				CopyPixels(this->pixels+PixelPos(srcx,srcy+row,this->w),this->pixels+PixelPos(trgx,trgy+row,this->w),srcw);
			}
		}
		else
		{
			//scroll up
			srcy = 0;
			srch = this->h-y;
			trgy = y;
			for(row=srch-1;row>=0;row=row-1)
			{
				CopyPixels(this->pixels+PixelPos(srcx,srcy+row,this->w),this->pixels+PixelPos(trgx,trgy+row,this->w),srcw);
			}
		}
	}
	else	
	{
		//scroll right
		srcx = 0;
		srcw = this->w-x;
		trgx = x;
		if (y<0)
		{
			//scroll down
			srcy = y*(-1);
			srch = this->h-srcy;
			trgy = 0;
			for(row=0;row<srch;row++)
			{
				CopyPixelsBW(this->pixels+PixelPos(srcx,srcy+row,this->w),this->pixels+PixelPos(trgx,trgy+row,this->w),srcw);
			}
		}
		else
		{
			//scroll up
			srcy = 0;
			srch = this->h-y;
			trgy = y;
			for(row=srch-1;row>=0;row=row-1)
			{
				CopyPixelsBW(this->pixels+PixelPos(srcx,srcy+row,this->w),this->pixels+PixelPos(trgx,trgy+row,this->w),srcw);
			}
		}
	}
}

void TSG_Image::Clear(char r, char g, char b, char t)
{
	int size = Size();
	for (int i=0;i<size;i++)
	{
		(pixels+i)->red = r;
		(pixels+i+1)->blue = g;
		(pixels+i+2)->green = b;
		(pixels+i+3)->transp = t;
	}
}
*/

inline double Image::getValue1(int x, int y)
{
	return *(pixels1+PixelPos(x,y,w));
}

inline double Image::getValue2(int x, int y)
{
	return *(pixels2+PixelPos(x,y,w));
}

inline double Image::getValue3(int x, int y)
{
	return *(pixels3+PixelPos(x,y,w));
}




#endif

