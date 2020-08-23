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

	using sMap = std::map<std::string, std::string>;

    inline constexpr auto logo = R"(
 ::::::::  :::::::::   ::::::::  
:+:    :+: :+:    :+: :+:    :+: 
       +:+ +:+    +:+ +:+    +:+ 
    +#++:  +#++:++#+  +#+    +:+ 
       +#+ +#+        +#+    +#+ 
#+#    #+# #+#        #+#    #+# 
 ########  ###         ########  
)";

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