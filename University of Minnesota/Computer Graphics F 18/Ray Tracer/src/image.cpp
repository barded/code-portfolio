#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <cstdio>

/**
 * Image
 **/
Image::Image (int width_, int height_){

    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];
	int b = 0; //which byte to write to
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
			data.raw[b++] = 0;
		}
	}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){

	width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;

    data.raw = new uint8_t[num_pixels*4];

    //memcpy(data.raw, src.data.raw, num_pixels);
    *data.raw = *src.data.raw;
}

Image::Image (char* fname){

	int numComponents; //(e.g., Y, YA, RGB, or RGBA)
	data.raw = stbi_load(fname, &width, &height, &numComponents, 4);

	if (data.raw == NULL){
		printf("Error loading image: %s", fname);
		exit(-1);
	}

	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;

}

Image::~Image (){
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){

	int lastc = strlen(fname);

	switch (fname[lastc-1]){
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}

void Image::AddNoise (double factor)
{
  int x,y;
  int r=100;

	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
      int u=rand()%(r+r+1)-r;
      int v=rand()%(r+r+1)-r;
      int w=rand()%(r+r+1)-r;
			Pixel p = GetPixel(x, y);
			p.r=ComponentClamp(p.r+(int)u*factor);
      p.g=ComponentClamp(p.g+(int)v*factor);
      p.b=ComponentClamp(p.b+(int)w*factor);
			SetPixel(x,y,p);
		}
	}


}

void Image::Brighten (double factor)
{
	int x,y;
	for (x = 0 ; x < Width() ; x++)
	{
		for (y = 0 ; y < Height() ; y++)
		{
			Pixel p = GetPixel(x, y);
			Pixel scaled_p;
			scaled_p.r = ComponentClamp(factor*p.r);
			scaled_p.g = ComponentClamp(factor*p.g);
			scaled_p.b = ComponentClamp(factor*p.b);
			GetPixel(x,y) = scaled_p;
		}
	}
}

void Image::ChangeContrast (double factor)
{
  int x,y;
  int sum=0;
  int ave;

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      sum+=(int)p.Luminance();
    }
  }

  ave=(int)sum/NumPixels();

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      p.Set(ComponentLerp(ave,p.r,factor),ComponentLerp(ave,p.g,factor),ComponentLerp(ave,p.b,factor));
      SetPixel(x,y,p);
    }
  }

}


void Image::ChangeSaturation(double factor)
{
  int x,y,gray;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      gray=p.Luminance();
      p.Set(ComponentLerp(gray,p.r,factor),ComponentLerp(gray,p.g,factor),ComponentLerp(gray,p.b,factor));
      SetPixel(x,y,p);
    }
  }

}


Image* Image::Crop(int x, int y, int w, int h)
{
  Image* cropImg=new Image(w,h);
  int nx,ny;
  for (nx = x ; nx < x+w ; nx++)
  {
    for (ny = y ; ny < y+h ; ny++)
    {
      Pixel p=GetPixel(nx,ny);
      cropImg->SetPixel(nx-x,ny-y,p);
    }
  }
	return cropImg;
}


void Image::ExtractChannel(int channel)
{
  int x,y;
  if(channel==IMAGE_CHANNEL_RED){                 // red
    for (x = 0 ; x < Width() ; x++)
    {
      for (y = 0 ; y < Height() ; y++)
      {
        Pixel p = GetPixel(x, y);
        p.g=0;
        p.b=0;
        p.a=255;
        SetPixel(x,y,p);
      }
    }
  } else if(channel==IMAGE_CHANNEL_GREEN){          // green
    for (x = 0 ; x < Width() ; x++)
    {
      for (y = 0 ; y < Height() ; y++)
      {
        Pixel p = GetPixel(x, y);
        p.r=0;
        p.b=0;
        p.a=255;
        SetPixel(x,y,p);
      }
    }
  } else if(channel==IMAGE_CHANNEL_BLUE){              //  blue
    for (x = 0 ; x < Width() ; x++)
    {
      for (y = 0 ; y < Height() ; y++)
      {
        Pixel p = GetPixel(x, y);
        p.r=0;
        p.g=0;
        p.a=255;
        SetPixel(x,y,p);
      }
    }
  }
  else if(channel==IMAGE_CHANNEL_ALPHA){              //  blue
    for (x = 0 ; x < Width() ; x++)
    {
      for (y = 0 ; y < Height() ; y++)
      {
        Pixel p = GetPixel(x, y);
        p.r=p.a;
        p.g=p.a;
        p.b=p.a;
        SetPixel(x,y,p);
      }
    }
  }

}


