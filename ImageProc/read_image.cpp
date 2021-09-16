
#include <stdio.h> 
#include <iostream> 
#include<opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

void show_image(string img_path, string window_name) {
	Mat image = imread(img_path);
	namedWindow(window_name);
	imshow(window_name, image);
	waitKey(3000);
}


Mat create_hist(Mat image) {
	while (true) {
		//https://docs.opencv.org/3.4/d8/dbc/tutorial_histogram_calculation.html
		vector<Mat> bgr_planes;
		split(image, bgr_planes);
		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		const float* histRange[] = { range };
		bool uniform = true, accumulate = false;
		Mat b_hist, g_hist, r_hist;
		calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);
		int hist_w = 512, hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);
		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}
		return histImage;
	}
}

Mat take_photo() {
	cv::VideoCapture camera(0);
	if (!camera.isOpened()) {
		std::cerr << "ERROR: Could not open camera" << std::endl;
	}

	namedWindow("Webcam", 512);
	cout << "Taking photo with webcam";
	Mat frame, frame_canny, histImage;
	while (true) {
		camera >> frame;
		histImage = create_hist(frame);
		Canny(frame, frame_canny, 25, 75);
		imshow("Canny Edge Detector", frame_canny);
		imshow("Video Stream", frame);
		imshow("Histogram of Video Capture", histImage);
		if (waitKey(1) >= 0) {
			break;
		}
	}
	if (frame.empty())
	{
		cerr << "Something is wrong with the webcam, could not get frame." << endl;
	}
	return frame;
}

void save_image(Mat image, string img_path, string image_save_format) {
	string image_path_wo_extention = img_path.substr(0,img_path.find("."));
	imwrite(image_path_wo_extention + image_save_format, image);
	cout << "Image saved to: " << image_path_wo_extention << " as: " << image_save_format << endl;
}

void print_image_attributes(string img_path, string window_name, string file_type) {
	Mat image = imread(img_path);
	cout << "file name: " << window_name << endl;
	cout << "file size: " << image.size << endl;
	cout << "image type. " << file_type << endl;
	if (image.channels() == 3) {
		cout << "Color format is BGR" << endl;
	}
}


int main()
{
	const string Resource_path = "Resource/";
	string image_save_format = ".png";
	string img_path = Resource_path + "webcam_test.jpg";
	string window_name = "test";
	string delimiter = ".";
	string file_type = img_path.substr(img_path.find(delimiter) + 1, img_path.length());

	show_image(img_path, window_name);
	print_image_attributes(img_path, window_name, file_type);
	Mat image = take_photo();
	save_image(image, img_path, image_save_format);
	return 0;
}

// test
