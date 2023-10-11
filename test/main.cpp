#include "../src/config.h"
#include "../src/trt_deploy.h"
#include "../src/trt_deployresult.h"
#include "src/model.h"

using namespace gf;
using namespace smoke;

/**
 * @example
 * @param argc number of input params, at least 1.
 * @param argv params lists
 * @return
 */

int main(int argc, char **argv) {
    Config::LoadConfigFile(argc, argv, "../config/infer_cfg.yaml");
    //prepare the input data.
    auto in_path = std::filesystem::path(Config::VIDEO_FILE);
    cv::VideoCapture cap(in_path);
    cv::VideoWriter vw;
    std::filesystem::path output_path = in_path.parent_path() / (in_path.stem().string() + ".result.mp4");
    vw.open(output_path,
            cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
            cap.get(cv::CAP_PROP_FPS),
            cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    size_t total = cap.get(cv::CAP_PROP_FRAME_COUNT), current = 0;
    std::vector<std::vector<cv::Mat>> imgs;
    cv::Mat img;
    int c = 0;
    std::vector<std::vector<cv::Mat>> unsampled;
    std::vector<cv::Mat> cur;
    std::vector<cv::Mat> whole;
    int num = total / (Config::TRIGGER_LEN * Config::SAMPLE_INTERVAL);
    while (cap.read(img)) {
        if (c / (Config::TRIGGER_LEN * Config::SAMPLE_INTERVAL) == num)break;
        if (c % Config::SAMPLE_INTERVAL == 0) {
            cur.emplace_back(img.clone());
        }
        whole.emplace_back(img.clone());
        c++;
        if (cur.size() == Config::TRIGGER_LEN) {
            imgs.emplace_back(cur);
            unsampled.emplace_back(whole);
            cur.clear();
            whole.clear();
        }
    }
    c = 0;
    auto model =  Allocate_Algorithm(imgs[0][0], 0, 0);
    SetPara_Algorithm(model,0);
    UpdateParams_Algorithm(model);
    for (auto &each: imgs) {
        Process_Algorithm(model, each[0]);
        c++;
    }
    for (auto &f: imgs) {
        for (auto & single:f) {
            vw.write(single);
        }
    }
    unsampled.clear();
    Destroy_Algorithm(model);

    return 0;
}