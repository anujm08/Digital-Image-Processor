#include "Image.h"
#include<gtkmm.h>
#include<iostream>
#include<cmath>
#include<cstring>
#include <stdlib.h>
void convert(char*A,int*B,int len);
int** createintarray (int height,int width);
float** createfloatarray (int height,int width);
void deletearray(int **a,int height);
void deletearray(float **a,int height);

Image::Image()//ctor
{
    width=0;
    height=0;
    size=0;
    index=0;
    is_save=true;
    no_undo=0;
    R=NULL;
    G=NULL;
    B=NULL;
    for(int i=0;i<100;i++)
    {
        process[i][0]=0;
        process[i][1]=0;
    }
}

Image::~Image()
{
    //dtor
}

void Image::createRGB()//To create R,G,B pixel array with calculated height ang width
{
    R=new int*[height];
    G=new int*[height];
    B=new int*[height];
    if(R==NULL||G==NULL||B==NULL)
    {
        Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("Size Error");
        dlg.run();
        exit(-1);
    }
    for(int i=0;i<height;i++)
    {
        R[i]=new int[width];
        G[i]=new int[width];
        B[i]=new int[width];
        if(R[i]==NULL||G[i]==NULL||B[i]==NULL)
        {
            Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
            dlg.set_title("Size Error");
            dlg.run();
            exit(-1);
        }
    }
}

void Image::deleteRGB() //This will delete R,G,B pixel of array
{
    for(int i=0;i<height;i++)
    {
        if(R[i]!=NULL)
            delete[] R[i];
        if(G[i]!=NULL)
            delete[] G[i];
        if(B[i]!=NULL)
            delete[] B[i];
    }
    if(R!=NULL)
        delete[] R;
    if(G!=NULL)
        delete[] G;
    if(B!=NULL)
        delete[] B;
}

bool Image::readim(std::ifstream& f) //to get information about image
{
    index=0;
    no_undo=0;
    is_save=true;
    int line[60];
    char linech[10];
    f.read(header,54);
    convert(header,line,54);
    if(header[0]!='B'||header[1]!='M')
    {
        Gtk::MessageDialog dlg("Please give only bmp image as input", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("Input Is Not BMP Image");
        dlg.run();
        return false;
    }
    size=line[2]+line[3]*pow(16,2)+line[4]*pow(16,4)+line[5]*pow(16,6);
    width=line[18]+line[19]*pow(16,2)+line[20]*pow(16,4)+line[21]*pow(16,6);
    height=line[22]+line[23]*pow(16,2)+line[24]*pow(16,4)+line[25]*pow(16,6);
    noofbitsperpixel=line[28]+line[29]*pow(16,2);
    if(noofbitsperpixel!=24)
    {

        Gtk::MessageDialog dlg("Please give only 24 bit bmp image as input", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("Input Image Is Not 24 bit BMP");
        dlg.run();
        return false;
    }
    if(size>40000000)
    {
        Gtk::MessageDialog dlg("You have choosen a very large image. Some process might take some time ", false,Gtk::MESSAGE_WARNING,Gtk::BUTTONS_OK, true);
        dlg.set_title("Too large image");
        dlg.run();

    }
    createRGB();
    for(int i=0;i<height;i++)     //reading RGB pixel of arrays
        {
            for(int j=0;j<width;j++)
            {
                f.read(linech,3);
                convert(linech,line,3);
                B[i][j]=line[0];
                G[i][j]=line[1];
                R[i][j]=line[2];

            }
            f.read(linech,width%4);   //reading extra byte which is 00 as number of bytes in aline are multiple of 4
                                   //so number of extra byte = 4-(3*width)%4= width%4
        }
return true;
}

void Image::createim() //creating image
{
    std::ofstream fo;
    char linech[10];
    int i,j,k,temp;
    fo.open(".image_temp.bmp");
    if(!fo)
    {
        Gtk::MessageDialog dlg("Can't create output image in the ImageEditor folder.", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("File Creation Error");
        dlg.run();
        exit(-1);
    }
    for(int i=0;i<54;i++)
    {
        if(i==2)
        {
           temp=size;
           for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            i=5;
        }
        else if(i==18)
        {
           temp=width;
           for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            temp=height;
            for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            i=25;
        }
        else
            fo<<header[i];
    }

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            linech[0]=B[i][j];
            linech[1]=G[i][j];
            linech[2]=R[i][j];
            fo.write(linech,3);
        }
        for(k=0;k<(width)%4;k++)
            linech[k]=0;
        fo.write(linech,width%4);
    }
    fo.close();
}

void Image::createim(std::ofstream& fo)  //Function overloading - creating image at specified folder
{
    char linech[10];
    int i,j,k,temp;
    if(!fo)
    {
        Gtk::MessageDialog dlg("Can't create output image in the specified folder", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("File Creation Error");
        dlg.run();
        exit(-1);
    }
    for(int i=0;i<54;i++)
    {
        if(i==2)    //writing size
        {
           temp=size;
           for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            i=5;
        }
        else if(i==18)  //writing width and height
        {
           temp=width;
           for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            temp=height;
            for(int j=0;j<4;j++)
            {
                linech[j]=temp%256;
                temp/=256;
            }
            fo.write(linech,4);
            i=25;
        }
        else
            fo<<header[i];
    }

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            linech[0]=B[i][j];
            linech[1]=G[i][j];
            linech[2]=R[i][j];
            fo.write(linech,3);
        }
        for(k=0;k<(width)%4;k++)
            linech[k]=0;
        fo.write(linech,width%4);
    }
    fo.close();
}

