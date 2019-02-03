/*
   ContourPairClass.h
   @author - Maddie, Anika, Angela
   @version - February 2, 2019
*/

// Includes
#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#include <networktables/NetworkTable.h>
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

#ifndef CONTOURPAIR_H_
#define CONTOURPAIR_H_

class ContourPair
{
 public:
   ContourPair(Contour c1, Contour c2);
   float getDist(float lengthWidth, int widthThreshold, int rectHeight, float frameHeight, float frameWidth, int rectWidth, float leftCornerDist, float rightCornerDist, float offsetInches);
   float getDistFromBot();
   float getOffset(Rect leftRect, Rect rightRect, float T_INCHES_BOTH_WIDTH, int FOV_PIXELS_WIDTH);
 private:
   vector<Contour> contourPair;
   Point centerOfTapes;
   // variables for distance calculation
   float distance;
   float fovHeight;
   float fovWidth;
   float fovDiagonal;
   float verticalDistanceToTape;
   float distanceToTape;
   float horizDistanceToTapeLeft;
   float horizDistanceToTapeRight;
   float avgDistToTape;
   float distanceFromRobot;

   // variables for offset calculation
   float offset;
   float pixelsToInches;
   float tapeCenter;
   float localOffset;
};
#endif