#include "KImageExtractor.h"
#include "messages/motion.pb.cc"
KImageExtractor::~KImageExtractor()
{
    try
    {

        c->callVoid( "unsubscribe", GVM_name );
    }
    catch (AL::ALError& e)
    {
        throw ALError("KImageExtractor", "Destruct", "Unable to unsubscribe GVM");
    }
}

KImageExtractor::KImageExtractor(AL::ALPtr<AL::ALBroker> pbroker , MessageQueue *mq):Publisher("KImageExtractor"),resolution(VISION_RESOLUTION),cSpace( VISION_CSPACE)
{
    doneSubscribe=false;
    try
    {
        c = pbroker->getProxy( "ALVideoDevice" );
        c->callVoid( "unsubscribe", GVM_name );
        GVM_name = c->call<std::string>( "subscribe", GVM_name, resolution,
                                         cSpace,VISON_FPS );

        doneSubscribe=true;

    }
    catch (AL::ALError& e)
    {
        cout<<e.toString()<<endl;
        throw ALError("KImageExtractor", "Construct ", "Unable to create proxies and subscribe GVM");
    }
    if (mq!=NULL)
        mq->add_publisher(this);
}


/**
 * Fetch a new Image from the hardware, it automatically fixs IplImage and enclosed binary space when needed
 * Use Allocate Image for an initial allocation of an image
 */

struct timespec KImageExtractor::fetchImage(IplImage *img)
{
    struct timespec rt;//Timestamp
    cout<<"KImageExtractor::fetchimage():"<<endl;
    if (doneSubscribe==false)
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

    results = (c->call<ALValue> ("getDirectRawImageRemote", GVM_name));
#else
    results = (c->call<ALValue> ("getImageRemote", GVM_name));
#endif
    if (results.getType() != ALValue::TypeArray && results.getSize() != 7)
    {
        throw ALError("KImageExtractor", "saveImageRemote", "Invalid image returned.");
    }
    //const int size = results[6].getSize();
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

    int size =width*height*nChannels;

    //Fetch TimeStamp;
    rt.tv_sec=(time_t)((int) results[4]);
    rt.tv_nsec=(int)  results[5]*1000L;

    //Change of image data size
    assert(img!=NULL);
    //cout<<img->imageSize<<" "<<size<<endl;
    if (img->imageSize!=size )
    {
        //cout<<img->width<<" "<<img->height<<endl;
        cout<<"KImageExtractor::fetchImage():allocating new imagedata"<<endl;
        //cout<<"Delete old"<<endl;
        //delete img->imageData;
        //img->imageData=NULL;
        cout<<"cvInitImage"<<endl;
        cvInitImageHeader(img,  cvSize(width,height),IPL_DEPTH_8U, nChannels);
        //img->imageData=NULL;
        cout<<" Done"<<endl;
        //img->imageData=(char*)malloc(img->imageSize);
    }


