#include "HW_3.h"

#include <cmath>

#include <thread>

#include "../ImageProcessing/ImageProcessing.hpp"
#include "../FourierTransform/FourierTransform.hpp"

void HW_3_1()
{
	std::thread t1(HW_3_1_frequency);
	std::thread t2(HW_3_1_spatial);
	t1.join();
	t2.join();
}

void HW_3_1_frequency()
{
	dip::Image building_474x632, img;
	building_474x632.openRAW("building_474x632.raw", dip::Size(632, 474));

	Complex2D buildingFFT = vector2Complex2D(building_474x632.img);

	// FFT
	Complex2D sobelFFT_0, sobelFFT_90;
	void (*makeSobelFilter)(int, int, int, Complex2D&) = &dft::makeSobelFilter;
	void(*fft2D)(const Complex2D&, Complex2D&, bool, bool) = &dft::fft2D;
	std::thread t1(makeSobelFilter, 0, 632, 474, std::ref(sobelFFT_0));
	std::thread t2(makeSobelFilter, 90, 632, 474, std::ref(sobelFFT_90));
	std::thread t3(fft2D, std::ref(buildingFFT), std::ref(buildingFFT), true, false);
	t1.join();
	t2.join();
	t3.join();

	// Filtering
	Complex2D building0_IFFT = dft::Filtering(buildingFFT, sobelFFT_0);
	Complex2D building90_IFFT = dft::Filtering(buildingFFT, sobelFFT_90);

	// IFFT
	std::thread t4(fft2D, std::ref(building0_IFFT), std::ref(building0_IFFT), true, true);
	std::thread t5(fft2D, std::ref(building90_IFFT), std::ref(building90_IFFT), true, true);
	t4.join();
	t5.join();

	Double2D building0_mag = complex2dReal2vector<double>(building0_IFFT);
	Double2D building90_mag = complex2dReal2vector<double>(building90_IFFT);
	Double2D buildingSobel(632, Double1D(474));
	for (int r = 0; r < 632; r++)
		for (int c = 0; c < 474; c++)
			buildingSobel[r][c] = std::round(
				std::sqrt(building0_mag[r][c] * building0_mag[r][c] + building0_mag[r][c] * building0_mag[r][c])
			);

	Byte2D magnitude = dft::drawFFT(sobelFFT_0);
	img.setImg(magnitude);
	img.saveRAW("sobelFFT_0.raw", true);
	magnitude = dft::drawFFT(sobelFFT_90);
	img.setImg(magnitude);
	img.saveRAW("sobelFFT_90.raw", true);
	magnitude = dft::drawFFT(buildingFFT);
	img.setImg(magnitude);
	img.saveRAW("buildingFFT.raw", true);

	magnitude = minMaxNormalization<Byte>(buildingSobel, 0, 255);
	img.setImg(magnitude);
	img.saveRAW("buildingSobel_frequency.raw", true);
}

void HW_3_1_spatial()
{
	dip::Image building_474x632, img;
	building_474x632.openRAW("building_474x632.raw", dip::Size(632, 474));

	dip::sobelFiltering(building_474x632, img);
	img.saveRAW("buildingSobel_spatial.raw", true);
}
