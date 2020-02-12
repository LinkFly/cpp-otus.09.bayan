#pragma once

#include <functional>
#include <set>
#include <memory>

#include "share-types.h"

class EqualGroup {
	std::set<PFileReaded> filesSet;
public:
	EqualGroup() = default;
	EqualGroup(PFileReaded file): EqualGroup() {
		add(file);
	}

	void add(PFileReaded file) {
		filesSet.insert(file);
		/*file->eqGroup = std::shared_ptr<EqualGroup*/
	}
	//void union_with(PEqualGroup group) {
	//	// TODO Not implemented
	//}
	void forEachWhile(std::function<bool(PFileReaded fileReaded)> fnFileProcess) {
		for (auto file : filesSet) {
			bool isNext = fnFileProcess(file);
			if (!isNext) {
				break;
			}
		}
	}
};

//class EqualGroup {
//	std::set<shared_ptr<FileReaded>> equalFiles;
//public:
//	EqualGroup(shared_ptr<FileReaded> fileRes) {
//		equalFiles.insert(fileRes);
//	}
//	void add(shared_ptr<FileReaded> fileRes) {
//		equalFiles.insert(fileRes);
//	}
//};

