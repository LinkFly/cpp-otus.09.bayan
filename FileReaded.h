#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include "share.h"
#include "Config.h"
#include "Block.h"
#include "Hash.h"
#include "FilesUtils.h"
#include "logger.h"

namespace fs = boost::filesystem;

using std::string;
using std::ifstream;
using std::vector;
using std::shared_ptr;

class FileReaded {
	string path{};
	std::unique_ptr<ifstream> pfile;
	size_t size = 0;
	size_t curBlockNumber = 0;
	bool isStartedReading = false;
	vector<shared_ptr<Hash>> blocksHashes;
	string filePath = "";
	const Config& config;

	void maybeError() {
		if (!pfile->good() && !pfile->eof()) {
			error("Input/output error");
		}
	}

	void open() {
		if (!isOpen()) {
			pfile.reset(new ifstream{});
			maybeError();
			if (config.isDisableCache) {
				pfile->rdbuf()->pubsetbuf(nullptr, 0);
				maybeError();
			}
			pfile->open(path);
			maybeError();
			size = fileLen(*pfile);
		}
	}

	void reopen() {
		close();
		open();
	}

	void close() {
		if (isOpen()) {
			pfile->close();
			maybeError();
			pfile.reset(nullptr);
		}
	}

	bool isOpen() {
		return !(pfile.get() == nullptr);
	}

	// For use immediately after file opening
	size_t fileLen(ifstream& file) {
		auto fpos = pfile->tellg();
		maybeError();
		pfile->seekg(0, std::ios::end);
		maybeError();
		auto curPos = pfile->tellg();
		maybeError();
		size_t res = curPos - fpos;
		pfile->seekg(fpos);
		maybeError();
		return res;
	}

	bool readBlock(Block& block, size_t blockNumber = 0) {
		auto blockSize = block.size();
		size_t readed = 0;
		size_t len = getSize();

		size_t curPos = blockSize * blockNumber;
		if (curPos >= len) {
			return false;
		}
		pfile->seekg(curPos);
		maybeError();

		pfile->read(reinterpret_cast<char*>(block.data()), blockSize);
		maybeError();
		size_t curReaded = pfile->gcount();
		maybeError();
		readed += curReaded;

		if (curReaded < blockSize) {
			block.addZeros(curReaded);
		}
		return true;
	}
public:
	static Id getNextId() {
		static Id curId = 0;
		return curId++;
	}
	static Id nextId;
	Id id;
	PEqualGroup eqGroup;

	//static void init() {
	//	nextId = 0;
	//}
	FileReaded(const string& filePath, const Config& config): config { config } {
		id = getNextId();
		path = filePath;
	}

	string getFilePath() {
		if (filePath == "") {
			filePath = FilesUtils::normalizeFilePath(path);
		}
		return filePath;
	}

	bool isEmptyGroup() {
		return eqGroup.get() == nullptr;
	}

	size_t getSize() {
		return fs::file_size(path);
	}

	void startReading() {
		logger(getFilePath(), "[START_READ]");
		open();
		curBlockNumber = 0;
		isStartedReading = true;
	}

	void maybeStartReading() {
		if (!isStartedReading) {
			startReading();
		}
	}

	bool tryGetInCache(shared_ptr<Hash>& hash) {
		if (curBlockNumber < blocksHashes.size()) {
			hash = blocksHashes[curBlockNumber];
			return true;
		}
		return false;
	}

	void finishReadingIfNotFinished() {
		if (isOpen()) {
			finishReading();
			return;
		}
		logger(getFilePath(), "[FINISHED_ALREADY]");
	}
	void finishReading() {
		close();
		curBlockNumber = 0;
		isStartedReading = false;
		logger(getFilePath(), "[FINISH_READ]");
	}

	bool readNextHash(shared_ptr<Hash>& hash) {
		logger(getFilePath(), "[NEXT_READ]");
		if (tryGetInCache(hash)) {
			logger(getFilePath(), "[FOUND_IN_CACHE]");
			++curBlockNumber;
			return true;
		}
		maybeStartReading();

		Block block{ config.blockSize };
		auto res = readBlock(block, curBlockNumber);
		if (!res) {
			logger(getFilePath(), "[ALL_READED]");
			finishReadingIfNotFinished();
			return false;
		}
		++curBlockNumber;

		hash = std::make_shared<Hash>(block, config.curHashType);
		blocksHashes.push_back(hash);
		return true;
	}
};
