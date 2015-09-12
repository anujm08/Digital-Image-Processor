#ifndef IMAGE_H
#define IMAGE_H
#include<fstream>
#include<gtkmm.h>
class Image
{
    public:
        Image();
        virtual ~Image();
        bool readim(std::ifstream& f);
        void createim();
        void createim(std::ofstream& fo);
        void grayscale();
        void brightness(int b);
        void contrast();
        void sepia();
        void negative();
        void blur(int n);
        void rotate90(int p);
        void flip_horizontal();
        void flip_vertical();
        void dither();
        void rotate(int angle);
        void rotate1(int**A,float theta);
        void edge_detect();
        void posterize();
        void salt_pepper();
        void halftone();
        void crop(int x1,int x2,int y1,int y2);
        void posterize(int n);
        void posterize1(int** A,int n);
        void undo(Image imagorg);
        void redo();
        void addprocess(int i,int j=0);
        void addprocess(int i,int* j);
        void createRGB();
        void deleteRGB();
        void copyImage(const Image& I);
        int process[500][5];               //A maximum of 500 continuous undo are possible;
        int index;
        int no_undo;
        int width;
        int height;
        bool is_save;
        unsigned int size;
    protected:
    private:
        char header[60];
        int noofbitsperpixel;
        int**R,**G,**B;
};
#endif // Image_H