void Image::copyImage(const Image& I) //copying only R, G, B ,height,width and size NOT copying process[] and index
{
    if(height!=I.height||width!=I.width)
    {
        deleteRGB();
        height=I.height;
        width=I.width;
        size=I.size;
        for(int i=0;i<54;i++)
        {
            header[i]=I.header[i];
        }
        createRGB();
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            R[i][j]=I.R[i][j];
            G[i][j]=I.G[i][j];
            B[i][j]=I.B[i][j];
        }
    }
}

void Image::addprocess(int i,int j) //adding unique no as per effect applied necessary to undo
{
    if(index==500)
    {
        for(int m=0;m<index-1;m++)
            for(int n=0;n<5;n++)
                {
                    process[m][n]=process[m+1][n];
                    index--;
                }
    }
    process[index][0]=i;
    process[index][1]=j;
    process[index][2]=0;
    process[index][3]=0;
    process[index][4]=0;
    index++;
}

void Image::addprocess(int i,int* j) //for crop as it requires 4 values
{
    if(index==500)
    {
        for(int m=0;m<index-1;m++)
            for(int n=0;n<5;n++)
                {
                    process[m][n]=process[m+1][n];
                    index--;
                }
    }
    process[index][0]=i;
    for(int k=0;k<4;k++)
        process[index][k+1]=j[k];
    index++;
}

void Image::grayscale()
{
    int i,j,gray;
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {   gray=(0.3*R[i][j]+0.59*G[i][j]+0.11*B[i][j]);
            R[i][j]=G[i][j]=B[i][j]=gray;
        }
}

void Image::brightness(int b)
{
    int i,j;
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {
            R[i][j]=(R[i][j]+b>255)?255:((R[i][j]+b<0)?0:(R[i][j]+b));
            G[i][j]=(G[i][j]+b>255)?255:((G[i][j]+b<0)?0:(G[i][j]+b));
            B[i][j]=(B[i][j]+b>255)?255:((B[i][j]+b<0)?0:(B[i][j]+b));
        }
}

