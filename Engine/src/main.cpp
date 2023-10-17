#include "Game.h"
#include "PCSTree.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

#include "json.hpp"
#include "tiny_gltf.h"
#include "modelData.pb.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	static_cast<void>(prevInstance);
	static_cast<void>(cmdLine);

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	tinygltf::ColorValue v = { 1, 2, 3 };
	tinygltf::Model model;

	modelData_proto prototmp;

	nlohmann::json printer = {
		{"Engine", true},
		{"Version", 1.0},
	};

	Trace::out(printer.dump(4).c_str());
	Trace::out("\n\n");

	int output = Azul::Game::Launch(hInstance, cmdShow);
	Azul::Game::Teardown();

	google::protobuf::ShutdownProtobufLibrary();

	return output;
}