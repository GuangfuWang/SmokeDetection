#include <opencv2/imgproc.hpp>
#include "postprocessor.h"
#include "config.h"

namespace gf {

    thread_local bool Postprocessor::INIT_FLAG = false;

    void SmokeDeployPost::Run(const SharedRef<TrtResults> &res, const std::vector<cv::Mat> &img,
                                   std::vector<cv::Mat> &out_img,int& alarm) {
        //our simple program will only draw letters on top of images.
        auto flag = static_cast<PostProcessFlag>(Config::POST_MODE);
        assert(flag == PostProcessFlag::DRAW_BOX_LETTER);

        std::vector<std::vector<Box>> boxes;

        int start = 0;
        std::vector<float> num_dets;
        std::vector<float> dets;
        res->Get(Config::OUTPUT_NAMES[0],dets);
        res->Get(Config::OUTPUT_NAMES[1],num_dets);
        float scale_x = img[0].cols/Config::TARGET_SIZE[1];
        float scale_y = img[0].rows/Config::TARGET_SIZE[0];
        for (int i = 0; i < Config::TRIGGER_LEN; ++i) {
            std::vector<Box> curr;
            for (int j = start; j < start+num_dets[i]; ++j) {
                Box b;
                b.class_id = std::round(dets[0+j*6]);
                b.score = dets[1+j*6];
                b.x_min = dets[2+j*6]*scale_x;
                b.y_min = dets[3+j*6]*scale_y;
                b.x_max = dets[4+j*6]*scale_x;
                b.y_max = dets[5+j*6]*scale_y;
                m_moving_average.push_back(b.score);
                if(b.x_min>0)std::cout<<"x_min: "<<b.x_min<<std::endl;
                curr.push_back(b);
            }
            boxes.push_back(curr);
            start += std::round(num_dets[i]);
        }
        if (boxes.empty())return;
        float sum = 0.0f;
        if(m_moving_average.size()>2*Config::TRIGGER_LEN){
            int n = m_moving_average.size();
            for (int i = n-1; i > n-1-n/2&&i>=0; i--) {
                sum+=m_moving_average[i];
            }
            m_moving_average.erase(m_moving_average.begin());
        }
        std::stringstream text;
        text << Config::POST_TEXT << ": " << 100 * sum << "%";
        std::cout<<text.str()<<std::endl;
		if(sum>Config::THRESHOLD)m_latency = 8;
        for (int i = 0; i < img.size(); ++i) {
            ///@note the putText method does not have GPU version since it quite slow running on GPU for per pixel ops.
            if (sum>=Config::THRESHOLD||m_latency){
                for (int j = 0; j < Config::SAMPLE_INTERVAL; ++j) {
                    cv::putText(out_img[i*Config::SAMPLE_INTERVAL+j], text.str(),
                                cv::Point(Config::TEXT_OFF_X, Config::TEXT_OFF_Y),
                                cv::FONT_HERSHEY_PLAIN, Config::TEXT_FONT_SIZE,
                                cv::Scalar(Config::TEXT_COLOR[0], Config::TEXT_COLOR[1], Config::TEXT_COLOR[2]),
                                (int) Config::TEXT_LINE_WIDTH);
                }
				m_latency--;
				alarm = 1;
            }else alarm = 0;
            for (int j = 0; j < Config::SAMPLE_INTERVAL; ++j) {
                auto b = boxes[i];
                for (int k = 0; k < b.size(); ++k) {
                    if(b[k].score>Config::SCORE_THRESHOLD)
                        Util::plotBox(out_img[i*Config::SAMPLE_INTERVAL+j],b[k].x_min,b[k].y_min,
                                      b[k].x_max,b[k].y_max,
                                      Config::BOX_COLOR,Config::BOX_LINE_WIDTH);
                }
            }
        }
    }

    void Postprocessor::Init() {
        if (!m_ops) {
            m_ops = createSharedRef<Factory<PostprocessorOps>>();
        }
        m_ops->registerType<SmokeDeployPost>(Config::POSTPROCESS_NAME);
        m_worker = m_ops->create(Config::POSTPROCESS_NAME);
    }

    void Postprocessor::Run(const SharedRef<TrtResults> &res, const std::vector<cv::Mat> &img,
                            std::vector<cv::Mat> &out_img,int& alarm) {
        if (!INIT_FLAG) {
            Init();
            INIT_FLAG = true;
        }
        m_worker->Run(res, img, out_img,alarm);
    }

    Postprocessor::~Postprocessor() {
        if (m_ops) {
            m_ops->destroy();
        }
    }

}