void Image::Quantize (int nbits)
{
  int x,y;
  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      p=PixelQuant(p,nbits);
      SetPixel(x,y,p);
    }
  }

}

void Image::RandomDither (int nbits)
{
  int x,y;
  float r,g,b,a,gray;
  Pixel p;
  float c=pow(2.0,nbits);
  float low,high;
  float logtwo=log(2);
  float t;

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      p=GetPixel(x,y);
      if (p.r==p.g&&p.g==p.b) {
        gray=p.r/255.0;
        low=floor(gray*c)/c;
        high=ceil(gray*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.r=ComponentClamp((int)(((gray<t) ? low : high)*255));
        p.g=p.r;
        p.b=p.r;

        a=p.a/255.0;
        low=floor(a*c)/c;
        high=ceil(a*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.a=ComponentClamp((int)(((a<t) ? low : high)*255));

      } else{
        r=p.r/255.0;
        g=p.g/255.0;
        b=p.b/255.0;
        a=p.a/255.0;

        low=floor(r*c)/c;
        high=ceil(r*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.r=ComponentClamp((int)(((r<t) ? low : high)*255));

        low=floor(g*c)/c;
        high=ceil(g*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.g=ComponentClamp((int)(((g<t) ? low : high)*255));

        low=floor(b*c)/c;
        high=ceil(b*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.b=ComponentClamp((int)(((b<t) ? low : high)*255));

        low=floor(a*c)/c;
        high=ceil(a*c)/c;
        t=(rand()%256)/255.0/c+low;
        p.a=ComponentClamp((int)(((a<t) ? low : high)*255));
      }

      SetPixel(x,y,p);
    }
  }
}


static int Bayer4[4][4] =
{
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};


void Image::OrderedDither(int nbits)
{
	/* WORK HERE */
}

/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
  int x,y;
  double r,g,b,gray;
  Pixel p,a,q;
  float rederr,grerr,berr;
  double alpha,beta,gamma,delta;

  for (y = 0 ; y < Height() ; y++)
  {
    for (x = 0 ; x < Width() ; x++)
    {
      p=GetPixel(x,y);
      q=PixelQuant(p,nbits);
      SetPixel(x,y,q);

      r=q.r/255.0;
      g=q.g/255.0;
      b=q.b/255.0;
      rederr=p.r/255.0-r;
      grerr=p.g/255.0-g;
      berr=p.b/255.0-b;

      if (x==0&&y<Height()-1) {
        alpha=ALPHA/(ALPHA+GAMMA+DELTA);
        gamma=GAMMA/(ALPHA+GAMMA+DELTA);
        delta=DELTA/(ALPHA+GAMMA+DELTA);

        a=GetPixel(x+1,y);
        p.SetClamp(a.r+rederr*alpha,a.g+grerr*alpha,a.b+berr*alpha);
        SetPixel(x+1,y,p);

        a=GetPixel(x,y+1);
        p.SetClamp(a.r+rederr*gamma,a.g+grerr*gamma,a.b+berr*gamma);
        SetPixel(x,y+1,p);

        a=GetPixel(x+1,y+1);
        p.SetClamp(a.r+rederr*delta,a.g+grerr*delta,a.b+berr*delta);
        SetPixel(x+1,y,p);

      } else if(x<Width()-1&&y<Height()-1){ // not border

        a=GetPixel(x+1,y);
        p.SetClamp(a.r+rederr*ALPHA,a.g+grerr*ALPHA,a.b+berr*ALPHA);
        SetPixel(x+1,y,p);

        a=GetPixel(x-1,y+1);
        p.SetClamp(a.r+rederr*BETA,a.g+grerr*BETA,a.b+berr*BETA);
        SetPixel(x-1,y+1,p);

        a=GetPixel(x,y+1);
        p.SetClamp(a.r+rederr*GAMMA,a.g+grerr*GAMMA,a.b+berr*GAMMA);
        SetPixel(x,y+1,p);

        a=GetPixel(x+1,y+1);
        p.SetClamp(a.r+rederr*DELTA,a.g+grerr*DELTA,a.b+berr*DELTA);
        SetPixel(x+1,y,p);


      } else if (x==Width()-1&&y<Height()-1) {  // no right pixel

        beta=BETA/(BETA+GAMMA);
        gamma=GAMMA/(BETA+GAMMA);
        a=GetPixel(x-1,y+1);
        p.SetClamp(a.r+rederr*beta,a.g+grerr*beta,a.b+berr*beta);
        SetPixel(x-1,y+1,p);
        a=GetPixel(x,y+1);
        p.SetClamp(a.r+rederr*gamma,a.g+grerr*gamma,a.b+berr*gamma);
        SetPixel(x,y+1,p);

      } else if (x<Width()-1 &&y==Height()-1) { // no pixels below
        alpha=ALPHA/ALPHA;
        a=GetPixel(x+1,y);
        p.SetClamp(a.r+rederr*alpha,a.g+grerr*alpha,a.b+berr*alpha);
        SetPixel(x+1,y,p);
      }

    }
  }
}

Pixel convolve(Image *a,double b[],int fw, int i, int j, int w, int h){

  int r=fw/2;
  double s=0;
  Pixel ret;
  double  red=0;
  double green=0;
  double blue=0;
  double weight;
  double weightsum=0;
  for(int y=0;y<fw;y++){
    for (int x = 0; x < fw;x++) {
      int row=j-r+y;
      int col=i-r+x;
      if (row>=0&&row<h&&col>=0&&col<w) {
        Pixel p=a->GetPixel(col,row);
        weight = b[y*fw+x];
        weightsum+=weight;
        red+=p.r*weight;
        green+=p.g*weight;
        blue+=p.b*weight;
      }
    }

  }

  ret.SetClamp(red/weightsum,green/weightsum,blue/weightsum);
  return ret;
}

double convolve2(Image *a,double b[],int fw, int i, int j, int w, int h){
// for grayscale edgeDetect
  int r=fw/2;
  double sum=0;
  Pixel ret;
  double weight;

  for(int y=0;y<fw;y++){
    for (int x = 0; x < fw;x++) {
      int row=j-r+y;
      int col=i-r+x;
      if (row>=0&&row<h&&col>=0&&col<w) {
        Pixel p=a->GetPixel(col,row);
        weight = b[y*fw+x];
        double lum=p.r;
        sum+=lum*weight;
      }
    }

  }

  return sum;
}


void Image::Blur(int n)
{
/*  int x,y;
  Image *temp=new Image(Width(),Height()); // orig pixels

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      temp->SetPixel(x,y,p);
    }
  }

  double sigma=2;
  double filter[n*n];   // y*n+x
  double mid=n/2.0;
  for (x = 0 ; x < n ; x++)    // filter values
  {
    for (y = 0 ; y < n ; y++)
    {
      filter[y*n+x]=exp( -0.5 * (pow((x-mid)/sigma, 2.0) + pow((y-mid)/sigma,2.0)) )/ (2 * M_PI * sigma * sigma);
    }
  }

  for (y = 0 ; y < Height() ; y++)
  {
    for (x = 0 ; x < Width() ; x++)
    {
      Pixel p=convolve(temp,filter,n,x,y,Width(),Height());
      SetPixel(x,y,p);
    }
  }
  delete temp;
  */
}

void Image::Sharpen(int n)
{
  int x,y;
  Image *temp=new Image(Width(),Height()); // orig pixels

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      temp->SetPixel(x,y,p);
    }
  }

  Blur(n);

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      Pixel q=temp->GetPixel(x,y);
      SetPixel(x,y,PixelLerp(p,q,2));
    }
  }

}

