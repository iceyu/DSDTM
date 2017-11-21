//
// Created by buyi on 17-11-9.
//
#include "Keyframe.h"


namespace DSDTM
{
    KeyFrame::KeyFrame(Frame &tframe):
            mFrame(&tframe), mT_c2w(tframe.Get_Pose()), mvMapPoints(tframe.mvMapPoints),
            mvFeatures(tframe.mvFeatures), mpObservation(tframe.mpObservation)
    {

    }

    KeyFrame::~KeyFrame()
    {

    }

    void KeyFrame::Add_MapPoint(MapPoint *tMPoint)
    {
        mvMapPoints.push_back(tMPoint);

    }

    void KeyFrame::Add_Observations(long int tId, MapPoint *tMPoint)
    {
        mpObservation.insert(std::pair<long int, MapPoint*>(tId, tMPoint));
    }




} //namesapce DSDTM