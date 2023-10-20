#include "model.h"
#include "config.h"
#include "trt_deploy.h"
#include "trt_deployresult.h"

using namespace gf;

unsigned int COUNT_TOTAL = 0;
unsigned int COUNT_LOOP = 0;
unsigned int COUNT = 0;
namespace smoke{
class InferModel {
public:
	InferModel() {
		mDeploy = createSharedRef<TrtDeploy>();
		mResult = createSharedRef<TrtResults>();
	}

public:
	SharedRef<TrtDeploy> mDeploy;
	SharedRef<TrtResults> mResult;
	std::vector<cv::Mat> mSampled;
};

static void *GenModel() {
	InferModel *model = new InferModel();
	return reinterpret_cast<void *>(model);
}

cvModel *Allocate_Algorithm(cv::Mat &input_frame, int algID, int gpuID) {
	std::string file;
	if(Util::checkFileExist("./infer_cfg.yaml"))
		file = "./infer_cfg.yaml";
	else if(Util::checkFileExist("../config/infer_cfg.yaml")){
		file = "../config/infer_cfg.yaml";
	}else{
		std::cout<<"Cannot find YAML file!"<<std::endl;
	}
	Config::LoadConfigFile(0, nullptr, file);
	auto *ptr = new cvModel();
	ptr->FrameNum = 0;
	ptr->Frameinterval = 0;
	ptr->countNum = 0;
	ptr->width = input_frame.cols;
	ptr->height = input_frame.rows;
	ptr->scaleX = (Config::TARGET_SIZE[1]) / (ptr->width);
	ptr->scaleY = (Config::TARGET_SIZE[0]) / (ptr->height);
	ptr->iModel = GenModel();
	COUNT = Config::SAMPLE_INTERVAL*Config::TRIGGER_LEN;
	return ptr;
}

void SetPara_Algorithm(cvModel *pModel, int algID) {
	//todo: implement this
}

void UpdateParams_Algorithm(cvModel *pModel) {
	//todo: implement this
}

void Process_Algorithm(cvModel *pModel, cv::Mat &input_frame) {
	if (COUNT_LOOP < COUNT) {
		auto model = reinterpret_cast<InferModel *>(pModel->iModel);
		if((COUNT_TOTAL%Config::SAMPLE_INTERVAL)==0){
			model->mSampled.emplace_back(input_frame.clone());
		}
		COUNT_LOOP++;
	}else{
		auto model = reinterpret_cast<InferModel *>(pModel->iModel);
		model->mDeploy->Infer(model->mSampled, model->mResult);
		std::vector<cv::Mat> temp(1,input_frame);
		model->mDeploy->Postprocessing(model->mResult,model->mSampled,temp,pModel->alarm);
		input_frame = temp[0].clone();
		model->mSampled.clear();
		model->mSampled.push_back(input_frame.clone());
		COUNT_LOOP = 1;
	}
	COUNT_TOTAL++;
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