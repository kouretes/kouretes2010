#include "vision.h"

#define inbounds(x,y) ( ((x)>0 &&(y)>0)&&((x)<rawImage->width-1&&(y)<rawImage->height-1) )
#define CvDist(pa,pb) sqrt(((pa).x-(pb).x )*((pa).x-(pb).x )+((pa).y-(pb).y )*((pa).y-(pb).y ) )
#define MAXSKIP 5
#define GLOBALSKIP  15

void Vision::gridScan(const KSegmentator::colormask_t color) {
	//Horizontal + Vertical Scan
	ballpixels.clear();
	int diff = 0;
	int diffprev = -1;
	int min1 = -1;
	int min2 = -1;
	CvPoint tmpPoint;
	int prevMinAth = 0;

	int away = 80;
	int flag = 0;
	int foundcolor = 0; //0 not found negative found but lost possitive found

	int points[rawImage->width];
	int continues = 0;
	int threshold = 10;
	int step = 4;
	int ystep = 3;
	unsigned int greenpixel;
	KSegmentator::colormask_t tempcolor;
	int ballpixel = -1;
	unsigned int whitegreenpixelsthreshold = 2;
	unsigned int cntwhitegreenpixels = 0;
	int j;
	for (int i = 0; i < rawImage->width; i = i + step) {
		//Thru Horizon Possibly someday
		points[i] = -1;
		continues = 0;
		cntwhitegreenpixels = 0;
		ballpixel = -1;

		for (j = rawImage->height - 2; j > 0; j = j - ystep) {
			//image start from top left
			tempcolor = doSeg(i, j);

			if (ballpixel < 0) {
				if (tempcolor == white || tempcolor == green)
					cntwhitegreenpixels++;
				if (tempcolor == orange && cntwhitegreenpixels >= whitegreenpixelsthreshold) {
					tmpPoint.x = i;
					tmpPoint.y = j;
					ballpixels.push_back(tmpPoint);
					ballpixel = j;
				}
			}
			if (tempcolor == color) {
				//				if (j == (height - 3) || i == 1 || i == (width - 1)) // to close to the frame of the image
				//					break;
				//					///cout << "Color " << color << " found at w:" << i << " h: " << j << endl;
				continues++;
				//make sure that this color continues little more pixels above and it is not segmentation error
				if (continues == threshold) {
					points[i] = j + threshold * ystep - 1;
					break;
				}
			} else if (continues > 0) {
				continues = 0;
			} else { //TODO: Below is gui related stuff
				//CvPoint point = cvPoint(i, j);
				//cvLine(fIplSegImage, point, point, cvScalar(0, 40 * color, 50 * color), 1, 0, 0);
			}
		}
		for (; j > 0; j = j - ystep) {
			tempcolor = doSeg(i, j);

			if (ballpixel < 0) {
				if (tempcolor == white || tempcolor == green)
					cntwhitegreenpixels++;
				if (tempcolor == orange && cntwhitegreenpixels >= whitegreenpixelsthreshold) {
					tmpPoint.x = i;
					tmpPoint.y = j;
					ballpixels.push_back(tmpPoint);
					ballpixel = j;
					break;
				}
			}
		}
	}
	cout << "Ballpixelsize:" << ballpixels.size() << endl;
	balldata_t b = locateBall(ballpixels);
	cout << b.x << " " << b.y << " " << b.r << endl;
#define  VFov 34.8f
#define  HFov 46.4f
#define TO_RAD 0.01745329f
	float x = (0.5 - (double) (b.x) / (double) rawImage->width) * HFov * TO_RAD;
	float y = (0.5 - (double) (b.y) / (double) rawImage->height) * VFov * TO_RAD;
	float r = b.r * (HFov / (double) rawImage->width);
	if (b.r > 0) {
		memory->insertData("kouretes/Ball/cx", x); // change in Head Yaw
		memory->insertData("kouretes/Ball/cy", y); // change in Head Pitch
		memory->insertData("kouretes/Ball/radius", r); // change in Head Pitch
		memory->insertData("kouretes/Ball/found", 1.0f); // change
	} else {
		memory->insertData("kouretes/Ball/found", .0f); // change
	}
}

