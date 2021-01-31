#include "func_util.h"
#include <iostream>
#include "opencv2/opencv.hpp"
cv::Mat birdeye(cv::Mat img_front, cv::Mat img_right, cv::Mat img_left)
///img_front/right/left를 bird eye view로 변환해 img_out에 합쳐서 반환
{
	cv::Point2f front_pt1(29, 355);
	cv::Point2f front_pt2(599, 349);
	cv::Point2f front_pt3(640, 371);
	cv::Point2f front_pt4(640, 480);
	cv::Point2f front_pt5(0, 480);
	cv::Point2f front_pt6(0, 371);
	cv::Point2f right_pt1(0, 317);
	cv::Point2f right_pt2(425, 317);
	cv::Point2f right_pt3(552, 480);
	cv::Point2f right_pt4(0, 480);
	cv::Point2f left_pt1(229, 345);
	cv::Point2f left_pt2(640, 335);
	cv::Point2f left_pt3(640, 480);
	cv::Point2f left_pt4(119, 480);
	//cv::Point2f src_vertices_front_1[4];
	//cv::Point2f src_vertices_front_2[4];
	cv::Point2f src_vertices_front[6];
	cv::Point2f src_vertices_right[4];
	cv::Point2f src_vertices_left[4];
	/*src_vertices_front_1[0] = front_pt1;
	src_vertices_front_1[1] = front_pt2;
	src_vertices_front_1[2] = front_pt3;
	src_vertices_front_1[3] = front_pt6;
	src_vertices_front_2[0] = front_pt6;
	src_vertices_front_2[1] = front_pt3;
	src_vertices_front_2[2] = front_pt4;
	src_vertices_front_2[3] = front_pt5;*/
	src_vertices_front[0]=front_pt1;
	src_vertices_front[1]=front_pt2;
	src_vertices_front[4]=front_pt3;
	src_vertices_front[3]=front_pt4;
	src_vertices_front[2]=front_pt5;
	src_vertices_front[5]=front_pt6;
	src_vertices_right[0] = right_pt1;
	src_vertices_right[1] = right_pt2;
	src_vertices_right[2] = right_pt3;
	src_vertices_right[3] = right_pt4;
	src_vertices_left[0] = left_pt1;
	src_vertices_left[1] = left_pt2;
	src_vertices_left[2] = left_pt3;
	src_vertices_left[3] = left_pt4;
	//cv::Point2f dst_vertices_front_1[4];
	//cv::Point2f dst_vertices_front_2[4];
	cv::Point2f dst_vertices_front[6];
	cv::Point2f dst_vertices_right[4];
	cv::Point2f dst_vertices_left[4];
	/*dst_vertices_front_1[0] = cv::Point2f(0, 0);
	dst_vertices_front_1[1] = cv::Point2f(200, 0);
	dst_vertices_front_1[2] = cv::Point2f(200,25);
	dst_vertices_front_1[3] = cv::Point2f(0, 15);
	dst_vertices_front_2[0] = cv::Point2f(0, 15);
	dst_vertices_front_2[1] = cv::Point2f(200, 25);
	dst_vertices_front_2[2] = cv::Point2f(160, 92);
	dst_vertices_front_2[3] = cv::Point2f(40, 89);*/
	dst_vertices_front[0] = cv::Point2f(0, 0);
	dst_vertices_front[1] = cv::Point2f(200, 0);
	dst_vertices_front[4] = cv::Point2f(200, 25);
	dst_vertices_front[3] = cv::Point2f(160, 92);
	dst_vertices_front[2] = cv::Point2f(40, 89); //40, 89
	dst_vertices_front[5] = cv::Point2f(0, 15); //15
	dst_vertices_right[0] = cv::Point2f(200,126);
	dst_vertices_right[1] = cv::Point2f(200, 200);
	dst_vertices_right[2] = cv::Point2f(147, 200);
	dst_vertices_right[3] = cv::Point2f(147,158);
	dst_vertices_left[0] = cv::Point2f(0, 200);
	dst_vertices_left[1] = cv::Point2f(0, 125);
	dst_vertices_left[2] = cv::Point2f(52, 157);
	dst_vertices_left[3] = cv::Point2f(50, 200);
	
	//cv::Mat M_front_1 = cv::getPerspectiveTransform(src_vertices_front_1, dst_vertices_front_1);
	//cv::Mat M_front_2 = cv::getPerspectiveTransform(src_vertices_front_2, dst_vertices_front_2);
	cv::Mat M_front = cv::getPerspectiveTransform(src_vertices_front, dst_vertices_front);
	cv::Mat M_right = cv::getPerspectiveTransform(src_vertices_right, dst_vertices_right);
	cv::Mat M_left = cv::getPerspectiveTransform(src_vertices_left, dst_vertices_left);
	
	///Bird eye view로 변환 행렬 구하기
	cv::Mat img_out_front, img_out_right, img_out_left;
	int out_w = 200;
	int out_h = 200;
	//warpPerspective(img_front, img_out, M_front_1, cv::Size(out_w,out_h));
	//warpPerspective(img_front, img_out, M_front_2, cv::Size(out_w,out_h));
	warpPerspective(img_front, img_out_front, M_front, cv::Size(out_w,out_h));
	warpPerspective(img_right, img_out_right, M_right, cv::Size(out_w,out_h));
	warpPerspective(img_left, img_out_left, M_left, cv::Size(out_w,out_h));
	///정면, 좌측, 우측에서 받아온 이미지를 biredeve View로 변환해 img_out_front/right/left에 저장
	cv::Mat out_img = cv::Mat::zeros(200, 200, CV_8UC3);
	for(int i=0; i<200; i++)
	{
		for(int j=0; j<100; j++)
		{
			out_img.at<cv::Vec3b>(j,i) = img_out_front.at<cv::Vec3b>(j,i);
		}
	}
	for(int i=0; i<100; i++)
	{
		for(int j=100; j<200; j++)
		{
			out_img.at<cv::Vec3b>(j,i) = img_out_left.at<cv::Vec3b>(j,i);
		}
	}
	for(int i=100; i<200; i++)
	{
		for(int j=100; j<200; j++)
		{
			out_img.at<cv::Vec3b>(j,i) = img_out_right.at<cv::Vec3b>(j,i);
		}
	}
	///img_out_front/right/left를 out_img로 합침
	return out_img;
	/*int out_w = 200;
	int out_h = 200;
	cv::Point2f pt1(1370, 480);
	cv::Point2f pt2(0, 480);
	cv::Point2f pt3(487, 145);
	cv::Point2f pt4(883, 145);
	cv::Point2f src_vertices[4];
	src_vertices[0] = pt1;
	src_vertices[1] = pt2;
	src_vertices[2] = pt3;
	src_vertices[3] = pt4;
	cv::Point2f dst_vertices[4];
	dst_vertices[0] = cv::Point2f(out_w, out_h);
	dst_vertices[1] = cv::Point2f(0,out_h);
	dst_vertices[2] = cv::Point2f(0,0);
	dst_vertices[3] = cv::Point2f(out_w,0);
	cv::Mat M = cv::getPerspectiveTransform(src_vertices, dst_vertices);
	cv::Mat img_out;
	warpPerspective(temp_img, img_out, M, cv::Size(out_w,out_h));
	return img_out;*/
}
cv::Mat thresh_frame_in_HSV(cv::Mat src)
///src 이미지에서 노란색 부분만 추출
{
	cv::Mat hsv = cv::Mat::zeros(src.size(), CV_8UC3);
	///CV_8UC3 == unsigned 8bit * 3(R, G, B)
	cv::Mat yellow = cv::Mat::zeros(src.size(), CV_8UC1);
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV_FULL);
	cv::inRange(src, cv::Scalar(0, 50, 100), cv::Scalar(124, 255, 255), yellow);
	///src에서 HSV (0, 50, 100)~(124, 255, 255)사이의 값만 추려내서 yellow에 저장
	///관련 설명: https://learnopencv.com/color-spaces-in-opencv-cpp-python/
	return yellow;
}
cv::Mat thresh_frame_sobel(cv::Mat src)
///이미지에서 경계 추출
{
	int scale = 1;
	int delta = 0;
	cv::Mat gray = cv::Mat::zeros(src.size(), CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::Mat grad;
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;
	
	///cv::Sobel(src, mat, 결과 영상의 데이터 형식, x축 미분, y축 미분, sobel 커널 크기, 스케일, 기본 밝기, 가장자리 처리 방법)
	cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
	///grad_x, y: 가로 경계선, 세로 경계선
	cv::convertScaleAbs(grad_x, abs_grad_x);
	cv::convertScaleAbs(grad_y, abs_grad_y);
	///grad_x,y 결과에 절댓값을 적용하고 8buit unsigned int로 변환
	cv::addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);
	///cv::addWighted(img1, weight1, img2, weight2, bias, result)
	/// result=img1*weight1+img2*weight2+bias
	return grad;
}
cv::Mat get_binary_from_equalized_grayscale(cv::Mat src)
///이미지에서 흰색에 가까운 부분과 아닌 부분을 흑백으로 분리
{
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::Mat gray_equalize;
	cv::equalizeHist(gray, gray_equalize);
	///이미지의 밝기에 따라 히스트그램이 치우쳐져 있을 수 있는데, 이를 평준화하여 적절한 밝기로 조절해줌
	cv::Mat gray_out;
	cv::threshold(gray_equalize, gray_out, 250, 255, cv::THRESH_BINARY);
	///픽셀 값이 threshold(250)보다 크면 value(255), 작으면 0으로 할당
	return gray_out;
}
cv::Mat get_lane_mask(cv::Mat yellow, cv::Mat grad, cv::Mat gray)
///이미지의 경계 중 흰색이면 128, 노란색이면 255, 이외에는 0으로
{
	int img_size = 200;
	cv::Mat lane_mask = cv::Mat::zeros(img_size, img_size, CV_8UC1);
	for(int i=0; i< img_size; i++)
	{
		for(int j=0; j< img_size; j++)
		{
			if(grad.at<uchar>(i,j) == 255 && gray.at<uchar>(i,j) == 255)
			{
				lane_mask.at<uchar>(i,j) = 128;
			}
			else if(grad.at<uchar>(i,j) == 255 && yellow.at<uchar>(i,j) == 255)
			{
				lane_mask.at<uchar>(i,j) = 255;
			}
			else
			{
				lane_mask.at<uchar>(i,j) = 0;
			}
		}
	}	
	return lane_mask;
}
