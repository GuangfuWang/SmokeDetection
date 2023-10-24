#include "util.h"
#include <sys/stat.h>
#include <iostream>
#include <valarray>
#include <opencv2/imgproc.hpp>


namespace smoke
{

thread_local std::chrono::high_resolution_clock::time_point Util::mTic;

bool Util::checkDirExist(const std::string &dir)
{
	///note this feature require c++ 17 and above.
	return std::filesystem::exists(dir);
}

bool Util::checkFileExist(const std::string &file)
{
	///note this feature require c++ 17 and above.
	return std::filesystem::exists(file);
}

void Util::tic()
{
	mTic = std::chrono::high_resolution_clock::now();
}

long Util::toc()
{
	auto dur = std::chrono::high_resolution_clock::now() - mTic;
	mTic = std::chrono::high_resolution_clock::now();
	long ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	return ms;
}

long Util::getFileSize(const std::string &file)
{
	if (!checkFileExist(file)) {
		std::cerr << "File: " << file << " not exists..." << std::endl;
		return -1;
	}
	struct stat statBuf;
	stat(file.c_str(), &statBuf);
	return statBuf.st_size;
}

int Util::cvtStr2Int(const char *arr)
{
	return std::atoi(arr);
}
void Util::softmax(std::vector<float> &in)
{
	float total = 0.0f;
	for (auto &e : in) {
		auto curr = std::exp(e);
		e = curr;
		total += curr;
	}
	for (auto &e : in) {
		e /= total;
	}
}
std::vector<std::string> Util::parseNames(const std::string &names)
{
	std::vector<std::string> ret;
	char sep_ = ' ';
	const std::string &temp_ = names;
	if (temp_.find_first_of(sep_) == std::string::npos)sep_ = ',';
	size_t curr = 0;
	size_t lst = 0;
	while (true) {
		curr = temp_.find(sep_, curr);
		ret.emplace_back(temp_.substr(lst, curr - lst));
		if (curr == std::string::npos)break;
		lst = curr;
	}
	return ret;
}
void Util::plotBox(cv::Mat &img, int x0, int y0, int x1, int y1,
				   std::vector<unsigned char>color,int thickness)
{
	cv::line(img,cv::Point(x0,y0),
			 cv::Point(x1,y0),cv::Scalar(color[0],color[1],color[2]),
			 thickness);
	cv::line(img,cv::Point(x0,y0),
			 cv::Point(x0,y1),cv::Scalar(color[0],color[1],color[2]),
			 thickness);
	cv::line(img,cv::Point(x0,y1),
			 cv::Point(x1,y1),cv::Scalar(color[0],color[1],color[2]),
			 thickness);
	cv::line(img,cv::Point(x1,y0),
			 cv::Point(x1,y1),cv::Scalar(color[0],color[1],color[2]),
			 thickness);
}
}