void Image::EdgeDetect()
{
  int x,y,gray;

  ChangeSaturation(0);
  Blur(5);


  Image *temp=new Image(Width(),Height()); // orig pixels

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      Pixel p=GetPixel(x,y);
      temp->SetPixel(x,y,p);
    }
  }

  int n=3;
  double filterx[]={-1,0,1,-2,0,2,-1,0,1};  // sobel filters
  double filtery[]={-1,-2,-1,0,0,0,1,2,1};

  for (y = 0 ; y < Height() ; y++)
  {
    for (x = 0 ; x < Width() ; x++)
    {
      double xval=convolve2(temp,filterx,n,x,y,Width(),Height());
      double yval=convolve2(temp,filtery,n,x,y,Width(),Height());
      int tot=(int)sqrt(xval*xval+yval*yval);
      Component c=ComponentClamp(tot);
      Pixel p;
      p.Set(c,c,c);
      SetPixel(x,y,p);
    }
  }

  delete temp;
}

Image* Image::Scale(double sx, double sy)
{
    Image *newimg=new Image(sx*Width(),sy*Height());
    int x,y;

    for(y=0; y<newimg->Height(); y++){
      for(x=0; x<newimg->Width(); x++){
        double u=x/sx;
        double v=y/sy;
        newimg->SetPixel(x,y,Sample(u,v));
      }
    }

	return newimg;
}

