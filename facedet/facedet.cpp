#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function Headers
void detectAndDisplay(Mat frame);

// Global variables
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "/home/rahim/workspace/FaceDet/Debug/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;

// Function main
int main(void)
{
    // Load the cascade
    if (!face_cascade.load(face_cascade_name)){
        printf("--(!)Error loading\n");
        return (-1);
    }
    VideoCapture cap;
    cap.open("/media/rahim/01D051E78FFCF7C0/Lab/Dataset/EyeHead/HPEG/HPEG/session_a/videos/%05d.jpg");
    // Read the image file
    for(int i=0;i<1835;i++)
    {
    	if(i%1835==0) cap = VideoCapture("/media/rahim/01D051E78FFCF7C0/Lab/Dataset/EyeHead/HPEG/HPEG/session_a/videos/frames/%05d.jpg");
    	cout << " image number : " << i << ".\n";
    	Mat frame;
    	cap >> frame;
    	// Apply the classifier to the frame
    	if (!frame.empty()){
    	//	detectAndDisplay(frame);
        	std::vector<Rect> faces;
        	Mat frame_gray;
        	Mat crop;
        	Mat res;
        	Mat gray;
        	string text;
        	stringstream sstm;

        	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        	equalizeHist(frame_gray, frame_gray);

        	// Detect faces
        	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        	// Set Region of Interest
        	cv::Rect roi_b;
        	cv::Rect roi_c;

        	size_t ic = 0; // ic is index of current element
        	int ac = 0; // ac is area of current element

        	size_t ib = 0; // ib is index of biggest element
        	int ab = 0; // ab is area of biggest element

        	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

        	{
        		roi_c.x = faces[ic].x;
        		roi_c.y = faces[ic].y;
        		roi_c.width = (faces[ic].width);
        		roi_c.height = (faces[ic].height);

        		ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

        		roi_b.x = faces[ib].x;
        		roi_b.y = faces[ib].y;
        		roi_b.width = (faces[ib].width);
        		roi_b.height = (faces[ib].height);

        		ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        		if (ac > ab)
        		{
        			ib = ic;
        			roi_b.x = faces[ib].x;
        			roi_b.y = faces[ib].y;
        			roi_b.width = (faces[ib].width);
        			roi_b.height = (faces[ib].height);
        		}

        		crop = frame(roi_b);
        		resize(crop, res, Size(256, 256), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        		cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

    			// Form a filename
    		//	filename = "";
    		//	stringstream ssfn;
    		//	ssfn << filenumber << i <<  ".jpg";
    		//	filename = ssfn.str();
    		//	filenumber++;
        	//	imwrite(filename, gray);

    			char *imageName = "image-";
    			char *jpgN = ".jpg";
    			stringstream s;
    			s << imageName << i << jpgN;
    			cv::imwrite (s.str(), gray);



            Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
            Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
            rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    		}

    		// Show image
    		sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
    		text = sstm.str();

    		putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
    		//imshow("original", frame);

    		if (!crop.empty())
    		{
    			imshow("detected", crop);
    		}
    		else
    			destroyWindow("detected");
    	}
    	else{
    		printf(" --(!) No captured frame -- Break!");
    		//break;
    	}


}
}


