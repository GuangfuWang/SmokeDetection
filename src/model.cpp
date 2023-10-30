#include "model.h"
#include "config.h"
#include "trt_deploy.h"
#include "trt_deployresult.h"

namespace smoke{

class InferModel {
public:
	explicit InferModel(SharedRef<Config>& config) {
		m_config = config;
		mDeploy = createSharedRef<TrtDeploy>(config);
		mResult = createSharedRef<TrtResults>(config);
	}

public:
	SharedRef<TrtDeploy> mDeploy;
	SharedRef<TrtResults> mResult;
	std::vector<cv::Mat> mSampled;
//	std::vector<cv::Mat> mTotal;
	SharedRef<Config> m_config;
	cv::Mat m_roi_img;
	unsigned int COUNT_LOOP = 0;
	unsigned int COUNT = 0;
	unsigned int COUNT_TOTAL = 0;
};

static void *GenModel(SharedRef<Config>& config) {
	auto *model = new InferModel(config);
	model->COUNT = config->SAMPLE_INTERVAL*config->TRIGGER_LEN;
	return reinterpret_cast<void *>(model);
}

static cv::Mat genROI(const cv::Size s, const std::vector<int> &points, cv_Point *coords)
{
	if (points.empty()){
		return {s, CV_8UC3, cv::Scalar::all(255)};
	}
	cv::Mat roi_img = cv::Mat::zeros(s, CV_8UC3);
	cv::Mat removed_roi;

	std::vector<std::vector<cv::Point>> contour;

	int sums = 0;
	for (auto &each : points) {
		std::vector<cv::Point> pts;
		for (int j = sums; j < each + sums; ++j) {
			pts.push_back(cv::Point(coords[j].x, coords[j].y));
		}
		sums += each;
		contour.push_back(pts);
	}
	sums = 0;
	for (auto &i : points) {
		cv::drawContours(roi_img, contour,
						 sums, cv::Scalar::all(255), -1);
		sums++;
	}
	return roi_img;
}

void plotLines(cv::Mat &im, const std::vector<int> &points,
			   const cv_Point * coords,const int& thickness)
{
	int sums = 0;
	for(auto& each:points){
		for (int j = sums; j < each+sums; ++j) {
			int k = j+1;
			if(k==each+sums)k=sums;
			cv::line(im, cv::Point(coords[j].x, coords[j].y),
					 cv::Point(coords[k].x, coords[k].y), cv::Scalar(255, 0, 0),
					 thickness);
		}
		sums+=each;
	}
}

cvModel *Allocate_Algorithm(cv::Mat &input_frame, int algID, int gpuID) {
	cv::cuda::setDevice(gpuID);
	cudaSetDevice(gpuID);
	std::string file;
	if(Util::checkFileExist("./smoke_detection.yaml"))
		file = "./smoke_detection.yaml";
	else if(Util::checkFileExist("../config/smoke_detection.yaml")){
		file = "../config/smoke_detection.yaml";
	}else{
		std::cout<<"Cannot find YAML file!"<<std::endl;
	}
	auto config = createSharedRef<Config>(1, nullptr,file);
	auto *ptr = new cvModel();
	ptr->FrameNum = 0;
	ptr->Frameinterval = 0;
	ptr->countNum = 0;
	ptr->width = input_frame.cols;
	ptr->height = input_frame.rows;
	ptr->iModel = GenModel(config);
	return ptr;
}

void SetPara_Algorithm(cvModel *pModel, int algID) {
	//todo: implement this
}

void UpdateParams_Algorithm(cvModel *pModel) {
	auto model = reinterpret_cast<InferModel *>(pModel->iModel);
	auto roi = pModel->p;
	model->m_roi_img = genROI(cv::Size(pModel->width,pModel->height),
							  pModel->pointNum, roi);
}

void Process_Algorithm(cvModel *pModel, cv::Mat &input_frame) {
	auto model = reinterpret_cast<InferModel *>(pModel->iModel);
	auto config = model->m_config;
	auto roi = pModel->p;
	if (model->m_roi_img.empty()) {
		model->m_roi_img = genROI(input_frame.size(), pModel->pointNum, roi);
	}

	cv::Mat removed_roi;
	input_frame.copyTo(removed_roi,model->m_roi_img);
	std::vector<cv::Mat> temp;
	temp.push_back(removed_roi);
	model->mDeploy->Infer(temp, model->mResult);
	std::vector<cv::Mat> ret;
	ret.push_back(input_frame);
	model->mDeploy->Postprocessing(model->mResult,temp,ret,pModel->alarm);
	input_frame = ret[0].clone();

	plotLines(input_frame,pModel->pointNum,
			  roi,(int)config->BOX_LINE_WIDTH);
}

void Destroy_Algorithm(cvModel *pModel) {
	if (pModel->iModel){
		auto model = reinterpret_cast<InferModel *>(pModel->iModel);
		delete model;
		model = nullptr;
	}
	if (pModel) {
		delete pModel;
		pModel = nullptr;
	}
}
}