    if (img->imageData != NULL)
    {
        //free( fIplImageHeader->imageData)
        memcpy(img->imageData, (char*) (results[6].GetBinary()), results[6].getSize() * sizeof(unsigned char));
    }
    else
    {
        img->imageData = new char[img->imageSize];
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
    if (!imageIn)
    {
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


    //Change of image data size
    if (img->imageSize!=size*sizeof(char) )
    {
        free(img->imageData);
        cvInitImageHeader(img,  cvSize(width,height),IPL_DEPTH_8U, nChannels);
        img->imageData=NULL;
        //img->imageData=(char*)malloc(img->imageSize);
    }

    if (img->imageData!=NULL)
    {
        //free( fIplImageHeader->imageData);
        memcpy ( img->imageData, (char*) imageIn->getFrame(), size*sizeof(char) );
    }
    else
    {
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

IplImage *KImageExtractor::allocateImage()
{
    if (doneSubscribe==false)
    {
        cout<<"KImageExtractor::allocateImage():WTF"<<endl;
        return NULL;

    }
    //Stupid way, but it works!
    int nChannels=getNumLayersInColorSpace(cSpace);
    int width;
    int height;
    getSizeFromResolution(resolution,width,height);
    cout<<"cvCreteImage"<<endl;
    cout<<width<<" "<<height << " "<< nChannels<<endl;
    return cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nChannels);
}


float KImageExtractor::calibrateCamera(int sleeptime,int exp)
{
    std:: vector <std::string> names;
    std:: vector <float>pos;
    int redchroma,bluechroma,gain,e;
    int rchromaL,bchromaL,gainL,eL;
    int rchromaR,bchromaR,gainR,eR;
    //int expvalue=(e*510.8)/33.0;
    MotionMessage mot;
    mot.add_parameter(0.0f);
    mot.add_parameter(0.0f);
	mot.set_topic("motion");
	mot.set_command("setHead");

    //mot->set_parameter(0, 0.9f * overshootfix * (cx));
    //mot->set_parameter(1, -0.9f * overshootfix * (cy));
    //Publisher::publish( mot);

    float scale=1;
    cout<<"Calibrate Start:"<<endl;
    //names.push_back("HeadYaw");
    //names.push_back("HeadPitch");

    //pos.push_back(0.9);
    //pos.push_back(0.9);

    //m->callVoid("gotoBodyStiffness",1.0,1.0,1);
    //m->callVoid("setStiffnesses",names,pos);
    //m->callVoid("gotoBodyStiffness",1.0,1.0,1);,,

    // cout<<(float)m->call<float>("getAngle","HeadYaw")<<" "<<(float)m->call<float>("getAngle","HeadPitch")<<endl;
    //SleepMs(1000);





    cout<<"Auto:"<<endl;
    try
    {



        c->callVoid( "setParam", kCameraSelectID, 1);

        c->callVoid( "setParam", kCameraExposureCorrectionID,0);
        SleepMs(1);


        //Start auto detect



        //pos.clear();
        mot.set_parameter(0,1.57);
        mot.set_parameter(1,0.22);
        publish(&mot);
        //pos.push_back(1.57);
        //pos.push_back(0.22);

        //m->callVoid("setAngles",names,pos,0.8);
        SleepMs(100);


        c->callVoid( "setParam", kCameraAutoGainID, 1);
        c->callVoid( "setParam", kCameraAutoExpositionID,1);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID,1);

        SleepMs(sleeptime);



        rchromaL=c->call<int>( "getParam", kCameraRedChromaID);
        bchromaL=c->call<int>( "getParam", kCameraBlueChromaID);
        gainL=c->call<int>( "getParam", kCameraGainID);
        eL=c->call<int>( "getParam", kCameraExposureID);


        c->callVoid( "setParam", kCameraAutoGainID, 0);
        c->callVoid( "setParam", kCameraAutoExpositionID,0);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID,0);




        cout<<"Left settings:"<<rchromaL<<" "<<bchromaL<<" "<<" "<< gainL<< endl;

        //cout<<"Scale:"<<scale<<endl;
        //cout<<exp*scale<<" "<<exp<<" " <<endl;
        //GET BOTTOM CAMERA SETTINGS!!!


        //pos.clear();
        //pos.push_back(-1.57);
        //pos.push_back(0.22);
        mot.set_parameter(0,-1.57);
        mot.set_parameter(1,0.22);
        publish(&mot);
        //m->callVoid("setAngles",names,pos,0.8);
        SleepMs(100);


        c->callVoid( "setParam", kCameraAutoGainID, 1);
        c->callVoid( "setParam", kCameraAutoExpositionID,1);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID,1);
        SleepMs(sleeptime);



        //GET BOTTOM CAMERA SETTINGS!!!
        rchromaR=c->call<int>( "getParam", kCameraRedChromaID);
        bchromaR=c->call<int>( "getParam", kCameraBlueChromaID);
        gainR=c->call<int>( "getParam", kCameraGainID);
        eR=c->call<int>( "getParam", kCameraExposureID);

        c->callVoid( "setParam", kCameraAutoGainID, 0);
        c->callVoid( "setParam", kCameraAutoExpositionID,0);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID,0);





