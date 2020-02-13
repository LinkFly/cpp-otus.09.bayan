#pragma once

#include <functional>
//#include <set>
#include <map>
#include <memory>
#include <string>
#include <iostream>

#include "share-types.h"

using std::string;
using std::cout;

#include "FileReaded.h"
//class FileReaded;

class EqualGroupBase {
	/*static inline Id nextId = 0;*/
public:
	/*Id id = nextId++;*/
	std::map<string, PFileReaded> filesSet;
	EqualGroupBase() = default;
	EqualGroupBase(PFileReaded file): EqualGroupBase() {
		add(file);
	}
	~EqualGroupBase() {
		/*cout << "===================== ~EqualGroupBase (id = " << (int)id << ") =======================\n";*/
	}
	void add(PFileReaded file) {
		/*filesSet.emplace(file->getFilePath(), file);*/
		/*FileReaded f;*/
		/*string filePath = file->getFilePath();*/
		/*filesSet[filePath] = file;*/
		/*filesSet.insert(std::tuple<Id, PFileReaded>(id, file));*/
	}

	void forEachWhile(std::function<bool(PFileReaded fileReaded)> fnFileProcess) {
		for (auto file : filesSet) {
			bool isNext = fnFileProcess(std::get<1>(file));
			if (!isNext) {
				break;
			}
		}
	}
};


