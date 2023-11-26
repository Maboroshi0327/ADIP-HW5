#include "HW_1.h"

#include <cmath>

#include <iostream>
#include <chrono>

#include <opencv2\opencv.hpp>

#include "../ImageProcessing/ImageProcessing.hpp"
#include "../FourierTransform/FourierTransform.hpp"

void opencvDrawFFT(cv::Mat& input, cv::Mat& output);
double opencvMSE(cv::Mat& mat1, dip::Image& img2);
double opencvPSNR(cv::Mat& mat1, dip::Image& img2);

void HW_1_1()
{
	dip::Image baboon_256, lena_256, img;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Complex2D fftResult;
	fftResult = vector2Complex2D(baboon_256.img);
	fftResult = dft::fft2D(fftResult, true, false);

	Byte2D result = dft::drawFFT(fftResult);
	img.setImg(result);
	img.saveRAW("baboon_fft.raw", true);

	fftResult = vector2Complex2D(lena_256.img);
	fftResult = dft::fft2D(fftResult, true, false);

	result = dft::drawFFT(fftResult);
	img.setImg(result);
	img.saveRAW("lena_fft.raw", true);
}

void HW_1_2()
{
	dip::Image baboon_256, lena_256, BaboonIFFT, LenaIFFT;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Complex2D baboonFFT, lenaFFT;
	baboonFFT = vector2Complex2D(baboon_256.img);
	lenaFFT = vector2Complex2D(lena_256.img);

	// FFT
	baboonFFT = dft::fft2D(baboonFFT, true, false);
	lenaFFT = dft::fft2D(lenaFFT, true, false);

	// IFFT
	baboonFFT = dft::fft2D(baboonFFT, true, true);
	lenaFFT = dft::fft2D(lenaFFT, true, true);

	Byte2D result;
	result = complex2dReal2vector<uint8_t>(baboonFFT);
	BaboonIFFT.setImg(result);
	BaboonIFFT.saveRAW("baboon_ifft.raw", true);

	result = complex2dReal2vector<uint8_t>(lenaFFT);
	LenaIFFT.setImg(result);
	LenaIFFT.saveRAW("lena_ifft.raw", true);

	// MSE PSNR
	std::cout << "baboon_256:" << std::endl;
	std::cout << "MSE: " << dip::mse(baboon_256, BaboonIFFT) << std::endl;
	std::cout << "PSNR: " << dip::psnr(baboon_256, BaboonIFFT) << std::endl;

	std::cout << "lena_256:" << std::endl;
	std::cout << "MSE: " << dip::mse(lena_256, LenaIFFT) << std::endl;
	std::cout << "PSNR: " << dip::psnr(lena_256, LenaIFFT) << std::endl;
}

void HW_1_3()
{
	dip::Image baboon_256, lena_256;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Complex2D baboonFFT, lenaFFT;
	baboonFFT = vector2Complex2D(baboon_256.img);
	lenaFFT = vector2Complex2D(lena_256.img);

	// FFT
	auto start = std::chrono::high_resolution_clock::now();

	baboonFFT = dft::fft2D(baboonFFT, true, false);
	lenaFFT = dft::fft2D(lenaFFT, true, false);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1, 1000>> duration = end - start;
	std::cout << "My DFT: " << duration.count() << "ms" << std::endl;

	// OpenCV
	cv::Mat baboon_256_Mat = cv::Mat::zeros(cv::Size(256, 256), CV_64FC1);
	cv::Mat lena_256_Mat = cv::Mat::zeros(cv::Size(256, 256), CV_64FC1);
	int shiftR = 1;
	for (int r = 0; r < 256; r++)
	{
		int shiftC = 1;
		for (int c = 0; c < 256; c++)
		{
			baboon_256_Mat.at<double>(r, c) = baboon_256.img[r][c] * shiftR * shiftC;
			lena_256_Mat.at<double>(r, c) = lena_256.img[r][c] * shiftR * shiftC;
			shiftC *= -1;
		}
		shiftR *= -1;
	}

	cv::Mat baboon_planes[] = { cv::Mat_<double>(baboon_256_Mat), cv::Mat::zeros(baboon_256_Mat.size(), CV_64F) };
	cv::Mat lena_planes[] = { cv::Mat_<double>(lena_256_Mat), cv::Mat::zeros(lena_256_Mat.size(), CV_64F) };
	cv::Mat baboon_complex, lena_complex;

	cv::merge(baboon_planes, 2, baboon_complex);
	cv::merge(lena_planes, 2, lena_complex);

	// OpenCV FFT
	start = std::chrono::high_resolution_clock::now();

	cv::dft(baboon_complex, baboon_complex);
	cv::dft(lena_complex, lena_complex);

	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "OpenCV DFT: " << duration.count() << "ms" << std::endl;

	cv::split(baboon_complex, baboon_planes);
	cv::split(lena_complex, lena_planes);

	// Magnitude
	cv::magnitude(baboon_planes[0], baboon_planes[1], baboon_planes[0]);
	cv::magnitude(lena_planes[0], lena_planes[1], lena_planes[0]);

	cv::Mat baboon_fft_opencv, lena_fft_opencv;
	opencvDrawFFT(baboon_planes[0], baboon_fft_opencv);
	opencvDrawFFT(lena_planes[0], lena_fft_opencv);

	cv::imwrite("baboon_fft_opencv.bmp", baboon_fft_opencv);
	cv::imwrite("lena_fft_opencv.bmp", lena_fft_opencv);

	cv::imshow("baboon_fft_opencv", baboon_fft_opencv);
	cv::waitKey();
	cv::imshow("lena_fft_opencv", lena_fft_opencv);
	cv::waitKey();
}

