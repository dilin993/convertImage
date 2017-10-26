#include <opencv2/opencv.hpp>
using namespace cv;

#include <fstream>
using namespace std;

void writeToFile(Mat img,int no)
{
    // Open the file in write mode.
    ofstream outputFile;
    string filename = "../frame" + to_string(no) + ".h";
    outputFile.open(filename);

    // Iterate through pixels.

    outputFile << "uint32_t frame" << no << "[]={" << endl;

    Mat yuv;
    cvtColor(img,yuv,CV_BGR2YUV);
    uint8_t* pixelPtr = (uint8_t*)yuv.data;
    const int CN = yuv.channels();
    const int SIZE = yuv.rows * yuv.cols;
    for(int i=0;i<SIZE;i+=2)
    {
        uint8_t y1 = pixelPtr[i*CN+0];
        uint8_t u = (pixelPtr[i*CN+1] + pixelPtr[(i+1)*CN+1])/2;
        uint8_t v = (pixelPtr[i*CN+2] + pixelPtr[(i+1)*CN+2])/2;
        uint8_t y2 = pixelPtr[(i+1)*CN+0];
        uint32_t word = y1;
        word = word  << 8;
        word = word | u;
        word = word  << 8;
        word = word | y2;
        word = word  << 8;
        word = word | v;
        if(i<(SIZE-2))
        {
            outputFile << word << "," << endl;
        }
        else
        {
            outputFile << word << endl;
        }
    }

    outputFile << "};";

    // Close the file.
    outputFile.close();
}


int main(int argc, char** argv)
{
    VideoCapture cap("/home/dilin/fyp/people_videos/terrace1-c0.avi");
    Mat img;

    for(int i=0;i<1000;i++)
        cap.grab();

    const int N = 3;
    for(int i=0;i<N;i++)
    {
        cap.read(img);
        resize(img,img,Size(320,240));
        writeToFile(img,i);
    }



    return 0;
}
