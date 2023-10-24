#include <opencv2/imgproc.hpp>
#include "postprocessor.h"
#include "config.h"
#include <opencv2/opencv.hpp>

namespace smoke
{

void SmokeDeployPost::Run(const SharedRef<TrtResults> &res, const std::vector<cv::Mat> &img,
						  std::vector<cv::Mat> &out_img, int &alarm)
{
	alarm = 0;
	std::vector<float> dets;
	std::vector<float> num_dets;
	res->Get(m_config->OUTPUT_NAMES[0], dets);
	res->Get(m_config->OUTPUT_NAMES[1], num_dets);

	float scale_x = (float)img[0].cols / (float)m_config->TARGET_SIZE[1];
	float scale_y = (float)img[0].rows / (float)m_config->TARGET_SIZE[0];

	std::vector<Box> curr;
	for (int j = 0; j < 100; ++j) {
		Box b;
		b.class_id = int(dets[0 + j * 6]);
		b.score = dets[1 + j * 6];
		b.x_min = (int)(dets[2 + j * 6] * scale_x);
		b.y_min = (int)(dets[3 + j * 6] * scale_y);
		b.x_max = (int)(dets[4 + j * 6] * scale_x);
		b.y_max = (int)(dets[5 + j * 6] * scale_y);
		curr.push_back(b);
//		if(b.score>m_config->SCORE_THRESHOLD)
//		std::cout<<"Class: "<<b.class_id<<"; Score: "<<b.score<<"; x0: "<<b.x_min
//		<<"; "<<"y0: "<<b.y_min<<"; x1: "<<b.x_max<<"; y1: "<<b.y_max<<std::endl;
	}

	auto b = curr;
	for (int k = 0; k < b.size(); ++k) {
		if (b[k].score > m_config->SCORE_THRESHOLD) {
			std::vector<unsigned char> box_color;
			box_color.resize(3);
			std::vector<unsigned char> text_color;
			box_color.resize(3);
			if (b[k].class_id == m_config->TARGET_CLASS) {
				box_color = m_config->ALARM_BOX_COLOR;
				text_color = m_config->ALARM_TEXT_COLOR;
			}
			else {
				box_color = m_config->BOX_COLOR;
				text_color = m_config->TEXT_COLOR;
			}
			Util::plotBox(out_img[0], b[k].x_min, b[k].y_min,
						  b[k].x_max, b[k].y_max,
						  box_color, m_config->BOX_LINE_WIDTH);
			const int line_type = 8;
			m_font->putText(out_img[0],
							m_config->POST_TEXT,
							cv::Point(b[k].x_min, b[k].y_min - m_config->TEXT_FONT_SIZE - 10),
							m_config->TEXT_FONT_SIZE,
							cv::Scalar(text_color[0], text_color[1], text_color[2]),
							(int)m_config->TEXT_LINE_WIDTH,
							line_type,
							false);

			if (b[k].class_id == m_config->TARGET_CLASS)alarm = 1;
		}
	}
}

void Postprocessor::Init()
{
	m_worker = new SmokeDeployPost(m_config);
}

void Postprocessor::Run(const SharedRef<TrtResults> &res, const std::vector<cv::Mat> &img,
						std::vector<cv::Mat> &out_img, int &alarm)
{
	if (!INIT_FLAG) {
		Init();
		INIT_FLAG = true;
	}
	m_worker->Run(res, img, out_img, alarm);
}

Postprocessor::~Postprocessor()
{
	if (m_worker) {
		delete m_worker;
		m_worker = nullptr;
	}
}

}
