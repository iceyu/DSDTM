// Copyright (c) 2016 Kwang Moo Yi.
// All rights reserved.

// This  software  is  strictly   for  non-commercial  use  only.  For
// commercial       use,       please        contact       me       at
// kwang.m<dot>yi<AT>gmail<dot>com.   Also,  when  used  for  academic
// purposes, please cite  the paper "Detection of  Moving Objects with
// Non-stationary Cameras in 5.8ms:  Bringing Motion Detection to Your
// Mobile Device,"  Yi et  al, CVPRW 2013  Redistribution and  use for
// non-commercial purposes  in source  and binary forms  are permitted
// provided that  the above  copyright notice  and this  paragraph are
// duplicated  in   all  such   forms  and  that   any  documentation,
// advertising  materials,   and  other  materials  related   to  such
// distribution and use acknowledge that the software was developed by
// the  Perception and  Intelligence Lab,  Seoul National  University.
// The name of the Perception  and Intelligence Lab and Seoul National
// University may not  be used to endorse or  promote products derived
// from this software without specific prior written permission.  THIS
// SOFTWARE IS PROVIDED ``AS IS''  AND WITHOUT ANY WARRANTIES.  USE AT
// YOUR OWN RISK!

#ifndef	_MCDWRAPPER_CPP_
#define	_MCDWRAPPER_CPP_

#include <ctime>
#include <cstring>
#include "MCDWrapper.h"
#include "params.h"

#if defined _WIN32 || defined _WIN64
int gettimeofday(struct timeval *tp, int *tz)
{
	LARGE_INTEGER tickNow;
	static LARGE_INTEGER tickFrequency;
	static BOOL tickFrequencySet = FALSE;
	if (tickFrequencySet == FALSE) {
		QueryPerformanceFrequency(&tickFrequency);
		tickFrequencySet = TRUE;
	}
	QueryPerformanceCounter(&tickNow);
	tp->tv_sec = (long)(tickNow.QuadPart / tickFrequency.QuadPart);
	tp->tv_usec = (long)(((tickNow.QuadPart % tickFrequency.QuadPart) * 1000000L) / tickFrequency.QuadPart);

	return 0;
}
#else
#include <sys/time.h>
#endif

MCDWrapper::MCDWrapper():frm_cnt(0)
{

}

MCDWrapper::~MCDWrapper()
{
}

void
MCDWrapper::Init()
{
    frm_cnt = 0;
    BGModel.init(imgGray);
}

cv::Mat MCDWrapper::Run(cv::Mat ColorImg, double *H, cv::Mat tMask)
{
    BGModel.mMask = tMask;

    if(frm_cnt==0)
    {
        imgGray = cvCreateImage(cvSize(ColorImg.cols, ColorImg.rows), IPL_DEPTH_8U, 1);
        detect_img = cvCreateImage(cvSize(ColorImg.cols, ColorImg.rows), IPL_DEPTH_8U, 1);
        imgGrayPrev = cvCreateImage(cvSize(ColorImg.cols, ColorImg.rows), IPL_DEPTH_8U, 1);

        BGModel.init(imgGray);
    }

    frm_cnt++;

    timeval tic, toc, tic_total, toc_total;
    float rt_preProc;	// pre Processing time
    float rt_motionComp;	// motion Compensation time
    float rt_modelUpdate;	// model update time
    float rt_total;		// Background Subtraction tim

    IplImage copy = ColorImg;
    IplImage *tIpltmp = &copy;
    cvSmooth(tIpltmp, imgGray, CV_MEDIAN, 5);

    BGModel.motionCompensate(H);

    gettimeofday(&tic, NULL);
    BGModel.update(detect_img);
    gettimeofday(&toc, NULL);
    rt_modelUpdate = (float)(toc.tv_usec - tic.tv_usec) / 1000.0;
    std::cout<< rt_modelUpdate << "ms" << std::endl;


    cv::Mat matimg = cv::Mat(detect_img);
    cv::Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    cv::morphologyEx(matimg, matimg, MORPH_OPEN, element);

    matimg.convertTo(matimg, CV_8SC1);
    cv::namedWindow("Moving_Detect");
    cv::imshow("Moving_Detect", matimg);
    cv::waitKey(1);



    cvCopy(imgGray, imgGrayPrev);

    cv::Mat tZero(matimg.size(), CV_8SC1, cv::Scalar::all(0));
    tZero = tZero - matimg;
    return tZero;

}

#endif				// _MCDWRAPPER_CPP