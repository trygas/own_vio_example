//
// Created by liu on 19-12-6.
//

#ifndef VIO_EXAMPLE_FEATUREMANAGER_H
#define VIO_EXAMPLE_FEATUREMANAGER_H

#include "Parameters.h"

#include <Eigen/Dense>
#include <GSLAM/core/GSLAM.h>

#include <iostream>
#include <map>
#include <list>
#include <stdlib.h>

class FeaturePerFrame
{
public:
    FeaturePerFrame(const Eigen::Matrix<double, 7, 1> &point, double td){
        _point.x() = point(0);
        _point.y() = point(1);
        _point.z() = point(2);
        _uv.x() = point(3);
        _uv.y() = point(4);
        _velocity.x() = point(5);
        _velocity.y() = point(6);
        _cur_td = td;
    }

    double _cur_td;
    Eigen::Vector3d _point;
    Eigen::Vector3d _uv;
    Eigen::Vector3d _velocity;
    double _z;
    bool _is_used;
    double _parallax;
    Eigen::MatrixXd _A;
    Eigen::VectorXd _b;
    double _dep_gradient;
};

class FeaturePerId
{
public:
    FeaturePerId(const int feature_id, const int start_frame): _feature_id(feature_id), _start_frame(start_frame),
                                                               _used_num(0), _estimated_depth(-1.0), _solve_flag(0)
    {}

    const int _feature_id;
    int _start_frame;
    std::vector<FeaturePerFrame> _feature_per_frame;
    int _used_num;
    bool _is_outlier;
    double _estimated_depth;
    bool _solve_flag;// 0: haven't solve yet, 1: solve success, 2: solve fail
};

class FeatureManager
{
public:
    FeatureManager(){}
    FeatureManager(std::vector<Eigen::Matrix3d> &Rs);
    bool AddFeatureCheckParallax(int frame_count,
                                 const std::map<int, std::vector<std::pair<int, Eigen::Matrix<double, 7, 1>>>> &image,
                                 double td);
    void ClearState(){
        _feature.clear();
    }

private:
    double CompensatedParallax(const FeaturePerId &it_per_id, int frame_count);

private:
    int _last_track_num;

    std::list<FeaturePerId> _feature;
    std::vector<Eigen::Matrix3d> _R;
    std::vector<Eigen::Matrix3d> _ric;
};


#endif //VIO_EXAMPLE_FEATUREMANAGER_H