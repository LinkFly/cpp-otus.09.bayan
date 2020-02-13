#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include "share.h"
#include "share-types.h"
#include "Config.h"
#include "EqualGroup.h"
#include "Block.h"
#include "Hash.h"
#include "Directory.h"

namespace fs = boost::filesystem;

using std::string;
using std::ifstream;
using std::vector;
using std::shared_ptr;

// TODO!!! Handling errors
class FileReaded {
	string path{};
	std::unique_ptr<ifstream> pfile;
	size_t size = 0;
	size_t curBlockNumber = 0;
	bool isStartedReading = false;
	vector<shared_ptr<Hash>> blocksHashes;
	string filePath = "";

	void open() {
		if (!isOpen()) {
			pfile.reset(new ifstream{});
			if (isDisableCache) {
				pfile->rdbuf()->pubsetbuf(nullptr, 0);
			}
			pfile->open(path);
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
			pfile.reset(nullptr);
		}
	}

	bool isOpen() {
		return !(pfile.get() == nullptr);
	}

	// For use immediately after file opening
	size_t fileLen(ifstream& file) {
		auto fpos = pfile->tellg();
		pfile->seekg(0, std::ios::end);
		size_t res = pfile->tellg() - fpos;
		pfile->seekg(fpos);
		return res;
	}

	bool readBlock(Block& block, size_t blockNumber = 0) {
		/*auto blockSize = Config::getInstance().blockSize;*/
		auto blockSize = block.size();

		/*std::unique_ptr<char> buf{ new char[blockSize + 1] };*/
		/*std::unique_ptr<char> buf{ new char[blockSize] };*/
		/*buf.get()[blockSize] = 0;*/
		size_t readed = 0;
		size_t len = getSize();

		size_t curPos = blockSize * blockNumber;
		if (curPos >= len) {
			return false;
		}
		pfile->seekg(curPos);

		// TODO!!! Handling errors
		/*pfile->read(buf.get(), blockSize);*/
		pfile->read(reinterpret_cast<char*>(block.ptr()), blockSize);
		size_t curReaded = pfile->gcount();
		readed += curReaded;

		if (curReaded < blockSize) {
			block.addZeros(curReaded);
		}
		
		// to block
		//for (size_t i = 0; i < blockSize; ++i) {
		//	/*block.set(i, buf.get()[i]);*/
		//	block.set(i, buf.get()[i]);
		//}
		return true;
	}
public:
	static inline Id nextId = 0;
	Id id;
	bool isDisableCache = Config::isDefaultDisableCache;
	PEqualGroup eqGroup;

	// debugging
	int _count = 0;
	int _max = 3;

	FileReaded() {
		id = nextId++;
	}

	FileReaded(const string& filePath): FileReaded() {
		path = filePath;
	}

	~FileReaded() {
		/*cout << "===================== ~FileReaded id: " << (int)this->id << "=======================\n";
		cout << "eqGroup.useCount(): " << eqGroup.use_count() << endl;*/
	}

	string getFilePath() {
		if (filePath == "") {
			filePath = Directory::normalizeFilePath(path);
		}
		return filePath;
	}

	bool isEmptyGroup() {
		return eqGroup.get() == nullptr;
	}

	size_t getSize() {
		bool wasClose = !isOpen();
		if (!wasClose) {
			return size;
		}
		// TODO commented - not obvious saved to size, refactor it
		open();
		if (wasClose)
			close();
		return size;
	}

	void startReading() {
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

	void finishReading() {
		close();
		curBlockNumber = 0;
		isStartedReading = false;
	}

	bool readNextHash(shared_ptr<Hash>& hash) {
		
		if (tryGetInCache(hash)) {
			++curBlockNumber;
			return true;
		}
		maybeStartReading();

		auto blockSize = Config::blockSize;
		Block block{ blockSize };

		//shared_ptr<Block> block = std::make_shared<Block>(blockSize);
		/*auto block = new Block(5);
		delete block;*/
		auto res = readBlock(block, curBlockNumber);
		//bool res = false;
		if (!res) {
			finishReading();
			return false;
		}
		++curBlockNumber;

		//return false;
		/*hash.swap(std::make_shared<Hash>());*/
		hash = std::make_shared<Hash>(block);
		/*blocksHashes.push_back(hash);*/
		return true;
	}
};