/**
 * Hard Decision: Is it good enough for a ball?
 */
bool Vision::calculateValidBall(const CvPoint2D32f center, float radius, KSegmentator::colormask_t c) {
	return true;
	unsigned int ttl = 0, gd = 0;
	float innerrad = radius * 0.707;
	float ratio;
	//Inner circle
	for (int i = center.x - innerrad; i <= center.x + innerrad; i++)
		for (int j = center.y - innerrad; j <= center.y + innerrad; j++) {
			if (!inbounds(i,j))
				continue;
			if (doSeg(i, j) == c)
				gd++;
			ttl++;
		}
	ratio = ((float) gd) / ttl;
	if (ratio < 0.75)
		return false;
	//Outer circle
	gd = 0;
	ttl = 0;
	for (int i = center.x - radius; i <= center.x + radius; i++)
		for (int j = center.y - radius; j <= center.y + radius; j++) {

			if (!inbounds(i,j))
				continue;
			if (i > center.x - innerrad && i < center.x + innerrad)
				continue;

			if (j > center.y - innerrad && j < center.y + innerrad)
				continue;

			if (doSeg(i, j) == c)
				gd++;
			ttl++;
		}
	ratio = ((float) gd) / ttl;
	if (ratio < 0.3 || ratio > 0.7)
		return false;

	return true;

}

