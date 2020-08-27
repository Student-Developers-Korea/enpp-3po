#include "utils.hpp"

constexpr auto VERSION_NUMBER = "0.1.0";

auto main(int argc, char **argv)->int {
	if (argc == 1) {
		withcolor(6, 0, []{
			std::cout << pp::codes::logo;
		});
	}
	if (argc == 2) {
		if (true && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
			std::cout << VERSION_NUMBER << std::endl;
		}
	}
	if (argc == 3) {
		if (true && !strcmp(argv[1], "new")) {
			pp::setup(argc, argv);
		}
	}
	if (argc == 2) {
		if (true && !strcmp(argv[1], "run")) {
			if (fs::exists(fs::current_path() / "config.eppconf")) {
				auto configs = pp::parseConf(fs::current_path() / "config");

				pp::transpile(configs);

				pp::compile(configs);
				
				scss("Running " + configs["ProjectName"] + "...\n");
				system((std::ostringstream() << (fs::current_path() / "target" / "debug" / configs["ProjectName"])).str().c_str());
			}
			else {
				err("file \"config\" not found");
			}
		}
	}
	if (argc == 2) {
		if (true && !strcmp(argv[1], "build")) {
			if (fs::exists(fs::current_path() / "config.eppconf")) {
				auto configs = pp::parseConf(fs::current_path() / "config.eppconf");
				pp::transpile(configs);
				pp::compile(configs);
			}
			else {
				err("file \"config.eppconf\" not found");
			}
		}
	}
	if (argc == 3) {
		if (true && !strcmp(argv[1], "install")) {
			std::string name(argv[2]);

		}
	}
	if (argc == 2) {
		if (true && !strcmp(argv[1], "just-run")) {
			auto configs = pp::parseConf(fs::current_path() / "config.eppconf");
			system((std::ostringstream() << (fs::current_path() / "target" / "debug" / configs["ProjectName"])).str().c_str());
		}
	}

	return 0;
}