void HW_1_4()
{
	dip::Image baboon_256, lena_256;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Complex2D baboonFFT, lenaFFT;
	baboonFFT = vector2Complex2D(baboon_256.img);
	lenaFFT = vector2Complex2D(lena_256.img);

	// FFT
	baboonFFT = dft::fft2D(baboonFFT, true, false);
	lenaFFT = dft::fft2D(lenaFFT, true, false);

	// IFFT
	auto start = std::chrono::high_resolution_clock::now();

	baboonFFT = dft::fft2D(baboonFFT, true, true);
	lenaFFT = dft::fft2D(lenaFFT, true, true);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1, 1000>> duration = end - start;
	std::cout << "My IDFT: " << duration.count() << "ms" << std::endl;

	// OpenCV
	cv::Mat baboon_256_Mat = cv::Mat::zeros(cv::Size(256, 256), CV_64FC1);
	cv::Mat lena_256_Mat = cv::Mat::zeros(cv::Size(256, 256), CV_64FC1);
	for (int r = 0; r < 256; r++)
		for (int c = 0; c < 256; c++)
		{
			baboon_256_Mat.at<double>(r, c) = baboon_256.img[r][c];
			lena_256_Mat.at<double>(r, c) = lena_256.img[r][c];
		}

	cv::Mat baboon_planes[] = { cv::Mat_<double>(baboon_256_Mat), cv::Mat::zeros(baboon_256_Mat.size(), CV_64F) };
	cv::Mat lena_planes[] = { cv::Mat_<double>(lena_256_Mat), cv::Mat::zeros(lena_256_Mat.size(), CV_64F) };
	cv::Mat baboon_complex, lena_complex;

	cv::merge(baboon_planes, 2, baboon_complex);
	cv::merge(lena_planes, 2, lena_complex);

	// OpenCV FFT
	cv::dft(baboon_complex, baboon_complex);
	cv::dft(lena_complex, lena_complex);

	// OpenCV IFFT
	start = std::chrono::high_resolution_clock::now();

	cv::idft(baboon_complex, baboon_complex, cv::DFT_REAL_OUTPUT);
	cv::idft(lena_complex, lena_complex, cv::DFT_REAL_OUTPUT);

	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "OpenCV IDFT: " << duration.count() << "ms" << std::endl;

	cv::normalize(baboon_complex, baboon_complex, 0, 255, cv::NORM_MINMAX);
	cv::normalize(lena_complex, lena_complex, 0, 255, cv::NORM_MINMAX);

	cv::Mat baboon_ifft_opencv = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
	cv::Mat lena_ifft_opencv = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
	for (int r = 0; r < 256; r++)
		for (int c = 0; c < 256; c++)
		{
			baboon_ifft_opencv.at<uchar>(r, c) = (uchar)std::round(baboon_complex.at<double>(r, c));
			lena_ifft_opencv.at<uchar>(r, c) = (uchar)std::round(lena_complex.at<double>(r, c));
		}

	// MSE PSNR
	std::cout << "baboon_256:" << std::endl;
	std::cout << "MSE: " << opencvMSE(baboon_ifft_opencv, baboon_256) << std::endl;
	std::cout << "PSNR: " << opencvPSNR(baboon_ifft_opencv, baboon_256) << std::endl;

	std::cout << "lena_256:" << std::endl;
	std::cout << "MSE: " << opencvMSE(lena_ifft_opencv, lena_256) << std::endl;
	std::cout << "PSNR: " << opencvPSNR(lena_ifft_opencv, lena_256) << std::endl;

	cv::imwrite("baboon_ifft_opencv.bmp", baboon_ifft_opencv);
	cv::imwrite("lena_ifft_opencv.bmp", lena_ifft_opencv);

	cv::imshow("baboon_ifft_opencv", baboon_ifft_opencv);
	cv::waitKey();
	cv::imshow("lena_ifft_opencv", lena_ifft_opencv);
	cv::waitKey();
}

