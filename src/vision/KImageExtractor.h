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



 #define VISION_RESOLUTION kQVGA
 #define VISION_CSPACE kYUV422InterlacedColorSpace
 #define VISON_FPS 5
 #define VISION_GVMNAME "KImageExtractor"

 #define REMOTE_ON 1
 #undef RAW

 using namespace AL;
 using namespace std;
 /**
  * Extract Image from hardware
  * Automatically deals with naoqi related stuff, and handles gracefully image size changes
  * TODO: Provide functionality to change resolution/framerate on the fly
  */
  class KImageExtractor
  {
    public:

      KImageExtractor(AL::ALPtr<AL::ALBroker> pbroker ):resolution(VISION_RESOLUTION),cSpace( VISION_CSPACE)
      {
          doneSubscribe=false;
          try {
            c = pbroker->getProxy( "ALVideoDevice" );

            GVM_name = c->call<std::string>( "subscribe", GVM_name, resolution,
                  cSpace,VISON_FPS );
          doneSubscribe=true;

          } catch (AL::ALError& e) {
              cout<<e.toString()<<endl;
              throw ALError("KImageExtractor", "Construct ", "Unable to create proxies and subscribe GVM");
          }

      }

      ~KImageExtractor()
      {
          try {

            c->callVoid( "unsubscribe", GVM_name );
          }
          catch (AL::ALError& e) {
              throw ALError("KImageExtractor", "Destruct", "Unable to unsubscribe GVM");
          }
      }
    //Get new Image from hardware
    struct timespec fetchImage(IplImage *img);
    //Create new space for image
    IplImage *allocateImage();

    private:
      AL::ALPtr<AL::ALProxy> c;//Camera proxy to naoqi
      //Name used when subscribing Generic Video Module
      std::string GVM_name;
      int resolution;//Current Resolution
      int cSpace;// Current Colorspace
      bool doneSubscribe;//Initializations done?
  };


 #endif
