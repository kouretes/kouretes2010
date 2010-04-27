 #ifndef VISION_H
 #define VISION_H

  #include <iostream>
  #include <albroker.h>


  //#include "albrokermanager.h"
  #include "alptr.h"
  #include "almemoryproxy.h"
  #include <opencv/cv.h>
  #include <opencv/highgui.h>

  #include "KImageExtractor.h"
  #include "KSegmentator.h"

 class Vision{

    public:

        Vision(AL::ALPtr<AL::ALBroker> pbroker):ext(pbroker),type(VISION_CSPACE){
         cout<<"Vision()"<<endl;
          rawImage=ext.allocateImage();
          memory=pbroker->getMemoryProxy();

          ifstream *config = new ifstream("segmentation.conf");
          seg= new KSegmentator(*config);//TODO PATH!!!

        }
        void testrun()
        {
            IplImage *segIpl=cvCreateImage(cvSize(rawImage->width, rawImage->height), IPL_DEPTH_8U, 3);
            cvNamedWindow("win1", CV_WINDOW_AUTOSIZE);
            while(1)
            {
                cout<<"fetchImage"<<endl;
                struct timespec t= ext.fetchImage(rawImage);
                cout<<t.tv_sec<<" "<<t.tv_nsec<<endl;
                //SleepMs(250);
                gridScan(orange);

                char * segImage=segIpl->imageData;
                for( int i=2;i<rawImage->width-2;i++)
                {
                  for(int j=2;j<rawImage->height-2;j++)
                   {




                      //*(imgA+i*width+j)=seg->classifyPixel(yuv);


                     //segImage[j][i] = seg-/>classifyPixel(yuv);//classifyPixel(fIplImageHeader, i, j, type);
                                //cout<<"Test3"<<endl;
                                KSegmentator::colormask_t k = doSeg(i,j);//sImage[j][i];//fIplImageHeader,i,j,kYUVColorSpace
                                //				if (k == 5)
                                // cout << "Pixel at i " << i << " j " << j << " value " << (int) k << endl;
                                int width=rawImage->width;
                                switch (k) {

                                case red://RED
                                    segImage[j * 3 * width + i * 3 + 2] = 255;
                                    segImage[j * 3 * width + i * 3 + 1] = 0;
                                    segImage[j * 3 * width + i * 3] = 0;
                                    break;
                                case blue://BlUE
                                    segImage[j * 3 * width + i * 3 + 2] = 0;
                                    segImage[j * 3 * width + i * 3 + 1] = 0;
                                    segImage[j * 3 * width + i * 3] = 255;
                                    break;
                                case green://GREEN
                                    segImage[j * 3 * width + i * 3 + 2] = 60;
                                    segImage[j * 3 * width + i * 3 + 1] = 120;
                                    segImage[j * 3 * width + i * 3] = 60;
                                    break;
                                case skyblue://SkyBlue
                                    segImage[j * 3 * width + i * 3 + 2] = 0;
                                    segImage[j * 3 * width + i * 3 + 1] = 107;
                                    segImage[j * 3 * width + i * 3] = 228;
                                    break;
                                case yellow://Yellow
                                    segImage[j * 3 * width + i * 3 + 2] = 255;
                                    segImage[j * 3 * width + i * 3 + 1] = 255;
                                    segImage[j * 3 * width + i * 3] = 0;
                                    break;
                                case orange://Orange
                                    segImage[j * 3 * width + i * 3 + 2] = 255;
                                    segImage[j * 3 * width + i * 3 + 1] = 180;
                                    segImage[j * 3 * width + i * 3] = 0;
                                    break;
                                case white://
                                    segImage[j * 3 * width + i * 3 + 2] = 255;
                                    segImage[j * 3 * width + i * 3 + 1] = 255;
                                    segImage[j * 3 * width + i * 3] = 255;
                                    break;
                                default:
                                    segImage[j * 3 * width + i * 3 + 2] = 0;
                                    segImage[j * 3 * width + i * 3 + 1] = 0;
                                    segImage[j * 3 * width + i * 3] = 0;
                                    break;
                                }
                     //cout<< hsl[0]<<","<<hsl[1]<<","<<hsl[2]<<endl<<endl;

                   }
                   //cout<<endl;
                }
                  cvShowImage("win1",segIpl);

                  /*std:: vector <std::string> names;
                   std:: vector <float> pos;
                      names.push_back("HeadYaw");
                   names.push_back("HeadPitch");
                       pos=m->call<vector<float> >("getAngles",names,true);
                        pos[0]=pos[0]+0.1;
                        pos[1]=pos[1]+0.1;
                       m->callVoid("setAngles",names,pos,0.8);*/
                     int k=cvWaitKey(100);

            }
        }
        ~Vision()
        {

        }
        void run(){
          std::cout << " MotionController Runs test" << std::endl;
          }

   private:

        AL::ALPtr<AL::ALMemoryProxy> memory;

        //Ball Detection related
        typedef struct balldata{
                float x, y;
                float r;//Observed radius
        } balldata_t;
        enum colors {
           bad=0, red = 1, blue = 2, green = 3, skyblue = 4, yellow = 5, orange = 6, white = 7, black = 8
        };
        //Extractor Object, to get a new image
        KImageExtractor ext;
        KSegmentator *seg;
        int type;//Colorspace fourCC
        //Raw Input Image
        IplImage *rawImage;
        //Ball Detection related
        std::vector<CvPoint> ballpixels;


        void gridScan(const  KSegmentator::colormask_t color);
        bool calculateValidBall(const CvPoint2D32f center, float radius, KSegmentator::colormask_t c);
		balldata_t  locateBall(vector<CvPoint> cand) ;
		CvPoint traceline(CvPoint start, CvPoint vel, KSegmentator::colormask_t c);
		//Wrapper for seg object
        KSegmentator::colormask_t doSeg(int x, int y)
        {
			if (x >= 0 && x < (rawImage-> width) && y >= 0 && y < (rawImage-> height)) {
				return seg->classifyPixel(rawImage, x, y, type);
				//return sgm->getColor(y, x, (uInt8 *) fIplImageHeader->imageData, false);
			} else {
				return 0;
			}

        };


 };

 #endif
