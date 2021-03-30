#include "func_util.h"
#include "lane_fit_sliding_window.h"
#include "lane_class.h"
#include <ctime>
#include <ros/ros.h>
//#include <cv_bridge/cv_bridge.h>
//#include <image_transport/image_transport.h> // check
//#include <opencv2/imgproc/imgproc.hpp> // check
//#include <opencv2/highgui/highgui.hpp> // check
#include <opencv2/opencv.hpp>
//#include <core_msgs/CostMapwithGoalVector.h>
//#include <geometry_msgs/~~.h>
//#include <nav_msgs/~~.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>
#include <std_msgs/Int8.h>

int main(int argc, char **argv)
{
	ros::init(argc,argv, "cv_main");
	ros::NodeHandle nh;
	ros::Publisher costmap_with_goal_vector_pub = nh.advertise<nav_msgs::OccupancyGrid>("cost_map_with_goal_vector",100);
    ros::Rate loop_rate(10);
    
	int time = clock();
	//v::Mat img;

	/*std::cout<<cap.get(cv::CAP_PROP_FRAME_WIDTH)<<std::endl;
	std::cout<<cap.get(cv::CAP_PROP_FRAME_HEIGHT)<<std::endl;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);*/

	/*cv::VideoCapture cap(0);
	if(!cap.isOpened())
	{
		std::cerr << "Camera open failed!!"<<std::endl;
		return -1;
	}*/

	//cv::VideoCapture cap1("/home/snuzero1/Videos/front_lane_detection_2.avi");
	//cv::VideoCapture cap2("/home/snuzero1/Videos/right_lane_detection_2.avi");
	//cv::VideoCapture cap3("/home/snuzero1/Videos/left_lane_detection_2.avi");

	cv::VideoCapture cap1(0);
	cv::VideoCapture cap2(2);
	cv::VideoCapture cap3(4);

	if(!cap1.isOpened())
	{
		std::cerr << "Camera 1 open failed!!"<<std::endl;
		return -1;
	}
	if(!cap2.isOpened())
	{
		std::cerr << "Camera 2 open failed!!"<<std::endl;
		return -1;
	}
	if(!cap3.isOpened())
	{
		std::cerr << "Camera 3 open failed!!"<<std::endl;
		return -1;
	}
//camera open
	cv::Mat frame1, frame2, frame3;


	std::ifstream inFile("/home/snuzero/catkin_ws/src/zero/computer_vision/lane_detection_CostMap/camera_calibration.txt");
	cv::Mat mtx = cv::Mat::zeros(3,3,CV_32FC1);
	cv::Mat dist = cv::Mat::zeros(5, 1, CV_32FC1);
	if(inFile.is_open())
	{
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				inFile >> mtx.at<float>(i,j);
			}
		}
		for(int i=0; i<5; i++)
		{
			inFile >> dist.at<float>(i);
		}
	}
	else
	{
		std::cout<<"No calibration txt file"<<std::endl;
		return -1;
	}
//calibration setting (3D->2D)
	//////////////////////////////////////////////////////////////////
	
	while (nh.ok()) 
	{
		//cap >> img;
		time = clock();
		//img = cv::imread("/home/ayounglee/catkin_ws/src/zero/computer_vision/lane_detection_CostMap/test_images/test_6.bmp");
		
		cap1 >> frame1;
		cap2 >> frame2;
		cap3 >> frame3;

		cv::Mat img_cal_1, img_cal_2, img_cal_3;

		cv::undistort(frame1, img_cal_1, mtx, dist, mtx);
		cv::undistort(frame2, img_cal_2, mtx, dist, mtx);
		cv::undistort(frame3, img_cal_3, mtx, dist, mtx);

		//std::cout<<"1"<<std::endl;
		cv::Mat perspective_img;
		perspective_img = birdeye(img_cal_1, img_cal_2, img_cal_3);
//bird eye view img get
		cv::namedWindow("perspective_img");
		cv::imshow("perspective_img",perspective_img);
		
		
		//std::cout<<"1"<<std::endl;
		cv::Mat yellow;
		yellow = thresh_frame_in_HSV(perspective_img);
		//cv::namedWindow("yellow_img");
		//cv::imshow("yellow_img",yellow);

		//std::cout<<"1"<<std::endl;
		cv::Mat grad;
		grad = thresh_frame_sobel(perspective_img);
		//cv::namedWindow("grad_img");
		//cv::imshow("grad_img", grad);	

		//std::cout<<"1"<<std::endl;
		cv::Mat gray;
		gray = get_binary_from_equalized_grayscale(perspective_img);
	//	cv::namedWindow("gray_img");
	//	cv::imshow("gray_img", gray);	
//yellow, gray, grad(변화율) mask get
		//std::cout<<"1"<<std::endl;
		cv::Mat lane_mask;
		lane_mask = get_lane_mask(yellow, grad, gray);
	//	cv::namedWindow("lane_mask");
	//	cv::imshow("lane_mask", lane_mask);
//yellow, gray,grad mask 조합해서 전체적인 lane_mask get
		//std::cout<<"1"<<std::endl;
		cv::Mat fitting_mask;
		fitting_mask = get_fits_by_sliding_window(lane_mask, 10);
//cost map을 get
		cv::namedWindow("fitting_mask");
		cv::imshow("fitting_mask", fitting_mask);

		//std::cout<<"1"<<std::endl;
		cv::Point2i goal_point;
    	goal_point = get_goal_point(fitting_mask, lane_mask.cols);
    	std::cout<<goal_point<<std::endl;
//1열에 있는 검은 픽셀 점 중 무게 중심을 goal point로 get
		nav_msgs::OccupancyGrid cost_map;
		cost_map = nav_msgs::OccupancyGrid();

		//std::cout<<"1"<<std::endl;
		cost_map.info.width = fitting_mask.size().width;
		cost_map.info.height = fitting_mask.size().height;
		//std_msgs::Int8 cost;
		for (int i = 0; i < cost_map.info.width; i++){
			for( int j = 0; j < cost_map.info.height; j++){
				//cost.data = fitting_mask.at<uchar>(j,cost_map.info.width-i-1);
				int cost = (fitting_mask.at<uchar>(i,cost_map.info.width-j-1))/4;
				cost_map.data.push_back(cost);
			}
		}

		//x.data = goal_point.y;
		//y.data = 199 - goal_point.x;
		//theta.data = 0;

		int x = static_cast<double>((goal_point.x)/2.0);
		int y = static_cast<double>((199 - goal_point.y)/2.0);
		int theta = 0;

		cost_map.data.push_back(x);
		cost_map.data.push_back(y);
    		cost_map.data.push_back(theta);
		//ros::Rate loop_rate(5);
		
		cost_map.header.stamp.sec = clock();
        	costmap_with_goal_vector_pub.publish(cost_map);
       		//ros::spinOnce();
   	    	loop_rate.sleep();
		   
		std::cout << (clock() - time)/(double)CLOCKS_PER_SEC << std::endl;
		
		cv::waitKey(1);
    }
	
	return 0;
}
