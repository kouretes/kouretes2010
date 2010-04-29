#ifndef KIMAGEEXTRACTOR_H
#define KIMAGEEXTRACTOR_H

#include <iostream>
#include "alxplatform.h"
#include <albroker.h>
#include <alproxy.h>
//#include <almemoryfastaccess.h>
#include "almotionproxy.h"
#include "alvideodeviceproxy.h"
#include "alvision/alvisiondefinitions.h"
#include "alvision/alimage.h"
//#include "albrokermanager.h"
#include "alptr.h"
#include <opencv/cv.h>
#include <time.h>
#include "pub_sub/publisher.h"


#define VISION_RESOLUTION kQVGA
#define VISION_CSPACE kYUV422InterlacedColorSpace
#define VISON_FPS 15
#define VISION_GVMNAME "KImageExtractor"

#define REMOTE_ON 1
#define  RAW

using namespace AL;
using namespace std;
/**
 * Extract Image from hardware
 * Automatically deals with naoqi related stuff, and handles gracefully image size changes
 * TODO: Provide functionality to change resolution/framerate on the fly
 */
class KImageExtractor : public Publisher
{
	public:
		KImageExtractor(AL::ALPtr<AL::ALBroker> pbroker, MessageQueue *mq);

		~KImageExtractor();
		//Get new Image from hardware
		struct timespec fetchImage(IplImage *img);
		//Create new space for image
		IplImage *allocateImage();
        float calibrateCamera(int sleeptime=2500,int exp=50);
	private:
		AL::ALPtr<AL::ALProxy> c;//Camera proxy to naoqi
		//Name used when subscribing Generic Video Module
		std::string GVM_name;
		int resolution;//Current Resolution
		int cSpace;// Current Colorspace
		bool doneSubscribe;//Initializations done?

};


#endif

