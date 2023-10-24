#include "../src/config.h"
#include "../src/trt_deploy.h"
#include "../src/trt_deployresult.h"
#include "src/model.h"

using namespace smoke;
using namespace smoke;

/**
 * @example
 * @param argc number of input params, at least 1.
 * @param argv params lists
 * @return
 */

int main(int argc, char **argv) {
    //prepare the input data.
    auto in_path = std::filesystem::path("/home/wgf/Downloads/datasets/smoke/smoke.mp4");
    cv::VideoCapture cap(in_path);
    cv::VideoWriter vw;
    std::filesystem::path output_path = in_path.parent_path() / (in_path.stem().string() + ".result.mp4");
    vw.open(output_path,
            cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
            cap.get(cv::CAP_PROP_FPS),
            cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    cv::Mat img;
    std::vector<cv::Mat> whole;
	bool init = false;
	cvModel * model = nullptr;
    while (cap.isOpened()) {
		cap.read(img);
		if(img.cols==0||img.rows==0)break;
        if(!init){
			model =  Allocate_Algorithm(img, 0, 0);
			SetPara_Algorithm(model,0);
			UpdateParams_Algorithm(model);
			init = true;
		}
		Process_Algorithm(model, img);
		vw.write(img.clone());
    }
    Destroy_Algorithm(model);

    return 0;
}