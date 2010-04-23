 #ifndef KIMAGEEXTRACTOR_H
 #define KIMAGEEXTRACTOR_H
 
  #include <iostream>
  #include "alxplatform.h"
  #include <albroker.h>
  #include <alproxy.h>
  //#include <almemoryfastaccess.h>
  #include "almotionproxy.h"
  #include "alvideodeviceproxy.h"
  #include "alvisiondefinitions.h"
  #include "alimage.h"
  //#include "albrokermanager.h"
  #include "alptr.h"
  #include <opencv/cv.h>
  #include <time.h>


  
 #define VISION_RESOLUTION kQVGA
 #define VISION_CSPACE kYUV422InterlacedColorSpace
 #define VISON_FPS 5
 #define VISION_GVMNAME "KImageExtractor"
 
 using namespace AL;
 using namespace std;
  class KImageExtractor
  {
    public:
      KImageExtractor(AL::ALPtr<AL::ALBroker> pbroker):resolution(VISION_RESOLUTION),cSpace( VISION_CSPACE)
      {
          doneSubscribe=false;
          try {
            ALPtr<ALProxy> c = pbroker->getProxy( "ALVideoDevice" );
  
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
    struct timespec fetchImage(IplImage *img);
    IplImage *allocateImage();
    
    private:
      AL::ALPtr<AL::ALProxy> c;
      std::string GVM_name;
      int resolution;
      int cSpace;
      bool doneSubscribe;
  };
  
  
 #endif

