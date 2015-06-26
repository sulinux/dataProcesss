#include "FaceDetection.h"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace std;

bool imageRotate( cv::Mat& oriImg, float scale, vector<int> faceRects, cv::Mat& outImg )
{
	int imgW = oriImg.cols, imgH = oriImg.rows;
	int toplx, toply, faceW, faceH;
	toplx = faceRects[0] ;
	toply = faceRects[1] ;
	faceW = faceRects[2];
	faceH = faceRects[3];
	cv::Point2f cx( toplx + faceW/2.0, toply+faceH/2.0 );
	cv::Mat R = getRotationMatrix2D( cx, scale, 1);
	cv::warpAffine( oriImg, outImg, R, cv::Size(oriImg.cols, oriImg.rows));
	return true;
}

int checkFcaeRect( cv::Mat& Imgs,CvRect faceRectRegion)
{
	int imW = Imgs.cols, imH = Imgs.rows;
	int x = faceRectRegion.x;
	int y = faceRectRegion.y;
	int w = faceRectRegion.width;
	int h = faceRectRegion.height;
	if( x > 0 && y > 0 && x+w <= imW && y+h <= imH && x + w >= 0 && y + h >= 0  )
		return 1;
	return 0;
}
CvRect imageMove( vector<int> faceRects, int iscale )
{   
	CvRect facer;
	float scale = iscale*0.01;
	int toplx, toply, faceW, faceH;
	toplx = faceRects[0] ;
	toply = faceRects[1] ;
	faceW = faceRects[2] ;
	faceH = faceRects[3] ;
	toplx = toplx - scale*faceW ;
	toply = toply - scale*faceH ;
	faceW = faceW * ( 1+scale ) ;
	faceH = faceH * ( 1+scale ) ;

	facer.x = toplx ;
	facer.y = toply ;
	facer.width = faceW ;
	facer.height = faceH ;
	return facer ;
}
int main()
{  
	CFaceDetection FaceDetection;
	ifstream infilename, rfile;
	infilename.open("E:\\Data_Set\\colorferect\\fa\\Path_Images.txt");
	string line, temp;
	
	CvRect faceRects[50];
	double faceWeights[50];
	string fileName, name;
	int LocationF, LocationL, k;
	int rm;
	vector<int> lines;
	vector<int> rmove;
	cv::Mat sorImgs, grayImgs, faceRegion, rotateface;
	while(getline( infilename, fileName ))
	{
		rfile.open("config.txt");
		sorImgs = cv::imread(fileName);
		LocationF = fileName.find_last_of('\\');
		LocationL = fileName.find_last_of('\.');
		name = fileName.substr( LocationF, LocationL - LocationF );
		cout<<name<<endl;
		if (sorImgs.empty())
		{
			break;
		}
		if (sorImgs.channels() == 3)
			cv::cvtColor( sorImgs, grayImgs, cv::COLOR_BGR2GRAY);
		else
		{
			grayImgs = sorImgs.clone();
		}
		int nface = FaceDetection.GetFaceRects(grayImgs,faceRects,faceWeights);
	
		for (int i = 0 ; i<nface; ++i)
		{	
			vector<int> facer ;
			facer.push_back( faceRects[i].x );
			facer.push_back( faceRects[i].y );
			facer.push_back( faceRects[i].width );
			facer.push_back( faceRects[i].height );
			while (getline( rfile, line))
			{
				istringstream recode (line) ;
				while (recode>>temp)
				{
					if ( temp == "<rotate>")
					{
						while (recode>>k)
						{
							lines.push_back(k);
						}
					}
					else if( temp == "<move>" )
					{
						while (recode>>rm)
						{
							rmove.push_back(rm);
						}
					}

				}
			}

			for( int k = 0; k<rmove.size(); k++)
			{
				CvRect faceRectRegion = imageMove( facer, rmove[k]);
				int flag = checkFcaeRect( grayImgs,faceRectRegion);
				if (flag)
				{
					faceRegion = grayImgs( faceRectRegion ).clone();
					string storeName = "C:\\Users\\DZP\\Desktop\\new\\" + name +"_s_"+to_string(rmove[k])+ ".jpg";
					cv::imwrite( storeName, faceRegion);
				}
				continue;
			}

			for( int k = 0; k<lines.size();k++)
			{
				int rotateValue = lines[k];
				imageRotate( grayImgs, rotateValue, facer, rotateface);
				faceRegion = rotateface( faceRects[i] ).clone();
				string storeName = "C:\\Users\\DZP\\Desktop\\new\\" + name +"_r_"+to_string(rotateValue)+ ".jpg";
				cv::imwrite( storeName, faceRegion);
			}

		}	
		rfile.close();
	}
	cout<<"END"<<endl;
	return 0;
}