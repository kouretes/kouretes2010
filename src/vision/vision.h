 #ifndef VISION_H
 #define VISION_H
  
  #include <iostream>
  #include <albroker.h>
 

  //#include "albrokermanager.h"
  #include "alptr.h"
  #include <opencv/cv.h>

  #include "KImageExtractor.h"

 
 class Vision{
  
    
  	
    Vision(AL::ALPtr<AL::ALBroker> pbroker):ext(pbroker){
     
      rawImage=ext.allocateImage();
    
    }
    ~Vision()
    {
     
    }
    void run(){
      std::cout << " MotionController Runs test" << std::endl;
      }
   private:
    KImageExtractor ext;
    IplImage *rawImage;
    
    
 };
 
 #endif