void HW_1_5()
{
	dip::Image baboon_256, lena_256, baboon_fct, lena_fct;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Double2D baboon_double = vector2Double2D(baboon_256.img);
	Double2D lena_double = vector2Double2D(lena_256.img);

	// FCT
	baboon_double = dct::fct2D(baboon_double, false);
	lena_double = dct::fct2D(lena_double, false);

	Byte2D baboon_byte = dct::drawFCT(baboon_double);
	Byte2D lena_byte = dct::drawFCT(lena_double);

	baboon_fct.setImg(baboon_byte);
	baboon_fct.saveRAW("baboon_fct.raw", true);
	lena_fct.setImg(lena_byte);
	lena_fct.saveRAW("lena_fct.raw", true);
}

void HW_1_6()
{
	dip::Image baboon_256, lena_256, baboon_ifct, lena_ifct;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));
	lena_256.openRAW("lena_256.raw", dip::Size(256, 256));

	Double2D baboon_double = vector2Double2D(baboon_256.img);
	Double2D lena_double = vector2Double2D(lena_256.img);

	// FCT
	baboon_double = dct::fct2D(baboon_double, false);
	lena_double = dct::fct2D(lena_double, false);

	// IFCT
	baboon_double = dct::fct2D(baboon_double, true);
	lena_double = dct::fct2D(lena_double, true);

	Byte2D result;
	result = double2d2vector<uint8_t>(baboon_double);
	baboon_ifct.setImg(result);

	result = double2d2vector<uint8_t>(lena_double);
	lena_ifct.setImg(result);

	baboon_ifct.saveRAW("baboon_ifct.raw", true);
	lena_ifct.saveRAW("lena_ifct.raw", true);

	// MSE PSNR
	std::cout << "baboon_256:" << std::endl;
	std::cout << "MSE: " << dip::mse(baboon_256, baboon_ifct) << std::endl;
	std::cout << "PSNR: " << dip::psnr(baboon_256, baboon_ifct) << std::endl;

	std::cout << "lena_256:" << std::endl;
	std::cout << "MSE: " << dip::mse(lena_256, lena_ifct) << std::endl;
	std::cout << "PSNR: " << dip::psnr(lena_256, lena_ifct) << std::endl;
}

void opencvDrawFFT(cv::Mat& input, cv::Mat& output)
{
	int rows = input.rows;
	int cols = input.cols;

	cv::Mat powMagnitude = cv::Mat::zeros(input.size(), CV_64F);
	double max = std::pow(std::abs(input.at<double>(0, 0)), 0.2);
	double min = max;
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
		{
			double pow = std::pow(std::abs(input.at<double>(r, c)), 0.2);
			powMagnitude.at<double>(r, c) = pow;
			if (max < pow)
				max = pow;
			if (min > pow)
				min = pow;
		}


	cv::Mat result = cv::Mat::zeros(input.size(), CV_8U);
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
		{
			result.at<uchar>(r, c) = (uchar)std::round((powMagnitude.at<double>(r, c) - min) / (max - min) * 255);
		}

	output = result.clone();
}

double opencvMSE(cv::Mat& mat1, dip::Image& img2)
{
	int sum = 0;
	for (int r = 0; r < mat1.rows; r++)
		for (int c = 0; c < mat1.cols; c++)
			sum = sum + (mat1.at<uchar>(r, c) - img2.img[r][c]) * (mat1.at<uchar>(r, c) - img2.img[r][c]);

	double value = (double)sum / (mat1.rows * mat1.cols);

	return value;
}

double opencvPSNR(cv::Mat& mat1, dip::Image& img2)
{
	double mse_v = opencvMSE(mat1, img2);
	double param = 255 * 255 / mse_v;
	double value = 10 * std::log10(param);

	return value;
}