void Image::contrast()
{
    int *histogram,*equalizer_Y,*cdf;
    histogram=new int[256];
    equalizer_Y=new int[256];
    cdf=new int[256];
    // initializing all values to 0; cdf stands for cumulative distribution function
    for (int i = 0; i<256;i++)
    {
        histogram[i]=0;
        cdf[i]=0;
        equalizer_Y[i]=0;
    }
    float** Y,**Cb,**Cr;
    Y=createfloatarray(height,width);
    Cb=createfloatarray(height,width);
    Cr=createfloatarray(height,width);
    /*conversion of RGB map to YCbCr Map where Y represents
     the intensity map approximately*/
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            Y[i][j]= R[i][j]*0.299+G[i][j]*0.587+B[i][j]*0.114;
            Cb[i][j]= R[i][j]*(-0.168935) +G[i][j]*(-0.331665) +B[i][j]*0.50059 +128;
            Cr[i][j]=R[i][j]*0.499813 +G[i][j]*(-0.418531) +B[i][j]*(-0.081282) +128;
        }
    // applying histogram equalization to Y map only
    int tempi;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
             tempi=(round(Y[i][j])>255)?255: round(Y[i][j]);
           // if(tempi>=0&&tempi<256)
                histogram[tempi]++;
        }
    // calculation of cdf table
    cdf[0]=histogram[0];
    for (int i=1;i<256;i++)
    {
        cdf[i]=histogram[i]+cdf[i-1];
    }
    // calculation of minimum non zero cdf value
    int cdf_min=256;
    for (int i = 0; i <256; i++)
    {
        if(cdf[i]>0&&cdf[i]<cdf_min)
            cdf_min=cdf[i];
    }
    // calculating the eualizer_Y using the equalization formula
    for(int i=0;i<256;i++)
    {
        equalizer_Y[i]=(float)(cdf[i]-cdf_min)/(width*height-cdf_min)*(256-1);
    }
    delete [] cdf;
    delete [] histogram;
    // calculating the equalized Y value array
    float **Y_equalized;
    Y_equalized=createfloatarray(height,width);
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            tempi=((Y[i][j])>255)?255:(Y[i][j]);
            //if(tempi>=0&&tempi<256)
            Y_equalized[i][j]=equalizer_Y[tempi];
        }
    delete [] equalizer_Y;

    // back conversion of new equaklizer_YCbCr to RGB map
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            float temp=Y_equalized[i][j]/Y[i][j];
            R[i][j]= round(Y_equalized[i][j]*1.0 + temp*(Cr[i][j]-128)*1.403);
            if(R[i][j]<0) R[i][j]=0;
            else if(R[i][j]>255) R[i][j]=255;
            G[i][j]=(Y_equalized[i][j]*1.0+ temp*(Cb[i][j]-128)*(-0.344)
                    +temp*(Cr[i][j]-128)*(-0.714));
            B[i][j]=Y_equalized[i][j]*1.0+ temp*(Cb[i][j]-128)*1.77;
            if(G[i][j]>255) G[i][j]=255;
            else if(G[i][j]<0) G[i][j]=0;

            if(B[i][j]>255) B[i][j]=255;
            else if(B[i][j]<0) B[i][j]=0;
        }
    deletearray(Y,height);
    deletearray(Cr,height);
    deletearray(Cb,height);
    deletearray(Y_equalized,height);

}

void Image::rotate(int angle)
{
    float pi=M_PI;
    float theta=angle*pi/180; // calculating the angle in radian
    rotate1(R,theta);
    rotate1(G,theta);
    rotate1(B,theta);
}

void Image::rotate1(int**A,float theta)
{
    int i1,j1;
    int m=height/2,n=width/2;
    int temph=height*(std::abs(cos(theta)))+width*(std::abs(sin(theta)));
    int tempw=width*(std::abs(cos(theta)))+height*(std::abs(cos(theta)));
    int **B;
    B=createintarray(height,width);
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
    {
        i1=(j-n)*sin(theta)+(i-m)*cos(theta)+m+ 1/2*(temph-height);
        j1=(j-n)*cos(theta)-(i-m)*sin(theta)+n+ 1/2*(tempw-width);
        if(-1<i1 && i1<height&& -1<j1 && j1<width)
            B[i][j]=A[i1][j1];
        else
            B[i][j]=128;
    }
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        A[i][j]=B[i][j];
        deletearray(B,height);
    return;
}

void Image::rotate90(int p)
{
    /*code for rotating image clockwise and anti-clockwise by 90 degrees*/
    int i,j;
    int** newR,**newG,**newB; //creating new arrays as height and width are changed
    newR=createintarray(width,height);
    newG=createintarray(width,height);
    newB=createintarray(width,height);
    //rotating the image anticlockwise by 90 degrees
   if(p==1)
   {
        for(i=0;i<height;i++)
        {
            for(j=0;j<width;j++)
            {
                newR[j][height-i-1]=R[i][j];
                newG[j][height-i-1]=G[i][j];
                newB[j][height-i-1]=B[i][j];
            }
        }

   }
    //rotating the image clockwise by 90 degrees
    if(p==-1)
    {
        for(i=0;i<height;i++)
        {
            for(j=0;j<width;j++)
            {
                newR[width-j-1][i]=R[i][j];
                newG[width-j-1][i]=G[i][j];
                newB[width-j-1][i]=B[i][j];
            }
        }
    }
    deleteRGB();
    R=newR;
    G=newG;
    B=newB;
    i=height;    //swapping height and width
    height=width;
    width=i;
    size=3*height*((width%4==0)?width:(width+width%4))+54;
}

