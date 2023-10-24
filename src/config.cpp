#include "config.h"
#include "macro.h"
#include "util.h"

namespace smoke
{


void Config::LoadConfigFile(int argc, char **argv, const std::string &file)
{
	if (init)return;
	MODEL_NAME = DEPLOY_MODEL;
	INPUT_NAME.clear();
	INPUT_NAME = Util::parseNames(MODEL_INPUT_NAME);
	OUTPUT_NAMES.clear();
	OUTPUT_NAMES = Util::parseNames(MODEL_OUTPUT_NAMES);

	if (!Util::checkFileExist(file)) {
		std::cerr << "Config file non exists! Aborting..." << std::endl;
	}

	YAML::Node config;
	config = YAML::LoadFile(file);

	if (config["MODEL"].IsDefined()) {
		auto model_node = config["MODEL"];
		if (model_node["MODEL_NAME"].IsDefined()) {
			MODEL_NAME = model_node["MODEL_NAME"].as<std::string>();
		}
		if (model_node["BACKBONE"].IsDefined()) {
			BACKBONE = model_node["BACKBONE"].as<std::string>();
		}
		if (model_node["INPUT_NAME"].IsDefined()) {
			INPUT_NAME = model_node["INPUT_NAME"].as<std::vector<std::string>>();
		}
		if (model_node["OUTPUT_NAMES"].IsDefined()) {
			OUTPUT_NAMES = model_node["OUTPUT_NAMES"].as<std::vector<std::string>>();
		}
	}
	else {
		std::cerr << "Please set MODEL, " << std::endl;
	}

	if (config["DATA"].IsDefined()) {
		auto model_node = config["DATA"];
		if (model_node["VIDEO_NAME"].IsDefined()) {
			VIDEO_FILE = model_node["VIDEO_NAME"].as<std::string>();
		}
		if (model_node["RTSP_SITE"].IsDefined()) {
			RTSP_SITE = model_node["RTSP_SITE"].as<std::string>();
		}
		if (model_node["INPUT_SHAPE"].IsDefined()) {
			INPUT_SHAPE = model_node["INPUT_SHAPE"].as<std::vector<int>>();
		}
	}
	else {
		std::cerr << "Please set DATA, " << std::endl;
	}

	if (config["PIPELINE"].IsDefined()) {
		auto model_node = config["PIPELINE"];
		if (model_node["STRIDE"].IsDefined()) {
			STRIDE = model_node["STRIDE"].as<unsigned int>();
		}
		if (model_node["INTERP"].IsDefined()) {
			INTERP = model_node["INTERP"].as<unsigned int>();
		}
		if (model_node["SAMPLE_INTERVAL"].IsDefined()) {
			SAMPLE_INTERVAL = model_node["SAMPLE_INTERVAL"].as<unsigned int>();
		}
		if (model_node["TRIGGER_LEN"].IsDefined()) {
			TRIGGER_LEN = model_node["TRIGGER_LEN"].as<unsigned int>();
		}
		if (model_node["BATCH_SIZE"].IsDefined()) {
			BATCH_SIZE = model_node["BATCH_SIZE"].as<unsigned int>();
		}
		if (model_node["THRESHOLD"].IsDefined()) {
			THRESHOLD = model_node["THRESHOLD"].as<float>();
		}
		if (model_node["SCORE_THRESHOLD"].IsDefined()) {
			SCORE_THRESHOLD = model_node["SCORE_THRESHOLD"].as<float>();
		}
		if (model_node["TARGET_CLASS"].IsDefined()) {
			TARGET_CLASS = model_node["TARGET_CLASS"].as<unsigned int>();
		}
		if (model_node["ENABLE_SCALE"].IsDefined()) {
			ENABLE_SCALE = model_node["ENABLE_SCALE"].as<bool>();
		}
		if (model_node["KEEP_RATIO"].IsDefined()) {
			KEEP_RATIO = model_node["KEEP_RATIO"].as<bool>();
		}
		if (model_node["TIMING"].IsDefined()) {
			TIMING = model_node["TIMING"].as<bool>();
		}
		if (model_node["TARGET_SIZE"].IsDefined()) {
			TARGET_SIZE = model_node["TARGET_SIZE"].as<std::vector<int>>();
		}
		if (model_node["TRAIN_SIZE"].IsDefined()) {
			TRAIN_SIZE = model_node["TRAIN_SIZE"].as<std::vector<int>>();
		}
		if (model_node["SHORT_SIZE"].IsDefined()) {
			SHORT_SIZE = model_node["SHORT_SIZE"].as<unsigned int>();
		}
		if (model_node["PIPELINE_TYPE"].IsDefined()) {
			PIPELINE_TYPE = model_node["PIPELINE_TYPE"].as<std::vector<std::string>>();
		}
		if (model_node["N_MEAN"].IsDefined()) {
			N_MEAN = model_node["N_MEAN"].as<std::vector<float>>();
		}
		if (model_node["N_STD"].IsDefined()) {
			N_STD = model_node["N_STD"].as<std::vector<float>>();
		}

	}
	else {
		std::cerr << "Please set PIPELINE, " << std::endl;
	}

	if (config["POSTPROCESS"].IsDefined()) {
		auto model_node = config["POSTPROCESS"];
		if (model_node["POST_MODE"].IsDefined()) {
			POST_MODE = model_node["POST_MODE"].as<int>();
		}
		if (model_node["TEXT_COLOR"].IsDefined()) {
			TEXT_COLOR = model_node["TEXT_COLOR"].as<std::vector<unsigned char>>();
			std::swap(TEXT_COLOR[0], TEXT_COLOR[2]);
		}
		if (model_node["BOX_COLOR"].IsDefined()) {
			BOX_COLOR = model_node["BOX_COLOR"].as<std::vector<unsigned char>>();
			std::swap(BOX_COLOR[0], BOX_COLOR[2]);
		}
		if (model_node["ALARM_TEXT_COLOR"].IsDefined()) {
			ALARM_TEXT_COLOR = model_node["ALARM_TEXT_COLOR"].as<std::vector<unsigned char>>();
			std::swap(ALARM_TEXT_COLOR[0], ALARM_TEXT_COLOR[2]);
		}
		if (model_node["ALARM_BOX_COLOR"].IsDefined()) {
			ALARM_BOX_COLOR = model_node["ALARM_BOX_COLOR"].as<std::vector<unsigned char>>();
			std::swap(ALARM_BOX_COLOR[0], ALARM_BOX_COLOR[2]);
		}
		if (model_node["TEXT_LINE_WIDTH"].IsDefined()) {
			TEXT_LINE_WIDTH = model_node["TEXT_LINE_WIDTH"].as<float>();
		}
		if (model_node["BOX_LINE_WIDTH"].IsDefined()) {
			BOX_LINE_WIDTH = model_node["BOX_LINE_WIDTH"].as<float>();
		}
		if (model_node["TEXT_FONT_SIZE"].IsDefined()) {
			TEXT_FONT_SIZE = model_node["TEXT_FONT_SIZE"].as<float>();
		}
		if (model_node["TEXT_OFF_X"].IsDefined()) {
			TEXT_OFF_X = model_node["TEXT_OFF_X"].as<int>();
			if (TEXT_OFF_X < 0) {
				TEXT_OFF_X = INPUT_SHAPE.back() / 2 - 5;
			}
		}
		if (model_node["TEXT_OFF_Y"].IsDefined()) {
			TEXT_OFF_Y = model_node["TEXT_OFF_Y"].as<int>();
		}
		if (model_node["POSTPROCESS_NAME"].IsDefined()) {
			POSTPROCESS_NAME = model_node["POSTPROCESS_NAME"].as<std::string>();
		}
		if (model_node["POST_TEXT"].IsDefined()) {
			POST_TEXT = model_node["POST_TEXT"].as<std::string>();
		}
		if (model_node["DET_SCORE"].IsDefined()) {
			DET_SCORE = model_node["DET_SCORE"].as<std::string>();
		}
		if (model_node["DET_CLASS_ID"].IsDefined()) {
			DET_CLASS_ID = model_node["DET_CLASS_ID"].as<std::string>();
		}
		if (model_node["DET_COORD"].IsDefined()) {
			DET_COORD = model_node["DET_COORD"].as<std::string>();
		}
		if (model_node["DET_NUM_BBOX"].IsDefined()) {
			DET_NUM_BBOX = model_node["DET_NUM_BBOX"].as<std::string>();
		}
		if (model_node["POST_TEXT_FONT_FILE"].IsDefined()) {
			POST_TEXT_FONT_FILE = model_node["POST_TEXT_FONT_FILE"].as<std::string>();
		}
	}
	else {
		std::cerr << "Please set MODEL, " << std::endl;
	}
	init = true;
	if (argc < 2)return;
	if (argv) {
		cmdline::parser parser;
		parser.add<std::string>("input_name", 'i', "Input layer name for trt.", false);
		parser.add<std::string>("output_names", 'o', "Output layer names for trt.", false);
		parser.add<std::string>("model_name", 'm', "Model name for trt.", false);
		parser.add<std::string>("video_file", 'v', "Video file for trt.", false);
		parser.parse_check(argc, argv);

		std::string InLayerName = parser.get<std::string>("input_name");
		std::string OutLayerNames = parser.get<std::string>("output_names");
		std::string ModelName = parser.get<std::string>("model_name");
		std::string VideoFile = parser.get<std::string>("video_file");

		if (!InLayerName.empty()) {
			INPUT_NAME = Util::parseNames(InLayerName);
		}
		if (!OutLayerNames.empty()) {
			OUTPUT_NAMES.clear();
			OUTPUT_NAMES = Util::parseNames(OutLayerNames);
		}
		if (!ModelName.empty()) {
			MODEL_NAME = ModelName;
		}

		if (!smoke::Util::checkFileExist(MODEL_NAME)) {
			std::cout << MODEL_NAME << std::endl;
			std::cerr << "Model does not exists!" << std::endl;
			std::cerr << "Please check the model path..." << std::endl;
		}

		if (!VideoFile.empty()) {
			VIDEO_FILE = VideoFile;
		}

	}
}

}

