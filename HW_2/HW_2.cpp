#include "HW_2.h"

#include <string>
#include <sstream>

#include "../ImageProcessing/ImageProcessing.hpp"
#include "../FourierTransform/FourierTransform.hpp"

void HW_2_1()
{
	dip::Image baboon_256, img;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));

	// FFT
	Complex2D baboon_complex, baboon_fft_shift, baboon_fft_nshift;
	baboon_complex = vector2Complex2D(baboon_256.img);
	baboon_fft_shift = dft::fft2D(baboon_complex, true, false);
	baboon_fft_nshift = dft::fft2D(baboon_complex, false, false);

	Double2D GF;
	Complex2D filtered;
	Byte2D result;

	const int D0[4] = { 10, 20, 40, 100 };
	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;

		// GLPF
		// shift
		GF = dft::makeGaussianLPF(D0[i], 256, 256);
		filtered = dft::Filtering(baboon_fft_shift, GF);
		filtered = dft::fft2D(filtered, true, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName << "GLPF_" << D0[i] << "_shift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// nshift
		filtered = dft::Filtering(baboon_fft_nshift, GF);
		filtered = dft::fft2D(filtered, false, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "GLPF_" << D0[i] << "_nshift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// GHPF
		// shift
		GF = dft::makeGaussianHPF(D0[i], 256, 256);
		filtered = dft::Filtering(baboon_fft_shift, GF);
		filtered = dft::fft2D(filtered, true, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "GHPF_" << D0[i] << "_shift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// nshift
		filtered = dft::Filtering(baboon_fft_nshift, GF);
		filtered = dft::fft2D(filtered, false, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "GHPF_" << D0[i] << "_nshift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}

	result = Byte2D(256, Byte1D(256));
	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;
		fileName << "GLPF_spectrum_" << D0[i] << ".raw";

		GF = dft::makeGaussianLPF(D0[i], 256, 256);
		for (int r = 0; r < 256; r++)
			for (int c = 0; c < 256; c++)
				result[r][c] = (Byte)(GF[r][c] * 255.0);
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}

	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;
		fileName << "GHPF_spectrum_" << D0[i] << ".raw";

		GF = dft::makeGaussianHPF(D0[i], 256, 256);
		for (int r = 0; r < 256; r++)
			for (int c = 0; c < 256; c++)
				result[r][c] = (Byte)(GF[r][c] * 255.0);
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}
}

void HW_2_2()
{
	dip::Image baboon_256, img;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));

	// FFT
	Complex2D baboon_complex, baboon_fft_shift, baboon_fft_nshift;
	baboon_complex = vector2Complex2D(baboon_256.img);
	baboon_fft_shift = dft::fft2D(baboon_complex, true, false);
	baboon_fft_nshift = dft::fft2D(baboon_complex, false, false);

	Double2D BF;
	Complex2D filtered;
	Byte2D result;

	const int D0[4] = { 10, 20, 40, 100 };
	for (int n = 1; n <= 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::stringstream fileName;

			// BLPF
			// shift
			BF = dft::makeButterworthLPF(D0[i], n, 256, 256);
			filtered = dft::Filtering(baboon_fft_shift, BF);
			filtered = dft::fft2D(filtered, true, true);
			result = minMaxNormalization<Byte>(filtered, 0, 255);
			fileName << "BLPF_" << D0[i] << "_" << n << "_shift.raw";
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);

			// nshift
			filtered = dft::Filtering(baboon_fft_nshift, BF);
			filtered = dft::fft2D(filtered, false, true);
			result = minMaxNormalization<Byte>(filtered, 0, 255);
			fileName.str(std::string());
			fileName << "BLPF_" << D0[i] << "_" << n << "_nshift.raw";
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);

			// BHPF
			// shift
			BF = dft::makeButterworthHPF(D0[i], n, 256, 256);
			filtered = dft::Filtering(baboon_fft_shift, BF);
			filtered = dft::fft2D(filtered, true, true);
			result = minMaxNormalization<Byte>(filtered, 0, 255);
			fileName.str(std::string());
			fileName << "BHPF_" << D0[i] << "_" << n << "_shift.raw";
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);

			// nshift
			filtered = dft::Filtering(baboon_fft_nshift, BF);
			filtered = dft::fft2D(filtered, false, true);
			result = minMaxNormalization<Byte>(filtered, 0, 255);
			fileName.str(std::string());
			fileName << "BHPF_" << D0[i] << "_" << n << "_nshift.raw";
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);
		}
	}

	result = Byte2D(256, Byte1D(256));
	for (int n = 1; n <= 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::stringstream fileName;
			fileName << "BLPF_spectrum_" << D0[i] << "_" << n << ".raw";

			BF = dft::makeButterworthLPF(D0[i], n, 256, 256);
			for (int r = 0; r < 256; r++)
				for (int c = 0; c < 256; c++)
					result[r][c] = (Byte)(BF[r][c] * 255.0);
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);
		}
	}

	for (int n = 1; n <= 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::stringstream fileName;
			fileName << "BHPF_spectrum_" << D0[i] << "_" << n << ".raw";

			BF = dft::makeButterworthHPF(D0[i], n, 256, 256);
			for (int r = 0; r < 256; r++)
				for (int c = 0; c < 256; c++)
					result[r][c] = (Byte)(BF[r][c] * 255.0);
			img.setImg(result);
			img.saveRAW(fileName.str().c_str(), true);
		}
	}

}