Vision::balldata_t Vision::locateBall(vector<CvPoint> cand) {
	//Skip first/last row/col
	vector<balldata_t> history;

	vector<CvPoint>::iterator i;
	//For all detected points
	cout << "locateball" << endl;
	for (i = cand.begin(); i != cand.end(); i++) {
		vector<CvPoint> points;
		vector<balldata_t>::iterator bd = history.begin();
		while (bd != history.end() && i != cand.end()) {
			if (CvDist(*bd,*i) < (*bd).r) {
				i++;//Skip pixels
				if (i == cand.end())
					break;
				bd = history.begin();
			} else
				bd++;
		}
		if (i == cand.end())
			break;
		if (!inbounds((*i).x,(*i).y))
			continue;
		CvPoint bottom = traceline((*i), cvPoint(0, 1), orange);

		CvPoint top = traceline((*i), cvPoint(0, -1), orange);
		if (!inbounds(top.x,top.y))
			continue;
		points.push_back(bottom);
		points.push_back(top);

		CvPoint middle;
		middle.x = top.x;
		middle.y = (top.y + bottom.y) / 2;

		CvPoint2D32f center;
		center.x = 0;
		center.y = 0;
		cout << "Trace X " << (*i).x << " y " << "Y " << (*i).y << endl;
		CvPoint pleft = traceline(middle, cvPoint(-1, 0), orange);
		if (inbounds(pleft.x,pleft.y))
			points.push_back(pleft);

		CvPoint pright = traceline(middle, cvPoint(1, 0), orange);
		if (inbounds(pright.x,pright.y))
			points.push_back(pright);

		CvPoint topright = traceline(middle, cvPoint(1, -1), orange);
		if (inbounds(topright.x,topright.y))
			points.push_back(topright);

		CvPoint topleft = traceline(middle, cvPoint(-1, -1), orange);
		if (inbounds(topleft.x,topleft.y))
			points.push_back(topleft);

		CvPoint bottomright = traceline(middle, cvPoint(1, 1), orange);
		if (inbounds(bottomright.x,bottomright.y))
			points.push_back(bottomright);

		CvPoint bottomleft = traceline(middle, cvPoint(-1, 1), orange);
		if (inbounds(bottomleft.x,bottomleft.y))
			points.push_back(bottomleft);
		//TODO: Something smarter? like circle from 3 points?
		//Iterate for center
		for (unsigned int j = 0; j < points.size(); j++) {
			center.x += points[j].x;
			center.y += points[j].y;
		}
		center.x /= points.size();
		center.y /= points.size();
		float radius = 0;
		cout << "Find Center:" << center.x << " " << center.y << " " << endl;
		//Iterate for radius
		for (unsigned int j = 0; j < points.size(); j++) {
			radius += CvDist(center,points[j]);//sqrt((center.x-points[i].x)*)(center.x-points[i].x)+(center.y-points[i].y)*)(center.y-points[i].y));
			//center.y+=points[i].y;
		}
		radius /= points.size();
		/////cout << "Wtf" << endl;
		balldata_t newdata;
		newdata.x = center.x;
		newdata.y = center.y;
		newdata.r = radius;
		history.push_back(newdata);

	}
	vector<balldata_t>::iterator bd = history.begin();
	balldata_t best;
	best.r = 0;
	///cout << "BEST found" << endl;
	while (bd != history.end()) {
		CvPoint2D32f t;
		t.x = (*bd).x;
		t.y = (*bd).y;
		if ((*bd).r > best.r && calculateValidBall(t, (*bd).r, (KSegmentator::colormask_t) orange))
			best = *bd;
		cout << best.x << " " << best.y << " " << endl;
		bd++;
	}
	return best;
	/*ALValue ret;

	 ret.arraySetSize(0);
	 if (best.r == 0)
	 return ret;
	 ret.arrayPush("Ball");
	 ret.arrayPush(2 * best.r);
	 vector<std::string> nameyeah;
	 nameyeah.push_back("HeadYaw");
	 vector<float> retangles = motion->call<vector<float> > ("getAngles", nameyeah, true);
	 //double headpitch = retangles[0];// motion->getAngles("HeadPitch",true);
	 ALValue headYawAngle2 = memprxy->getData(AL::ALValue("Device/SubDeviceList/HeadYaw/Position/Sensor/Value"));//motion->getAngles(AL::ALValue("HeadYaw"), true);;//motion->getAngles("HeadYaw",true);
	 float headYawAngle = retangles[0];//motion->getAngles("HeadYaw",true);
	 ///cout << "Head Yaw " << (float) headYawAngle << "Headyaw sketo " << (float)headYawAngle2<< endl;
	 //Bearing Warning not thru horizon! TODO
	 float tempdir = (headYawAngle - (0.5 - (double) (best.x) / (double) width) * HFov * TO_RAD);

	 Acx[orange] = (0.5 - (double) (best.x) / (double) width) * HFov * TO_RAD;
	 Acy[orange] = (0.5 - (double) (best.y) / (double) height) * VFov * TO_RAD;
	 ///cout << " Ball cx, cy" << tempdir << " Ball Diameter pixel" << (float)ret[1]<< endl;

	 ret.arrayPush(tempdir * 1000);
	 return ret;*/

}

CvPoint Vision::traceline(CvPoint start, CvPoint vel, KSegmentator::colormask_t c) {

	int skipcount = 0;
	int globalcount = 0;
	CvPoint curr = start;
	CvPoint latestValid = start;
	/////cout << "traceline:"<<start.x<<" "<<start.y<<endl;
	while (inbounds(curr.x,curr.y)) {
		if (doSeg(curr.x, curr.y) != c) {
			skipcount++;
			globalcount++;
		} else {
			latestValid = curr;
			skipcount = 0;
			globalcount = 0;
		};

		if (skipcount > MAXSKIP || globalcount > GLOBALSKIP)
			break;
		curr.x += vel.x;
		curr.y += vel.y;

	}
	if (!inbounds(latestValid.x,latestValid.y)) {
		latestValid.x = -1;
		latestValid.y = -1;
	}
	//cout<<"ret"<<latestValid.x<<" "<<latestValid.y<<endl;
	return latestValid;
}
