#ifndef VISION_H
#define VISION_H

#include <iostream>
#include <albroker.h>
#include "system/Thread.h"
//#include "albrokermanager.h"
#include "alptr.h"
//#include "almemoryproxy.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "KImageExtractor.h"
#include "KSegmentator.h"

#include "pub_sub/publisher.h"
#include "messages/VisionObservations.pb.h"
//#define DEBUGVISION


class Vision : public Thread, public Publisher
{

public:
    /**
    * The only available constructor: I need:
    * albroker: only to pass along to the KImageExtractor
    * MessageQueue : a messageQueue to deliver messages to
    * gui= enable debug gui? On the robot all cvhighigui calls should fail, so default is false
    */
    Vision(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue *mq,bool gui=false) :
            ext(pbroker),cvHighgui(gui),type(VISION_CSPACE)
    {
        cout << "Vision():" ;//<< endl;
        rawImage = ext.allocateImage();
        if(cvHighgui)
        {
            segIpl = cvCreateImage(cvSize(rawImage->width, rawImage->height), IPL_DEPTH_8U, 3);
            cvNamedWindow("win1", CV_WINDOW_AUTOSIZE);
        }
        //memory = pbroker->getMemoryProxy();

        ifstream *config = new ifstream("segmentation.conf");
        seg = new KSegmentator(*config);//TODO PATH!!!
        mq->add_publisher(this);
        cout<<"Done!"<<endl;


    }
    void testrun()
    {

        //cout << "fetchImage" << endl;
        struct timespec t = ext.fetchImage(rawImage);
#ifdef DEBUGVISION
        cout << "ImageTimestamp:"<< t.tv_sec << " " << t.tv_nsec << endl;
#endif
        //SleepMs(1000);
        gridScan(orange);
        if(cvHighgui)
            cvShowSegmented();




    }
    ~Vision()
    {

    }
    void run()
    {
        //std::cout << " Vision run" << std::endl;
        testrun();
    }

private:
    bool cvHighgui;
    BallTrackMessage trckmsg;
    //AL::ALPtr<AL::ALMemoryProxy> memory;

    //Ball Detection related
    typedef struct balldata
    {
        float x, y;
        float r;//Observed radius
    } balldata_t;
    enum colors
    {
        bad = 0, red = 1, blue = 2, green = 3, skyblue = 4, yellow = 5, orange = 6, white = 7, black = 8
    };
    //Extractor Object, to get a new image
    KImageExtractor ext;
    KSegmentator *seg;
    int type;//Colorspace fourCC
    //Raw Input Image
    IplImage *rawImage;
    IplImage *segIpl;
    //Ball Detection related
    std::vector<CvPoint> ballpixels;

    void gridScan(const KSegmentator::colormask_t color);
    bool calculateValidBall(const CvPoint2D32f center, float radius, KSegmentator::colormask_t c);
    balldata_t locateBall(vector<CvPoint> cand);
    CvPoint traceline(CvPoint start, CvPoint vel, KSegmentator::colormask_t c);
    //Wrapper for seg object
    KSegmentator::colormask_t doSeg(int x, int y)
    {
        if (x >= 0 && x < (rawImage-> width) && y >= 0 && y < (rawImage-> height))
        {
            return seg->classifyPixel(rawImage, x, y, type);
            //return sgm->getColor(y, x, (uInt8 *) fIplImageHeader->imageData, false);
        }
        else
        {
            return 0;
        }

    } ;
    void cvShowSegmented();
};

#endif