        cout<<"Right settings:"<<rchromaR<<" "<<bchromaR<<" "<<" "<< gainR<< endl;
        //============================
        // Final settings!
        //============================
        redchroma=(rchromaL+rchromaR)/2;
        bluechroma=(bchromaL+bchromaR)/2;
        if (eL<eR)
        {
            gain=gainL;
            e=eL;
        }
        else
        {
            gain=gainR;
            e=eR;
        }


        if ((exp*510)/33 <e)
        {
            scale=log((exp*510)/33)/log(e);
            cout<<"Scaling  exposure:"<<(exp*510)/33<<" "<<e<<endl;
            e=(exp*510)/33;
            //cout<<"Old Gain:"<<gain;
            //int hbits=gain&224;
            //int oldgain=gain;
            //gain=((float)(gain&31))/scale;
            //gain=gain|hbits;
            //cout<<"Old Gain:"<<gain<<endl;
            //scale=((float)gain)/oldgain*1.0;

        }
        cout<<"Scaling  exposure:"<<(exp*510)/33<<" "<<e<<endl;
        gain=gainL<gainR?gainL:gainR;
        //redchroma=128-((128.0-redchroma)*0.95);
        //bluechroma=128-((128.0-bluechroma)*0.95);
        //gain=((gain&31)/2)|(gain&32);
        cout<<"Final settings:"<<redchroma<<" "<<bluechroma<<" "<<" "<< gain<< endl;



        c->callVoid( "setParam", kCameraSelectID, 1);
        SleepMs(10);
        //SET BOTTOM CAMERA SETTINGS
        c->callVoid( "setParam", kCameraAutoGainID, 0);
        c->callVoid( "setParam", kCameraAutoExpositionID, 0);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID, 0);


        c->callVoid( "setParam", kCameraBlueChromaID,bluechroma);
        c->callVoid( "setParam", kCameraRedChromaID,redchroma);
        c->callVoid( "setParam", kCameraGainID,gain);
        c->callVoid( "setParam", kCameraExposureID,e);
        c->callVoid( "setParam", kCameraExposureCorrectionID,-6);


        //c->callVoid( "setParam", kCameraSelectID, 0);
        //SleepMs(10);
        c->callVoid( "setParam", kCameraSelectID, 0);
        SleepMs(100);
        //SET TOP CAMERA SETTINGS

        c->callVoid( "setParam", kCameraAutoGainID, 0);
        c->callVoid( "setParam", kCameraAutoExpositionID, 0);
        c->callVoid( "setParam", kCameraAutoWhiteBalanceID, 0);


        c->callVoid( "setParam", kCameraBlueChromaID,bluechroma);
        c->callVoid( "setParam", kCameraRedChromaID,redchroma);
        c->callVoid( "setParam", kCameraGainID,gain);
        //c->callVoid( "setParam", kCameraAutoExpositionID,2);
        c->callVoid( "setParam", kCameraExposureID,e);
        c->callVoid( "setParam", kCameraExposureCorrectionID,-6);



        SleepMs(100);
        //Start with bottom cam
        c->callVoid( "setParam", kCameraSelectID, 1);


    }
    catch (AL::ALError &e)
    {
        cout<<"No Autosettings available ... webots?"<<endl;
        std::cout << e.toString() << std::endl;
        //exit(0);
    }
    //seg->setLumaScale(0.338914);
    cout<<"done"<<endl;
    //pos.clear();
    //pos.push_back(0);
    //pos.push_back(-0.1);
    //m->callVoid("setAngles",names,pos,0.8);
    mot.set_parameter(0,0);
    mot.set_parameter(1,-0.1);
    publish(&mot);
    return scale;
}