void Image::flip_horizontal()
{
    int i,j,temp;
    for(i=0;i<height;i++)
        for(j=0;j<width/2;j++)
        {
            temp = R[i][j];
            R[i][j]=R[i][width-1-j];
            R[i][width-1-j]=temp;

            temp = G[i][j];
            G[i][j]=G[i][width-1-j];
            G[i][width-1-j]=temp;

            temp = B[i][j];
            B[i][j]=B[i][width-1-j];
            B[i][width-1-j]=temp;
        }
}

void Image::flip_vertical()
{
    int i,j,temp;
    for(j=0;j<width;j++)
        for(i=0;i<height/2;i++)
        {
            temp=R[i][j];
            R[i][j]=R[height-1-i][j];
            R[height-1-i][j]=temp;

            temp=G[i][j];
            G[i][j]=G[height-1-i][j];
            G[height-1-i][j]=temp;

            temp=B[i][j];
            B[i][j]=B[height-1-i][j];
            B[height-1-i][j]=temp;
        }
}

void Image::negative()
{
    int i,j;
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {
            R[i][j]=255-R[i][j];
            G[i][j]=255-G[i][j];
            B[i][j]=255-B[i][j];
        }
}

void Image::dither()
{
    struct RGBpalette
    {
        int R,G,B;
    }table[16];

    //creating 16 color palette table found using photoshop - modify values!!!
    table[0].R = 0xFF;
    table[0].G = 0xFF;
    table[0].B = 0xFF;
    table[1].R = 0xC0;
    table[1].G = 0xC0;
    table[1].B = 0xC0;
    table[2].R = 0x80;
    table[2].G = 0x80;
    table[2].B = 0x80;
    table[3].R = 0x00;
    table[3].G = 0x00;
    table[3].B = 0x00;
    table[4].R = 0xFF;
    table[4].G = 0x00;
    table[4].B = 0x00;
    table[5].R = 0x80;
    table[5].G = 0x00;
    table[5].B = 0x00;
    table[6].R = 0xFF;
    table[6].G = 0xFF;
    table[6].B = 0x00;
    table[7].R = 0x80;
    table[7].G = 0x80;
    table[7].B = 0x00;
    table[8].R = 0x00;
    table[8].G = 0xFF;
    table[8].B = 0x00;
    table[9].R = 0x00;
    table[9].G = 0x80;
    table[9].B = 0x00;
    table[10].R = 0x00;
    table[10].G = 0xFF;
    table[10].B = 0xFF;
    table[11].R = 0x00;
    table[11].G = 0x80;
    table[11].B = 0x80;
    table[12].R = 0x00;
    table[12].G = 0x00;
    table[12].B = 0xFF;
    table[13].R = 0x00;
    table[13].G = 0x00;
    table[13].B = 0x80;
    table[14].R = 0xFF;
    table[14].G = 0x00;
    table[14].B = 0xFF;
    table[15].R = 0x80;
    table[15].G = 0x00;
    table[15].B = 0x80;

    int x,y,i, Dsquared, minD, bestindex=0;
    minD=3*255*255+1;
    for(x=0;x<height;x++)
    {
        for(y=0;y<width;y++)
        {
            //finding nearest palette color for every pixel in the image
            for(i=0;i<16;i++)
            {
                int Rdiff = R[x][y] - table[i].R;
                int Gdiff = G[x][y] - table[i].G;
                int Bdiff = B[x][y] - table[i].B;
                Dsquared = Rdiff*Rdiff + Gdiff*Gdiff + Bdiff*Bdiff;
                if (Dsquared < minD)
                {
                    minD=Dsquared;
                    bestindex = i;
                }
            }
            RGBpalette error;
            error.R = R[x][y]-table[bestindex].R;
            error.G = G[x][y]-table[bestindex].G;
            error.B = B[x][y]-table[bestindex].B;
            //disperse the error in neighbouring elements
            if(x+1<height)
            {
                if((R[x+1][y] + error.R*7/16 < 255) && (R[x+1][y] + error.R*7/16 >= 0))
                R[x+1][y]+=error.R*7/16;
            }
            if(x-1>=0 && y+1<width)
            {
                if((R[x-1][y+1] + error.R*3/16 < 255) && (R[x-1][y+1] + error.R*3/16 >= 0))
                R[x-1][y+1]+=error.R*3/16;
            }
            if(y+1<width)
            {
                if((R[x][y+1] + error.R*5/16 < 255) && (R[x][y+1]+ error.R*5/16 >= 0))
                R[x][y+1]+=error.R*5/16 < 255;
            }
            if(x+1<height &&y+1<width)
            {
                if((R[x+1][y+1]+ error.R*1/16 < 255)&& (R[x+1][y+1]+ error.R*1/16 >= 0))
                R[x+1][y+1]+=error.R*1/16;
            }

            if(x+1<height)
            {
                if((G[x+1][y] + error.G*7/16 < 255) && (G[x+1][y] + error.G*7/16 >= 0))
                G[x+1][y]+=error.G*7/16;
            }
            if(x-1>=0 && y+1<width)
            {
                if((G[x-1][y+1] + error.G*3/16 < 255) && (G[x-1][y+1] + error.G*3/16 >= 0))
                G[x-1][y+1]+=error.G*3/16;
            }
            if(y+1<width)
            {
                if((G[x][y+1] + error.G*5/16 < 255) && (G[x][y+1]+ error.G*5/16 >= 0))
                G[x][y+1]+=error.G*5/16 < 255;
            }
            if(x+1<height &&y+1<width)
            {
                if((G[x+1][y+1]+ error.G*1/16 < 255)&& (G[x+1][y+1]+ error.G*1/16 >= 0))
                G[x+1][y+1]+=error.G*1/16;
            }

            if(x+1<height)
            {
                if((B[x+1][y] + error.B*7/16 < 255) && (B[x+1][y] + error.B*7/16 >= 0))
                B[x+1][y]+=error.B*7/16;
            }
            if(x-1>=0 && y+1<width)
            {
                if((B[x-1][y+1] + error.B*3/16 < 255) && (B[x-1][y+1] + error.B*3/16 >= 0))
                B[x-1][y+1]+=error.B*3/16;
            }
            if(y+1<width)
            {
                if((B[x][y+1] + error.B*5/16 < 255) && (B[x][y+1]+ error.B*5/16 >= 0))
                B[x][y+1]+=error.B*5/16 < 255;
            }
            if(x+1<height &&y+1<width)
            {
                if((B[x+1][y+1]+ error.B*1/16 < 255)&& (B[x+1][y+1]+ error.B*1/16 >= 0))
                B[x+1][y+1]+=error.B*1/16;
            }
        }
    }
}