void HW_2_3()
{
	dip::Image baboon_256, img;
	baboon_256.openRAW("baboon_256.raw", dip::Size(256, 256));

	// FFT
	Complex2D baboon_complex, baboon_fft_shift, baboon_fft_nshift;
	baboon_complex = vector2Complex2D(baboon_256.img);
	baboon_fft_shift = dft::fft2D(baboon_complex, true, false);
	baboon_fft_nshift = dft::fft2D(baboon_complex, false, false);

	Double2D IF;
	Complex2D filtered;
	Byte2D result;

	const int D0[4] = { 10, 20, 40, 100 };
	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;

		// ILPF
		// shift
		IF = dft::makeIdealLPF(D0[i], 256, 256);
		filtered = dft::Filtering(baboon_fft_shift, IF);
		filtered = dft::fft2D(filtered, true, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName << "ILPF_" << D0[i] << "_shift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// nshift
		filtered = dft::Filtering(baboon_fft_nshift, IF);
		filtered = dft::fft2D(filtered, false, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "ILPF_" << D0[i] << "_nshift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// IHPF
		// shift
		IF = dft::makeIdealHPF(D0[i], 256, 256);
		filtered = dft::Filtering(baboon_fft_shift, IF);
		filtered = dft::fft2D(filtered, true, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "IHPF_" << D0[i] << "_shift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);

		// nshift
		filtered = dft::Filtering(baboon_fft_nshift, IF);
		filtered = dft::fft2D(filtered, false, true);
		result = minMaxNormalization<Byte>(filtered, 0, 255);
		fileName.str(std::string());
		fileName << "IHPF_" << D0[i] << "_nshift.raw";
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}

	result = Byte2D(256, Byte1D(256));
	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;
		fileName << "ILPF_spectrum_" << D0[i] << ".raw";

		IF = dft::makeIdealLPF(D0[i], 256, 256);
		for (int r = 0; r < 256; r++)
			for (int c = 0; c < 256; c++)
				result[r][c] = (Byte)(IF[r][c] * 255.0);
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}

	for (int i = 0; i < 4; i++)
	{
		std::stringstream fileName;
		fileName << "IHPF_spectrum_" << D0[i] << ".raw";

		IF = dft::makeIdealHPF(D0[i], 256, 256);
		for (int r = 0; r < 256; r++)
			for (int c = 0; c < 256; c++)
				result[r][c] = (Byte)(IF[r][c] * 255.0);
		img.setImg(result);
		img.saveRAW(fileName.str().c_str(), true);
	}
}
