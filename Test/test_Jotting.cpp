//
// Created by buyi on 17-10-21.
//

#include "Camera.h"


double Eigenceil(double x)
{
    return ceil(x);
}

int main(int argc, char **argv)
{
    //google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    //! Have same meaning
    //FLAGS_logtostderr = true;
    //FLAGS_stderrthreshold = 0;
    FLAGS_log_dir = "./log";

    Eigen::Vector2d tTestCeil(1.1, 1.6);
    Eigen::Vector2d tTestFinal = tTestCeil.unaryExpr(std::ptr_fun(Eigenceil));

    double aa = ceil(tTestCeil(0));
    double bb = ceil(tTestCeil(1));

    cv::Mat tImage1, tImage2;
    tImage1 = cv::imread("1.png");
    tImage2 = cv::imread("2.png");
    cv::imwrite("1.ppm", tImage1);
    cv::imwrite("2.ppm", tImage2);

    /*
    Eigen::MatrixXd tMatTest(10000, 10000);
    for (int k = 0; k < 10000; ++k)
    {
        for (int i = 0; i < 10000; ++i)
        {
            tMatTest(k,i) = 1.0*k + 0.22;
        }
    }
    std::cout << tMatTest(999,100) << std::endl;

    DSDTM::TicToc tc;
    for (int k = 0; k < 10000; ++k)
    {
        for (int i = 0; i < 10000; ++i)
        {
            tMatTest(k,i) = ceil(tMatTest(k,i));
        }
    }
    std::cout << tc.toc() << std::endl;
    std::cout << tMatTest(999,100) << std::endl;

    for (int k = 0; k < 10000; ++k)
    {
        for (int i = 0; i < 10000; ++i)
        {
            tMatTest(k,i) = 1.0*k + 0.22;
        }
    }
    std::cout << tMatTest(999,100) << std::endl;

    //Eigen::MatrixXd tMatTestCeil(10000, 10000);
    DSDTM::TicToc tcc;
    Eigen::MatrixXd tMatTestCeil = tMatTest.unaryExpr(std::ptr_fun(Eigenceil));
    std::cout << tcc.toc() << std::endl;
    std::cout << tMatTestCeil(999,100) << std::endl;
    */

    Eigen::MatrixXf tWrapMat(2, 100);
    Eigen::MatrixXf tColBlock(1, 10);
    tColBlock << -5, -4, -3, -2, -1, 0, 1, 2, 3, 4;
    tWrapMat = Eigen::MatrixXf::Zero(2, 100);
    int tindex = 1;
    for (int i = -5; i < 5; ++i, ++tindex)
    {
        tWrapMat.block(0, (tindex-1)*10, 1, 10) = tColBlock;
        tWrapMat.block(1, (tindex-1)*10, 1, 10) = Eigen::MatrixXf::Ones(1, 10)*i;
    }
    std::cout << tWrapMat << std::endl;

    //! Convert c++ array into Eigen vector
    double tPt3[3];
    Eigen::Map<Eigen::Vector3d> tPtv3(tPt3);
    tPtv3 << 1, 2, 3;
    for (int k = 0; k < 3; ++k)
    {
        std::cout<< tPt3[k] << std::endl;
    }

    //! Resize the matrix without change former value
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> tmpMat;
    tmpMat.resize(3, 2);
    tmpMat.col(0) << 1, 2, 3;
    tmpMat.col(1) << 4, 5, 6;
    std::cout << tmpMat << std::endl;
    tmpMat.conservativeResize(2, 2);
    std::cout << tmpMat << std::endl;

    //! Test the select API in Eigen
    std::cout << "Test the select API in Eigen" << std::endl;
    Eigen::Vector3d tVec3(1, 2, 3);
    Eigen::VectorXd tVec3Out = Eigen::VectorXd((tVec3.array() >= 2).select(tVec3.array().inverse(), 0));
    std::cout << tVec3Out << std::endl;
    Eigen::MatrixXd tDigInv = tVec3.asDiagonal().inverse();
    std::cout << tDigInv << std::endl;

    //! Test the segment API in Eigen
    std::cout << "Test the segment API in Eigen" << std::endl;
    Eigen::Matrix<double, 6, 1> tVec6;
    tVec6 << 1, 2, 3, 4, 5, 6;
    Eigen::VectorXd tVec6Seg = tVec6.segment(2, 3);
    std::cout << tVec6Seg << std::endl;

    //! Test noalias
    Eigen::Matrix2d ta, tb, tc; ta << -1,2,3,-4; tb << 5,6,7,8;
    tc.noalias() = ta * tb; // this computes the product directly to c
    std::cout << tc << std::endl;
    std::cout << ta.cwiseAbs().maxCoeff() << std::endl;

    //! Test cv::Mat and eigen elements
    cv::Mat L = (cv::Mat_<double>(3,3) << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7, 8, 9);
    cv::Mat_<double> tMat_cv = L;
    Eigen::Matrix3d tMat_eigen;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            tMat_eigen(j, i) = tMat_cv(i, j);
    std::cout << tMat_cv <<std::endl;
    std::cout << tMat_eigen <<std::endl;

     //!Test NULL Sophus SE3
    Sophus::SE3 tNullSE3;
    if(tNullSE3.matrix().isZero())
    {
        tNullSE3 = DSDTM::IdentitySE3;
    }

    //! Test Eigen squaredNorm
    Eigen::Vector2d tSquaredNorm(1,2);
    double tSNfinal = tSquaredNorm.squaredNorm();

    //! Test quaterion multiplication a scalar
    Eigen::Quaterniond MulQuat(1, 2, 3, 4);
    MulQuat = MulQuat.coeffs()*2;

    std::vector<int> a;
    for (int i = 0; i < 10; ++i)
    {
        a.push_back(i);
    }
    a.assign(10, 0);
    a.resize(10, 0);

    int b = a[a.size()-1];
    LOG(INFO)<< "b = " << b;
    
    int c = static_cast<int>(5.10);
    LOG(WARNING)<< "c = " << c;

    LOG(ERROR)<< "finished";
    //LOG(FATAL)<< "finished";

    cv::Mat ModelIMg(480, 640, CV_8UC1, cv::Scalar(255, 255, 255));
    cv::cvtColor(ModelIMg, ModelIMg, CV_GRAY2BGR);
    for (int j = 0; j < 10; ++j)
    {
        cv::line(ModelIMg, cv::Point2f(64*j,0 ), cv::Point2f(64*j,479 ), cv::Scalar(0, 255, 0), 2, 8);
        cv::line(ModelIMg, cv::Point2f(0, 48*j), cv::Point2f(639, 48*j), cv::Scalar(0, 255, 0), 2, 8);
    }
    cv::imwrite("model.jpg", ModelIMg);
    cv::namedWindow("Model");
    cv::imshow("Model", ModelIMg);
    cv::waitKey(1);
    b++;

//! Release--NDEBUG     Debug--NDEBUG
#ifdef NDEBUG
    std::cout <<"aa" <<std::endl;
#else
    std::cout <<"bb" <<std::endl;
#endif
    return 0;
}