void Image::sepia()
{
    /*code for getting sepia effect for the image*/
    int **nR,**nG,**nB;
    nR=createintarray(height,width);
    nG=createintarray(height,width);
    nB=createintarray(height,width);

    /*Sepia tone as recommended by Microsoft standard */
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++)
        {
            nR[i][j] = (R[i][j]*0.393) + (G[i][j]*0.769) + (B[i][j]*0.189);
            nG[i][j] = (R[i][j]*0.349) + (G[i][j]*0.686) + (B[i][j]*0.168);
            nB[i][j] = (R[i][j]*0.272) + (G[i][j]*0.534) + (B[i][j]*0.131);
            if(nR[i][j]>255) nR[i][j]=255;
            if(nG[i][j]>255) nG[i][j]=255;
            if(nB[i][j]>255) nB[i][j]=255;
        }
    }
    //copieng nR,nG,nB,to R,G,B arrays
    deleteRGB();
    R=nR;
    G=nG;
    B=nB;
}

void Image::blur(int n)
{
    /*code for implementing 'blur' function on an image using 'Gaussian blur'*/
    float sum=0,**kernel;
    float sigma=n/6.0 +2;

    double sumr=0,sumb=0,sumg=0;

    //if (n<1 || n>10){cout<<"bad user input"<<endl;} //the value of 'n' should be integer from 1 to 10
    if(n==1){return;}//image is unaltered('Gaussian blur' is not applied)
    else{
        if(n%2==0){ kernel= new float *[n+1];//creating a kernel arrays to apply convolution to R,B,G arrays
                        for(int i=0;i<(n+1);i++)
                            {
                                kernel[i]= new float[n+1];
                            }
                    for(int i=0;i<(n+1);i++)
                    {
                        for(int j=0;j<(n+1);j++)
                            {
                                    kernel[i][j]=exp( -0.5 * (pow((i-(n/2))/sigma, 2.0) + pow((j-(n/2))/sigma,2.0)) );
                                                   // / (2 * M_PI  * sigma* sigma);
                                                    // Accumulate the kernel array values
                                    sum += kernel[i][j];

                            }

                    }
                        // Normalize the kernel array
                    for (int i=0;i<(n+1);i++)
                        for (int j = 0;j<(n+1);j++)
                            kernel[i][j] /= sum;
    }


        else
        {
            kernel= new float *[n];//creating a kernel arrays to apply convolution to R,B,G arrays
                            for(int i=0;i<n;i++)
                                {
                                    kernel[i]= new float[n];
                                }
                            for(int i=0;i<n;i++)
                            {
                                for(int j=0;j<n;j++)
                                    {
                                        kernel[i][j]=exp( -0.5 * (pow((i-(n/2))/sigma, 2.0) + pow((j-(n/2))/sigma,2.0)) );
                                                        // / (2 * M_PI  * sigma* sigma);
                                                        // Accumulate the kernel array values
                                        sum += kernel[i][j];

                                    }

                            }
                  // Normalize the kernel array

                            for (int i = 0;i<n;i++)
                                for (int j = 0;j<n;j++)
                                    kernel[i][j] /= sum;
            }

            for(int i=0;i<height;i++)
               {
                  for(int j=0;j<width;j++)
                {
                            for(int r=-(n/2);r<=(n/2);r++)
                            {
                             for(int s=-(n/2);s<=(n/2);s++)
                                {

                                int v1=i+r,v2=j+s,v3=n/2+r,v4=n/2+s;
                                if(-1<v1 && v1<height && -1<v2&& v2<width)
                                    {
                                        sumr+=R[v1][v2]*kernel[v3][v4];
                                        sumg+=G[v1][v2]*kernel[v3][v4];
                                        sumb+=B[v1][v2]*kernel[v3][v4];
                                    }

                           }
                    }
                    R[i][j]=sumr;//applying 'Gaussian blur' to R array by means of convolution between a kernel and R array
                    G[i][j]=sumg;//applying 'Gaussian blur' to G array by means of convolution between a kernel and G array
                    B[i][j]=sumb;//applying 'Gaussian blur' to B array by means of convolution between a kernel and B array
                    sumr=sumg=sumb=0;

               }
            }


        }
	return;
}