Image* Image::Rotate(double angle)
{

  angle=M_PI/180.0*angle;
  double angle2=angle;

  if ((angle > M_PI/2 && angle < M_PI ) || (angle > 3.0/2 * M_PI && angle < 2.0 * M_PI)){
    angle2=M_PI-angle;
  }

  int neww=fabs(Width()*cos(angle2)+Height()*sin(angle2));
  int newh=fabs(Width()*sin(angle2)+Height()*cos(angle2));

  Image *newimg=new Image(neww,newh);
  int x,y;
  double xmid=(double)newimg->Width()/2;
  double ymid=(double)newimg->Height()/2;
  double xmidOld=(double)Width()/2;
  double ymidOld=(double)Height()/2;
  Pixel p;


  for(y=0; y<newimg->Height(); y++){
    for(x=0; x<newimg->Width(); x++){
      double u=xmidOld+(x-xmid)*cos(-angle)-(y-ymid)*sin(-angle);
      double v=ymidOld+(x-xmid)*sin(-angle)+(y-ymid)*cos(-angle);

      if (u<0||u>=Width()||v<0||v>=Height()) {
        p.Set(255,255,255);           // white
        newimg->SetPixel(x,y,p);
      } else {
      newimg->SetPixel(x,y,Sample(u,v));
      }
    }
  }
	return newimg;
}

void Image::Fun()
{
  Image *newimg=new Image(Width(),Height());
  int x,y;
  double xmid=(double)newimg->Width()/2;
  double ymid=(double)newimg->Height()/2;
  double angle;
  double angleO=M_PI;
  Pixel p;

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      p=GetPixel(x,y);
      newimg->SetPixel(x,y,p);
    }
  }

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      // angle=angleO*sqrt((x-xmid)*(x-xmid)+(y-ymid)*(y-ymid))/sqrt(xmid*xmid+ymid*ymid); // swirl
      angle=angleO*y/(x-xmid);
      double u=xmid+(x-xmid)*cos(-angle)-(y-ymid)*sin(-angle);
      angle=angleO*x/(y-ymid);
      double v=ymid+(x-xmid)*sin(-angle)+(y-ymid)*cos(-angle);

      if (u<0||u>=Width()||v<0||v>=Height()) {
        p.Set(255,255,255);
        newimg->SetPixel(x,y,p);
      } else {
      newimg->SetPixel(x,y,Sample(u,v));
      }
    }
  }

  for (x = 0 ; x < Width() ; x++)
  {
    for (y = 0 ; y < Height() ; y++)
    {
      p=newimg->GetPixel(x,y);
      SetPixel(x,y,p);
    }
  }
    delete newimg;
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}


Pixel Image::Sample (double u, double v){


  if (SampleMethod()==IMAGE_SAMPLING_POINT) {
    int x=lround(u);
    int y=lround(v);
    Pixel p;

    x=(x<0) ? 0 : x;
    x=(x>=Width()) ? Width()-1 : x;
    y=(y<0) ? 0 : y;
    y=(y>=Height()) ? Height()-1 : y;

    p=GetPixel(x,y);

    return p;

  } else if (SampleMethod()==IMAGE_SAMPLING_BILINEAR) {

    int xl=(floor(u)<0) ? 0: floor(u);
    int xh=(ceil(u)>=Width()) ? Width()-1 : ceil(u);
    int yl=(floor(v)<0) ? 0 : floor(v);
    int yh=(ceil(v)>=Height()) ? Height()-1 : ceil(v);

    Pixel p=PixelLerp(GetPixel(xl,yl),GetPixel(xh,yl),u-xl);
    Pixel q=PixelLerp(GetPixel(xl,yh),GetPixel(xh,yh),u-xl);
    Pixel c=PixelLerp(p,q,v-yl);

    return c;

  } else if (SampleMethod()==IMAGE_SAMPLING_GAUSSIAN) {
    double sigma=.5;
    int r=4;
    int x,y;
    double  red=0;
    double green=0;
    double blue=0;
    double weight;
    double weightsum=0;
    Pixel p;
    int ylow=lround(v-r);
    int yhigh=lround(v+r);
    int xlow=lround(u-r);
    int xhigh=lround(u+r);

    for (y = ylow ; y <=yhigh ; y++)
    {
      for (x = xlow ; x<=xhigh ; x++)
      {
        if (x>=0&&x<Width()&&y>=0&&y<Height()) {
          p=GetPixel(x,y);

          weight=exp( -0.5 * (pow((x-u)/sigma, 2.0) + pow((y-v)/sigma,2.0)) )/ (2 * M_PI * sigma * sigma);
          weightsum+=weight;
          red+=p.r*weight;
          green+=p.g*weight;
          blue+=p.b*weight;
        }

      }
    }

    p.SetClamp(red/weightsum,green/weightsum,blue/weightsum);  // normalize by weightsum
    return p;


  }
  else{
    return Pixel(0,0,0);

  }


}
