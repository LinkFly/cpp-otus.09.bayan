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
	}

	void forEachWhile(std::function<bool(PFileReaded fileReaded)> fnFileProcess) {
		for (auto file : filesSet) {
			bool isNext = fnFileProcess(file);
			if (!isNext) {
				break;
			}
		}
	}
};