void Image::edge_detect()
{
    int i,j;
    //8 sobel masks.. allocating memory
    int ***mask = new int**[8];
    for(i=0;i<8;i++)
    {
        mask[i] = new int*[3];
        for(j=0;j<3;j++)
            mask[i][j]=new int[3];
    }
    //the values of the masks
    mask[0][0][0]=1;
    mask[0][0][1]=2;
    mask[0][0][2]=1;
    mask[0][1][0]=0;
    mask[0][1][1]=0;
    mask[0][1][2]=0;
    mask[0][2][0]=-1;
    mask[0][2][1]=-2;
    mask[0][2][2]=-1;

    mask[1][0][0]=2;
    mask[1][0][1]=1;
    mask[1][0][2]=0;
    mask[1][1][0]=1;
    mask[1][1][1]=0;
    mask[1][1][2]=-1;
    mask[1][2][0]=0;
    mask[1][2][1]=-1;
    mask[1][2][2]=-2;

    mask[2][0][0]=1;
    mask[2][0][1]=0;
    mask[2][0][2]=-1;
    mask[2][1][0]=2;
    mask[2][1][1]=0;
    mask[2][1][2]=-2;
    mask[2][2][0]=1;
    mask[2][2][1]=0;
    mask[2][2][2]=-1;

    mask[3][0][0]=0;
    mask[3][0][1]=-1;
    mask[3][0][2]=-2;
    mask[3][1][0]=1;
    mask[3][1][1]=0;
    mask[3][1][2]=-1;
    mask[3][2][0]=2;
    mask[3][2][1]=1;
    mask[3][2][2]=0;

    mask[4][0][0]=-1;
    mask[4][0][1]=-2;
    mask[4][0][2]=-1;
    mask[4][1][0]=0;
    mask[4][1][1]=0;
    mask[4][1][2]=0;
    mask[4][2][0]=1;
    mask[4][2][1]=2;
    mask[4][2][2]=1;

    mask[5][0][0]=-2;
    mask[5][0][1]=-1;
    mask[5][0][2]=0;
    mask[5][1][0]=-1;
    mask[5][1][1]=0;
    mask[5][1][2]=1;
    mask[5][2][0]=0;
    mask[5][2][1]=1;
    mask[5][2][2]=2;

    mask[6][0][0]=-1;
    mask[6][0][1]=0;
    mask[6][0][2]=1;
    mask[6][1][0]=-2;
    mask[6][1][1]=0;
    mask[6][1][2]=2;
    mask[6][2][0]=-1;
    mask[6][2][1]=0;
    mask[6][2][2]=1;

    mask[7][0][0]=0;
    mask[7][0][1]=1;
    mask[7][0][2]=2;
    mask[7][1][0]=-1;
    mask[7][1][1]=0;
    mask[7][1][2]=1;
    mask[7][2][0]=-2;
    mask[7][2][1]=-1;
    mask[7][2][2]=0;

    int sum=0,a1,b1,sum1;
    float grayvalue;
    int **a,**b,**c;
    a=createintarray(height,width);
    b=createintarray(height,width);
    c=createintarray(height,width);

    for(i=1;i<height-1;i++)
    {
        for(j=1;j<width-1;j++)
        {    sum=0;
            for(int m=0;m<8;m++) //for all masks
            {
                sum1=0;
                for(a1=-1;a1<2;a1++)
                {
                    for(b1=-1;b1<2;b1++)
                    {
                        grayvalue = 0.3*R[i+a1][j+b1]+0.59*G[i+a1][j+b1]+0.11*B[i+a1][j+b1];
                        sum1+=grayvalue*mask[m][a1+1][b1+1];
                    }
                }
                sum+=fabs(sum1);

            }
            sum/=4;
            if(sum>255) sum=255;
                if(sum<0) sum=0;
                //if(sum>grayvalue)
                a[i][j]=b[i][j]=c[i][j]=sum;
        }
    }
    deleteRGB();
    R=a;
    G=b;
    B=c;
//    //now threshold the image
//    for(i=0;i<height;i++)
//    {
//        for(int j=0;j<width;j++)
//        {
//            if(R[i][j] > 128) R[i][j]=G[i][j]=B[i][j]=0;
//            else R[i][j]=G[i][j]=B[i][j]=255;
//        }
//    }
}

