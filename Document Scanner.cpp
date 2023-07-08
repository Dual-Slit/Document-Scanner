#include<iostream>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#define mat Mat
#define str string

using namespace std;
using namespace cv;

/****************** Set Image Path ****************/

 str path = "Resources/paper.jpg"; //orginal image path

 float W = 210, H = 298; ///set here orginal document width and height here by defalut is A4 paper 
 float w = W * 2;
 float h = H * 2;

/****************** No Change Below ***************/



 mat imgOriginal, imgGray, imgCrop, matrix, imgBlur, imgWarp, imgDil, imgCanny, imgThre;
 vector<Point> initialPoints, docsPoints;

 

 mat preprocessing(mat img)
 {
	 cvtColor(img, imgGray, COLOR_BGR2GRAY);
	 GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	 Canny(img, imgCanny, 25, 75);
	 mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2)); 
	 dilate(imgCanny, imgDil, kernel);
	 return imgDil;
 }

 vector<Point> getContours(mat imgDil)
 {
	 vector<vector<Point>>  contours;
	 vector<Vec4i> hierarchy;

	 findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	 vector<vector<Point>> conPoly(contours.size());
	 vector<Rect> boundRect(contours.size());

	 vector<Point> biggest;
	 double maxArea = 0;
	 for (int i = 0; i < contours.size(); i++)
	 {
		 double area = contourArea(contours[i]);

		 if (area > 1000)
		 {
			 float peri = arcLength(contours[i], true);
			 approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			 if (area > maxArea && conPoly[i].size() == 4)
			 {
				 biggest = { conPoly[i][0],conPoly[i][1], conPoly[i][2], conPoly[i][3] };
				 maxArea = area;
			 }
		 }
	 }
	 return biggest;
	 ;
 }

 vector<Point> reorder(vector<Point>Points)
 {
	 vector<Point> newPoints;

	 vector<int> sumPoints, subPoints;

	 for (int i = 0; i < 4; i++)
	 {
		 sumPoints.push_back(Points[i].x + Points[i].y);
		 subPoints.push_back(Points[i].x - Points[i].y);
	 }

	 newPoints.push_back(Points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	 newPoints.push_back(Points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	 newPoints.push_back(Points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	 newPoints.push_back(Points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
    
	 return newPoints;
 }


 mat getWrap(mat img, vector<Point> Points, float w, float h)
 {
	 Point2f src[4] = { Points[0],Points[1],Points[2],Points[3]};
	 Point2f dst[4] = { {0.0f,0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	 matrix = getPerspectiveTransform(src, dst);
	 warpPerspective(img, imgWarp, matrix, Point(w, h));
	 return imgWarp;
 }

int main()
{
	imgOriginal = imread(path);

	resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	//Preprocessing 
	imgThre = preprocessing(imgOriginal);

	//get contor -- Biggest
	initialPoints = getContours(imgThre);

	
	docsPoints = reorder(initialPoints);


	//Warp
	imgWarp = getWrap(imgOriginal, docsPoints, w, h);

	//croping extra pixels
	int pixelreduce = 10;
	Rect roi(pixelreduce, pixelreduce, w - (2 * pixelreduce), h - (2 * pixelreduce)); //hr trf se 5 5 pixel cut kr dega
	imgCrop = imgWarp(roi);



	imshow("Initial Document", imgOriginal);
	imshow("Scanned Document", imgWarp);
	imshow("Crroped Document", imgCrop);
	waitKey(0);
	return 0;
}