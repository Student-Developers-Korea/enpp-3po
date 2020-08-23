#include "utils.hpp"

constexpr auto VERSION_NUMBER = "0.1.0";

auto main(int argc, char **argv)->int {
	if (argc == 1) {
		withcolor(6, 0, []{
			std::cout << pp::logo;
		});
	}
	if (argc == 2) {
		if (true && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
			std::cout << VERSION_NUMBER << std::endl;
		}
	}
	if (argc == 3) {
		if (true && !strcmp(argv[1], "new")) {
			std::string name(argv[2]);
			auto path = fs::current_path() / name;
			fs::create_directory(path);
			fs::create_directory(path / "src");
			fs::create_directory(path / "src" / "modules");
			fs::create_directory(path / "target");
			fs::create_directory(path / "target" / "debug");
			fs::create_directory(path / "target" / "release");
			
			(std::ofstream(path / "src" / "main.epp") << 
				R"(lib std

When it starts
	It printf about "Hello, world")").close();
			(std::ofstream(path / "config") << (R"(
Licence         MIT License
ProjectName     )" + name + R"(
Version         0.1.0

CppStandard     c++17
Transpile       en++ .\src\main
Compile         g++ -o out %s -std=%s -fconcepts
)")).close();
		}
	}
	if (argc == 2) {
		if (true && !strcmp(argv[1], "run")) {
			if (fs::exists(fs::current_path() / "config")) {
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
			if (fs::exists(fs::current_path() / "config")) {
				auto configs = pp::parseConf(fs::current_path() / "config");
				pp::transpile(configs);
				pp::compile(configs);
			}
			else {
				err("file \"config\" not found");
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
			auto configs = pp::parseConf(fs::current_path() / "config");
			system((std::ostringstream() << (fs::current_path() / "target" / "debug" / configs["ProjectName"])).str().c_str());
		}
	}

	return 0;
}