void Image::salt_pepper() //using rand function to create dots
{
    srand(time(NULL));
    int i,j;
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
        {
            int temp=rand()%256;
            if(temp==0) R[i][j]=G[i][j]=B[i][j]=0;
            else if(temp==255) R[i][j]=G[i][j]=B[i][j]=255;
        }
}

void Image::halftone()
{
    float** eg,**ep;
    eg=createfloatarray(height,width);
    ep=createfloatarray(height,width);
    int i,j;
    for(i = 0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            ep[i][j]=0.0;eg[i][j]=0.0;
        }
    }

    float c[2][3];
    c[0][0]=0.0; c[0][1]=0.2; c[0][2]=0.0;
    c[1][0]=0.6; c[1][1]=0.1; c[1][3]=0.1;
    int x,y;
    float sum_p,t,threshold=128;
    for(int m=0; m<height; m++)
    {
        for(int n=0; n<width; n++)
        {
            float grayvalue = 0.3*R[m][n]+0.59*G[m][n]+0.11*B[m][n];
            sum_p = 0.0;
            for(i=0; i<2; i++)
            {
                for(j=0; j<3; j++)
                {
                    x = m-i+1;
                    y = n-j+1;
                    if(x < 0) x=0;
                    if(x >= height) x=height-1;
                    if(y < 0) y=0;
                    if(y >= width) y=width-1;
                    sum_p = sum_p + c[i][j] * eg[x][y];
                }
            }
            ep[m][n] = sum_p;
            t = grayvalue + ep[m][n];
            if(t > threshold)
            {
                eg[m][n]= t - threshold*2;
                R[m][n]=G[m][n]=B[m][n]=255;
            }
            else
            {
                eg[m][n]=t;
                R[m][n]=G[m][n]=B[m][n]=0;
            }
        }
    }
    deletearray(ep,height);
    deletearray(eg,height);
}

void Image::crop(int x1,int x2,int y1,int y2)
{
    int newheight = std::abs(y2-y1);
    int newwidth = std::abs(x2-x1);
    int i,j;
    int **newR,**newG,**newB;
    newR=createintarray(newheight,newwidth); //as height and width are changed creating new arrays
    newG=createintarray(newheight,newwidth);
    newB=createintarray(newheight,newwidth);
    int min_y = y1>y2 ? y2 : y1;
    int min_x= x1>x2 ? x2 : x1;
    for(i=0;i<newheight;i++)
       { for(j=0;j<newwidth;j++)
            {
                newR[i][j]=R[min_y+i][min_x+j];
                newG[i][j]=G[min_y+i][min_x+j];
                newB[i][j]=B[min_y+i][min_x+j];
            }
       }
    deleteRGB();
    R=newR;
    G=newG;
    B=newB;

    height=newheight; // updating values of height and width
    width=newwidth;
    size=3*height*((width%4==0)?width:(width+width%4))+54;
}

