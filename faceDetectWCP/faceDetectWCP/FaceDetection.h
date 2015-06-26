#pragma once

#include <opencv/cv.h>
#include <vector>
using namespace std;



#ifdef FACEDETECTION_EXPORTS
#define FACEDETECTION_API __declspec(dllexport)
#else
#define FACEDETECTION_API __declspec(dllimport)
#endif

#define FACES_MAX 50


// 此类是从 FaceDetection.dll 导出的
class FACEDETECTION_API CFaceDetection 
{
public:

	CFaceDetection( const char* config = "face_config.xml" ); 
	int GetFaceRects(const cv::Mat &pImg_src,CvRect faceRects[FACES_MAX],double faceWeights[FACES_MAX]);
private:
	bool LoadSettingParams(const char* config);
	int GetFaceRects_vector(const cv::Mat &pImg_src,vector<CvRect> &faceRects,std::vector<double>& faceWeights);
private:
	float m_dFactor;
	float m_dScaleFactor; 
	int m_nMinNeighbors; 
	int m_nWinSize_Min; 
	int m_nWinSize_Max; 
	float m_dGroup_EPS; 
	string m_strClassifier_Model;
};

