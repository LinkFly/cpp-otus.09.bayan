#pragma once

#include <functional>
//#include <set>
#include <map>
#include <memory>
#include <string>
#include <iostream>

#include "FileReaded.h"

using std::string;
using std::cout;

class EqualGroup {
	std::map<string, PFileReaded> filesSet;
public:
	EqualGroup() = default;
	EqualGroup(PFileReaded file): EqualGroup() {
		add(file);
	}
	void add(PFileReaded file) {
		string filePath = file->getFilePath();
		filesSet[filePath] = file;
	}

	template<class Lambda>
	void forEachWhile(const Lambda& fnFileProcess) {
		for (auto file : filesSet) {
			bool isNext = fnFileProcess(std::get<1>(file));
			if (!isNext) {
				break;
			}
		}
	}
	/*void forEachWhile(std::function<bool(PFileReaded fileReaded)> fnFileProcess) {
		for (auto file : filesSet) {
			bool isNext = fnFileProcess(std::get<1>(file));
			if (!isNext) {
				break;
			}
		}
	}*/
};