void Image::posterize(int n)
{
    blur(4); // blurring effect given to cause smmothening
    posterize1(R,n);
    posterize1(G,n);
    posterize1(B,n);
}

void Image::posterize1(int** A,int n)
{
    int x;int s=256/n;int k=255/(n-1); // number of colours used to make image
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            x=A[i][j]/s;
            A[i][j]=x*k;//distributing A[i][j] accordingly its value lies in which range
            if(A[i][j]>255) A[i][j]=255;
        }
    }
}

void Image::undo(Image imagorg)
{
    copyImage(imagorg);
    no_undo++;
    index--;
    for(int i=0;i<index;i++)
    {
        switch(process[i][0])
        {
            case 1: grayscale();
                    break;
            case 2: sepia();
                    break;
            case 3: negative();
                    break;
            case 4: blur(process[i][1]);
                    break;
            case 5: brightness(process[i][1]);
                    break;
            case 6: contrast();
                    break;
            case 7: rotate(process[i][1]);
                    break;
            case 8: rotate90(process[i][1]);
                    break;
            case 9: flip_horizontal();
                    break;
            case 10: flip_vertical();
                     break;
            case 11: dither();
                     break;
            case 12: halftone();
                     break;
            case 13: salt_pepper();
                     break;
            case 14: edge_detect();
                     break;
            case 15: posterize(process[i][1]);
                     break;
            case 16: crop(process[i][1],process[i][2],process[i][3],process[i][4]);
                     break;
        }
    }
}

void Image::redo()
{
    no_undo--;
    switch(process[index][0])
    {
            case 1: grayscale();
                    break;
            case 2: sepia();
                    break;
            case 3: negative();
                    break;
            case 4: blur(process[index][1]);
                    break;
            case 5: brightness(process[index][1]);
                    break;
            case 6: contrast();
                    break;
            case 7: rotate(process[index][1]);
                    break;
            case 8: rotate90(process[index][1]);
                    break;
            case 9: flip_horizontal();
                    break;
            case 10: flip_vertical();
                     break;
            case 11: dither();
                     break;
            case 12: halftone();
                     break;
            case 13: salt_pepper();
                     break;
            case 14: edge_detect();
                     break;
            case 15: posterize(process[index][1]);
                     break;
            case 16: crop(process[index][1],process[index][2],process[index][3],process[index][4]);
                     break;
        }
        index++;
}

void convert(char*A,int*B,int len)//to convert char array to int array
{
    for(int i=0;i<len;i++)
        B[i]=(A[i]>=0)?A[i]:(A[i]+256);
}

int** createintarray (int h,int w) //to create dynamically allocated 2 d int array a[h][w]
{
    int**a;
    a=new int*[h];
    if(a==NULL)
    {
        Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg.set_title("Size Error");
        dlg.run();
        exit(-1);
    }
    for(int i=0;i<h;i++)
    {
        a[i]=new int[w];
        if(a[i]==NULL)
        {
            Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            dlg.set_title("Size Error");
            dlg.run();
            exit(-1);
        }

    }
    return a;
}

void deletearray(int**a,int height)
{
    for(int i=0;i<height;i++)
    {
        if(a[i]!=NULL)
            delete[] a[i];
    }
    if(a!=NULL)
        delete[] a;
}

float** createfloatarray (int height,int width)  //to create dynamically allocated 2 d float array a[h][w]
{
    float**a;
    a=new float*[height];
    if(a==NULL)
    {
        Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg.set_title("Size Error");
        dlg.run();
        exit(-1);
    }
    for(int i=0;i<height;i++)
    {
        a[i]=new float[width];
        if(a[i]==NULL)
        {
            Gtk::MessageDialog dlg("Too large image!! Please use smaller image and save it as  \"in.bmp\" in the ImageEditor folder", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            dlg.set_title("Size Error");
            dlg.run();
            exit(-1);
        }
    }
    return a;
}

void deletearray(float **a,int height) //function overloading
{
    for(int i=0;i<height;i++)
    {
        if(a[i]!=NULL)
            delete[] a[i];
    }
    if(a!=NULL)
        delete[] a;
}
