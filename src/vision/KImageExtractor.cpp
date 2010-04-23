#include "KImageExtractor.h"


struct timespec KImageExtractor::fetchImage(IplImage *img)
    {
      struct timespec rt;
      
      if(doneSubscribe==false)
      { 
        cout<<"KImageExtractor: Warning! fetchImage()  called although GVM Subscription has failed!"<<endl;
        rt.tv_sec=0;
        rt.tv_nsec=0;
        return rt;
      }
      
      #ifdef REMOTE_ON
          //		cout << "Remote method on" << endl;
          //		sleep(1);
          ALValue results;
      #ifdef RAW
          results = (c->call<ALValue> ("getDirectRawImageRemote", fGvmName));
      #else
          results = (c->call<ALValue> ("getImageRemote", fGvmName));
      #endif
          if (results.getType() != ALValue::TypeArray && results.getSize() != 7) {
            throw ALError(getName(), "saveImageRemote", "Invalid image returned.");
          }
          //		const int size = results[6].getSize();
          // You can get some image information that you may find useful.
          //	const int width = (int) results[0];
          //	const int height = (int) results[1];
          //	const int nbLayers = (int) results[2];
          //	const int colorSpace = (int) results[3];
          //const long long timeStamp = ((long long) (int) results[4]) * 1000000LL + ((long long) (int) results[5]);
          //	const int seconds = (int) (timeStamp / 1000000LL);
          // Set the buffer we received to our IplImage header.
          //fIplImageHeader->imageData = (char*) (results[6].GetBinary());
          //cout << "Size" << size << endl;
        
          
          
          int width = (int) results[0];
          int height = (int) results[1];
          int nChannels = (int) results[2];
          int colorSpace = (int) results[3];
          //Fetch TimeStamp;
          rt.tv_sec=(time_t) results[4];
          rt.tv_nsec=(long)  results[5]*1000L;
          char *imdata=img->imageData;
          cvInitImageHeader(img, cvSize(width,height),IPL_DEPTH_8U, nChannels);
          img->imageData=imdata;
          
          if (img->imageData != NULL) {
            //free( fIplImageHeader->imageData);
            memcpy(img->imageData, (char*) (results[6].GetBinary()), results[6].getSize() * sizeof(unsigned char));
          } else {
            img->imageData = new char[results[6].getSize()];
            memcpy(img->imageData, (char*) (results[6].GetBinary()), results[6].getSize() * sizeof(char));
          }
      #else
          //cout << "Remote method off" << endl;
          //sleep(1);
          ALImage* imageIn = NULL;
          // Now you can get the pointer to the video structure.
      #ifdef RAW
          imageIn = (ALImage*) (c->call<int> ("getDirectRawImage", GVM_name));
      #else
          imageIn = (ALImage*) (c->call<int> ("getImageLocal", GVM_name));
      #endif
          if (!imageIn) {
            throw ALError("KImageExtractor", "saveImageLocal", "Invalid image returned.");
          }
          //fLogProxy->info(getName(), imageIn->toString());
          // You can get some image information that you may find useful.
          int width = imageIn->fWidth;
          int height = imageIn->fHeight;
          const int nChannels = imageIn->fNbLayers;
          //		const int colorSpace = imageIn->fColorSpace;
          const long long timeStamp = imageIn->fTimeStamp;
          //		const int seconds = (int) (timeStamp / 1000000LL);
          const int size = width*height*nChannels;
          // Set the buffer we received to our IplImage header.
           //Fetch TimeStamp;
          rt.tv_sec=(time_t) (timeStamp / 1000000LL);
          rt.tv_nsec=(long)  ((timeStamp-rt.tv_sec*1LL)*1000LL);
          
          char *imdata=img->imageData;
          cvInitImageHeader(img,  cvSize(width,height),IPL_DEPTH_8U, nChannels);
          img->imageData=imdata;
          
          if(img->imageData!=NULL) {
            //free( fIplImageHeader->imageData);
            memcpy ( img->imageData, (char*) imageIn->getFrame(), size*sizeof(char) );
          } else {
            img->imageData = new char[size];
            memcpy ( img->imageData, (char*) imageIn->getFrame(), size*sizeof(char) );
          }
          //fIplImageHeader->imageData = (char*) imageIn->getFrame();
          //saveIplImage(fIplImageHeader, name, pImageFormat, seconds);
          // Now that you're done with the (local) image, you have to release it from the V.I.M.
          c->call<int> ("releaseImage", GVM_name);
      #endif
      return rt;
    };
    
    IplImage *KImageExtractor::allocateImage(){
      if(doneSubscribe==false)
      { 
        return NULL;
        
      }
      //Stupid way, but it works!
      int nChannels=getNumLayersInColorSpace(cSpace);
      int width;
      int height;
      getSizeFromResolution(resolution,width,height);
      
      return cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nChannels);
    }
