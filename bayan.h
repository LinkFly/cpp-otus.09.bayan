#pragma once

#include "share.h"

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <map>
#include <set>
#include <fstream>

#include <boost/filesystem.hpp>

#include "FileReaded.h"
#include "Hash.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::ifstream;
using std::list;

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

namespace fs = boost::filesystem;


class EqualGroupsCollection {
	list<PEqualGroup> equalGroups;
public:
	void add(PEqualGroup group) {
		equalGroups.push_back(group);
	}
	void forEach(std::function<void(PEqualGroup & group)> fnGroupProcess) {
		for (auto group : equalGroups) {
			fnGroupProcess(group);
		}
	}
};

using Id = uint8_t;
using PFileReaded = shared_ptr<FileReaded>;

// TODO vector<paths>
class Bayan {

	vector<string> files;
	unsigned blockSize;
	std::map<string, vector<string>> filesHashes;
	vector<PFileReaded> readedFiles;
	EqualGroupsCollection groupsCol;

	class CheckTable {
		using vecbool = vector<bool>;
		vector<vecbool>  negTable;
	public:
		void init(size_t size) {
			negTable.resize(size);
			for (auto& vecBool : negTable) {
				vecBool.resize(size);
			}
		}
		bool isNotEqual(PFileReaded fileLeft, PFileReaded fileRight) {
			return negTable[fileLeft->id][fileRight->id];
		}
		void setNotEqual(PFileReaded fileLeft, PFileReaded fileRight) {
			negTable[fileLeft->id][fileRight->id] = true;
			negTable[fileRight->id][fileLeft->id] = true;
		}
	} checkTable;

	enum class CompareResult: int8_t { Unknown = -1, NotEqual, Equal };

	void saveCompareResult(PFileReaded fileLeft, PFileReaded fileRight, bool result) {
		if (!result) {
			checkTable.setNotEqual(fileLeft, fileRight);
		}
		else {
			// TODO Refactor repeat code (see below)
			bool isBothHaveGroups = !fileLeft->isEmptyGroup() && !fileRight->isEmptyGroup();
			bool isOnlyLeftHaveGroup = !fileLeft->isEmptyGroup() && fileRight->isEmptyGroup();
			bool isOnlyRightHaveGroup = fileLeft->isEmptyGroup() && !fileRight->isEmptyGroup();
			bool isBothNoHaveGroups = fileLeft->isEmptyGroup() && fileRight->isEmptyGroup();
			if (isBothHaveGroups) {
				if (fileLeft->eqGroup == fileRight->eqGroup) {
					return;
				}
				else {
					error("For result == true, files mustn't exists into both different groups\n");
				}
			}
			if (isBothNoHaveGroups) {
				PEqualGroup eqGroup = std::make_shared<EqualGroup>();
				eqGroup->add(fileLeft);
				fileLeft->eqGroup = eqGroup;
				eqGroup->add(fileRight);
				fileRight->eqGroup = eqGroup;
				groupsCol.add(eqGroup);
				return;
			}
			if (isOnlyLeftHaveGroup) {
				fileLeft->eqGroup->add(fileRight);
				fileRight->eqGroup = fileLeft->eqGroup;
			}
			else if (isOnlyRightHaveGroup) {
				fileRight->eqGroup->add(fileLeft);
				fileLeft->eqGroup = fileRight->eqGroup;
			}
		}
	}

	CompareResult checkCompareResult(PFileReaded fileLeft, PFileReaded fileRight, list<PFileReaded> history = list<PFileReaded>{}) {
		for (auto other : history) {
			if (other == fileRight)
				return CompareResult::Unknown;
		}
		bool isBothHaveGroups = !fileLeft->isEmptyGroup() && !fileRight->isEmptyGroup();
		bool isOnlyLeftHaveGroup = !fileLeft->isEmptyGroup() && fileRight->isEmptyGroup();
		bool isOnlyRightHaveGroup = fileLeft->isEmptyGroup() && !fileRight->isEmptyGroup();
		bool isBothNoHaveGroups = fileLeft->isEmptyGroup() && fileRight->isEmptyGroup();
		if (isBothHaveGroups || isBothNoHaveGroups) {
			history.push_back(fileRight);
			if (isBothHaveGroups) {
				if (fileRight->eqGroup == fileRight->eqGroup) {
					return CompareResult::Equal;
				}
				else {
					return CompareResult::NotEqual;
				}
			}
			if (isBothNoHaveGroups) {
				return CompareResult::Unknown;
			}
		}

		////// Exists group in left or in right //////
		PEqualGroup curGroup;
		PFileReaded curAnalyzedFile;
		PFileReaded otherFile;
		if (isOnlyLeftHaveGroup) {
			curGroup = fileLeft->eqGroup;
			curAnalyzedFile = fileRight;
			otherFile = fileLeft;
		}
		if (isOnlyRightHaveGroup) {
			curGroup = fileRight->eqGroup;
			curAnalyzedFile = fileLeft;
			otherFile = fileRight;
		}

		history.push_back(otherFile);
		CompareResult checkForFilesInGroup = CompareResult::Unknown;
		curGroup->forEachWhile([this, &curAnalyzedFile, &otherFile, &checkForFilesInGroup, &history](PFileReaded file) {
			CompareResult checkRes = checkCompareResult(curAnalyzedFile, file, history);
			if (checkRes == CompareResult::Unknown)
				return true;
			checkForFilesInGroup = checkRes;
			return false;
		});
		return checkForFilesInGroup;
	}

	bool compareFilesByBlocks(PFileReaded fileLeft, PFileReaded fileRight) {
		if (fileLeft->getSize() != fileRight->getSize()) {
			return false;
		}

		shared_ptr<Hash> hashLeft, hashRight;
		fileLeft->startReading();
		fileRight->startReading();
		bool res = true;
		while (fileLeft->readNextHash(hashLeft) && fileRight->readNextHash(hashRight)) {
			if (*hashLeft != *hashRight) {
				res = false;
				break;
			}
		}
		fileLeft->finishReading();
		fileRight->finishReading();
		return res;
	}
public:
	Bayan(uint8_t blockSize, vector<string> files): blockSize(blockSize), files(files) {
		for (auto file : files) {
			auto fileReaded = std::make_shared<FileReaded>(file);
			readedFiles.push_back(fileReaded);
		}
		checkTable.init(readedFiles.size());
	}

	void compareFiles(PFileReaded& fileLeft, PFileReaded& fileRight) {
		CompareResult checkRes = checkCompareResult(fileLeft, fileRight);
		bool isEqual;
		if (checkRes != CompareResult::Unknown) {
			isEqual = checkRes == CompareResult::Equal;
		}
		else {
			isEqual = compareFilesByBlocks(fileLeft, fileRight);
		}
		saveCompareResult(fileLeft, fileRight, isEqual);
	}

	void run() {
		size_t size = readedFiles.size();
		for (size_t i = 0; i < size - 1; i++) {
			auto curLeftFile = readedFiles[i];
			for (size_t j = i + 1; j < size; j++) {
				compareFiles(curLeftFile, readedFiles[j]);
			}
		}
	}

	void printGroups(std::ostream& out) {
		groupsCol.forEach([&out](PEqualGroup& group) {
			group->forEachWhile([&out](PFileReaded& file) {
				out << file->getFilePath() << endl;
				return true;
			});
			out << endl;
		});
	}
};
