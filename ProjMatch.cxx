/*
Copyright 2013 BIT OTISMED
*/

#include "ProjMatch.h"

ProjMatch::ProjMatch()
{
}

ProjMatch::~ProjMatch()
{
}

void ProjMatch::SetPath(char str1[], char str2[])
{
	strcpy(path1, str1);
	strcpy(path2, str2);
}

void ProjMatch::SetSize(int a, int b)
{
	SizeA = a;
	SizeB = b;
}

void ProjMatch::Update(int i)
{
	int j;
	double mind;
	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	mind = 0xFFFFFF;
	for (j = 0; j < SizeB; j++)
	{
		if (Match(i, j) < mind)
		{
			mind = Match(i, j);
			BestPair[i] = j;
		}
	}
	matcher.match( descriptors_1[i], descriptors_2[BestPair[i]], matches );
	GoodMatches[i].clear();
	for( int j = 0; j < descriptors_1[i].rows; j++ )
	{ 

		if (matches[j].distance < 0.25)
			GoodMatches[i].push_back(matches[j]);
	}

}

void ProjMatch::ImgBlur()
{
	char path[100];
	int i;
	Mat img, dst;

	for (i = 0; i < SizeA; i++)
	{
		sprintf(path, "%s/%d.bmp", path1, i);
		img = imread( path, CV_LOAD_IMAGE_GRAYSCALE );
		GaussianBlur(img, dst, Size(5,5), 0, 0);
		sprintf(path, "%s/blur%d.bmp", path1, i);
		imwrite(path, dst);
	}
	for (i = 0; i < SizeB; i++)
	{
		sprintf(path, "%s/%d.bmp", path2, i);
		img = imread( path, CV_LOAD_IMAGE_GRAYSCALE );
		GaussianBlur(img, dst, Size(5,5), 0, 0);
		sprintf(path, "%s/blur%d.bmp", path2, i);
		imwrite(path, dst);
	}
}

void ProjMatch::KeypointsExtract()
{
	Mat img;
	char path[100];
	int minHessian = 400, i;
	SurfDescriptorExtractor extractor;
	vector<KeyPoint> keypoint;
	vector<DMatch> matches;
	Mat descriptor;

	keypoints_1.clear();
	keypoints_2.clear();
	descriptors_1.clear();
	descriptors_2.clear();
	GoodMatches.clear();
	BestPair.clear();

	for (i = 0; i < SizeA; i++)
	{
		sprintf(path, "%s/%d.bmp", path1, i);
		img = imread( path, CV_LOAD_IMAGE_GRAYSCALE );	

		//-- Step 1: Detect the keypoints using SURF Detector
		SurfFeatureDetector detector( minHessian );
		detector.detect(img, keypoint);
		keypoints_1.push_back(keypoint);

		//-- Step 2: Calculate descriptors (feature vectors)
		extractor.compute( img, keypoints_1[i], descriptor);
		descriptors_1.push_back(descriptor);

		BestPair.push_back(i);
		GoodMatches.push_back(matches);
	}

	for (i = 0; i < SizeB; i++)
	{
		sprintf(path, "%s/%d.bmp", path2, i);
		img = imread( path, CV_LOAD_IMAGE_GRAYSCALE );	

		//-- Step 1: Detect the keypoints using SURF Detector
		SurfFeatureDetector detector( minHessian );
		detector.detect(img, keypoint);
		keypoints_2.push_back(keypoint);

		//-- Step 2: Calculate descriptors (feature vectors)
		extractor.compute( img, keypoints_2[i], descriptor);
		descriptors_2.push_back(descriptor);
	}
	cout<<"Extract Down"<<endl;
}

void ProjMatch::ShowMatch(int a, int b)
{
	char path[100];

	sprintf(path, "%s/%d.bmp", path1, a);
	Mat img_1 = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(path, "%s/%d.bmp", path2, b);
	Mat img_2 = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_1[a], descriptors_2[b], matches );

	std::vector< DMatch > good_matches;


	for( int i = 0; i < descriptors_1[a].rows; i++ )
	{ 
		double dist = matches[i].distance;
		if (dist < 0.25)
			good_matches.push_back(matches[i]);
	}

	Mat img_matches;
	drawMatches( img_1, keypoints_1[a], img_2, keypoints_2[b], good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//imwrite(path, img_matches);
	imshow( "Matches", img_matches);
	waitKey(0);
}

/** @function Match */
double ProjMatch::Match(int a, int b)
{
	double ret = 0;

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;

	vector< DMatch > matches;
	matcher.match( descriptors_1[a], descriptors_2[b], matches );


	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_1[a].rows; i++ )
	{ 
		double dist = matches[i].distance;
		ret += dist;
	}
	//showmatch(a, b);
	return ret;
}