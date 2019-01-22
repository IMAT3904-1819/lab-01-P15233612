#include <iostream>
#include "glog\logging.h"
#include "IGameEigine.h"

int main(int argc, char* argv[])
{
	google::SetCommandLineOption("GLOG_minloglevel", "0");
	gflags::SetUsageMessage("This Program Test GPUVideoCodec Library.");
	gflags::SetVersionString("0.1.0.0");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	auto obj = game::IGameEigine::GetInstance();
	obj->createWindow(1920, 1080, std::string("myGame"));
	obj->initEngine(std::string("./config.prototxt"));
	return 0;
}