#ifndef __UTILS_H__
#define __UTILS_H__

#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>

#include "colorio.hpp"

namespace fs = std::filesystem;

namespace pp {

	namespace codes {
		constexpr auto mainEpp = R"(
lib std

When it starts
	It printf about "Hello, world"
)";
		auto conf(std::string st) {
			return (R"(
Licence         MIT License
ProjectName     )" + st + R"(
Version         0.1.0

CppStandard     c++17
Transpile       en++ .\src\main
Compile         g++ -o out %s -std=%s -fconcepts
)");
		}
		constexpr auto stdlib = R"(
#ifndef __EPP_STD__
#define __EPP_STD__

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace estd {
	template<typename T1, typename T2>
	auto range(T1 start, T2 end, int diff)->vector<T1> {
		vector<T1> ret;
		for (T1 i = 0; i <= end; i += diff) {
			ret.push_back(i);
		}
		return ret;
	}

	class Range {
	private:
		int start;
		int End;
		int diff;
	public:
		Range(int _end) {
			start = 0;
			End = _end;
			diff = 1;
		}
		Range(int _start, int _end, int _diff = 1) {
			start = _start;
			End = _end;
			diff = _diff;
			if (diff == 0) {
				throw std::exception("diff cannot be 0");
			}
		}
		const int* toList() {
			int* list = new int[(End - start + 1) / diff];
			int* ins = list;
			if (diff > 0) {
				for (int i = start; i <= End; i += diff) {
					*ins = i;
					ins++;
				}
			}
			else {
				for (int i = End; i >= start; i += diff) {
					*ins = i;
					ins++;
				}
			}
			return list;
		}
		class iterator {
		private:
			int _diff;
		public:
			int _val;

			iterator(int v, int d):_val(v), _diff(d) {}
			auto operator++()->iterator& {
				_val += _diff;
				return *this;
			}
			inline int operator*() {
				return _val;
			}
			int operator==(iterator i) {
				return (i._val == _val);
			}
			int operator!=(iterator i) {
				return (i._val != _val);
			}
		};
		inline auto begin()->iterator {
			return iterator(start, diff);
		}
		inline auto end()->iterator {
			return iterator(End + diff, diff);
		}
	};
}

#endif
)";
		inline constexpr auto logo = R"(
 ::::::::  :::::::::   ::::::::  
:+:    :+: :+:    :+: :+:    :+: 
       +:+ +:+    +:+ +:+    +:+ 
    +#++:  +#++:++#+  +#+    +:+ 
       +#+ +#+        +#+    +#+ 
#+#    #+# #+#        #+#    #+# 
 ########  ###         ########  
)";
	}
	
	auto setup(int argc, char** argv)->void {
		std::string name(argv[2]);
		auto path = fs::current_path() / name;
		fs::create_directory(path);
		fs::create_directory(path / "src");
		fs::create_directory(path / "src" / "lib");
		fs::create_directory(path / "src" / "modules");
		fs::create_directory(path / "target");
		fs::create_directory(path / "target" / "debug");
		fs::create_directory(path / "target" / "release");

		(std::ofstream(path / "src" / "main.epp") << codes::mainEpp).close();
		(std::ofstream(path / "config.eppconf") << codes::conf(name)).close();
		(std::ofstream(path / "src" / "lib" / "eppstd.hpp") << codes::stdlib).close();
	}

	using sMap = std::map<std::string, std::string>;

	auto parseConf(fs::path configpath)->std::map<std::string, std::string> {
		std::ifstream configFile(configpath);
		std::string buff;
		std::map<std::string, std::string> configs;
		while (std::getline(configFile, buff)) {
			if (buff.length() > 2) {
				auto valueName = buff.substr(0, 16);
				while (valueName.back() == ' ') {
					valueName.pop_back();
				}
				configs.insert(std::pair(valueName, buff.substr(16)));
			}
		}
		configFile.close();
		return configs;
	}

	auto transpile(sMap& configs)->void {
		scss("Transpiling " + configs["ProjectName"] + " (" + configs["Transpile"] + ")\n");
		try {
			std::cout << "Transpilation code: " << system((configs["Transpile"]).c_str()) << std::endl;
			scss("Sucessed to transpile main.epp to main.cpp!\n");
		}
		catch (const std::exception e) {
			err(e.what());
		}
	}

	auto compile(sMap& configs)->void {
		char s[80];
		sprintf(s, configs["Compile"].c_str(), ".\\src\\main.cpp", configs["CppStandard"].c_str());
		scss("Compiling " + configs["ProjectName"] + " (" + s + ")\n");
		system(s);
		
		if (fs::exists(fs::current_path() / "target" / "debug" / (configs["ProjectName"] + ".exe"))) {
			fs::remove(fs::current_path() / "target" / "debug" / (configs["ProjectName"] + ".exe"));
		}
		if (fs::exists(fs::current_path() / "target" / "debug" / "out.cpp")) {
			fs::remove(fs::current_path() / "target" / "debug" / "out.cpp");
		}
		fs::copy(fs::current_path() / "out.exe", fs::current_path() / "target" / "debug" / (configs["ProjectName"] + ".exe"));
		fs::copy(fs::current_path() / "src" / "main.cpp", fs::current_path() / "target" / "debug" / "out.cpp");
		fs::remove(fs::current_path() / "out");
		fs::remove(fs::current_path() / "out.exe");
		fs::remove(fs::current_path() / "src" / "main.cpp");
		
		scss("Sucessed to Compile main.cpp to out!\n");
	}
}

#endif