/*
Copyright 2013 BIT OTISMED
*/

#ifndef ProjMatch_H
#define ProjMatch_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;

class ProjMatch
{
public:
	ProjMatch();
	~ProjMatch();

    vector<vector<KeyPoint> > keypoints_1, keypoints_2;
	vector<Mat> descriptors_1, descriptors_2;
    vector<vector<DMatch> > GoodMatches;
	vector<int> BestPair;
	char path1[200], path2[200];
	int SizeA, SizeB;

	void SetPath(char str1[], char str2[]);
	void SetSize(int a, int b);
	void Update(int i);
	void KeypointsExtract();
	void ShowMatch(int a, int b);

private:
	void ImgBlur();
	double Match(int a, int b);
};

#endif
