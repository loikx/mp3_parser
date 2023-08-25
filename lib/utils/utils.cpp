#include "utils.h"

std::string mp3::utils::encodingName(int encoding) {
	switch (encoding) {
		case 0:
			return "ISO-8859-1";
		case 1:
			return "UTF-16";
		case 2:
			return "UTF-16BE";
		case 3:
			return "UTF-8";
		default:
			throw std::invalid_argument("Undefined encoding type");
	}
}

void mp3::utils::FrameHeader::parse(std::ifstream& file) {
	// Reading frame size
	uint8_t a = 0, b = 0, c = 0, d = 0;
	file.read(reinterpret_cast<char*>(&a), sizeof(a));
	file.read(reinterpret_cast<char*>(&b), sizeof(b));
	file.read(reinterpret_cast<char*>(&c), sizeof(c));
	file.read(reinterpret_cast<char*>(&d), sizeof(d));
	
	size = d | (c << 8) | (b << 16) | (a << 24);

	
	uint8_t firstFlags = 0, secondFlags = 0;
	// 2 bytes for flags
	file.read(reinterpret_cast<char*>(&firstFlags), sizeof(firstFlags));
	file.read(reinterpret_cast<char*>(&secondFlags), sizeof(secondFlags));
	
	auto flags = std::bitset<8>(firstFlags);
	tagAlterPreservation = (flags[4] == 1);
	fileAlterPreservation = (flags[5] == 1);
	readOnly = (flags[6] == 1);
	
	flags = std::bitset<8>(secondFlags);
	groupIdentity = (flags[0] == 1);
	compression = (flags[1] == 1);
	encryption = (flags[2] == 1);
	unSynchronisation = (flags[3] == 1);
	dataLength = (flags[6] == 1);
}

std::vector<uint8_t> mp3::utils::removeUnSyncBytes(const std::vector<uint8_t>& binaryData) {
	std::vector<uint8_t> ans;
	int readI = 0;
	while (readI < (int)binaryData.size() - 1) {
		ans.push_back(binaryData[readI]);
		
		readI += (binaryData[readI] == 0xFF && binaryData[readI + 1] == 0) ? 2 : 1;
	}
	
	if (readI < (int)binaryData.size()) {
		ans.push_back(binaryData[readI]);
	}
	
	return std::move(ans);
}

mp3::utils::FrameHeader* mp3::utils::parseFrameHeader(std::ifstream& file) {
	auto headerParser = new mp3::utils::FrameHeader();
	headerParser->parse(file);
	
	if (headerParser->tagAlterPreservation || headerParser->fileAlterPreservation) {
		for (int i = 0; i < headerParser->size; ++i)
			file.get();
		
		return nullptr;
	}
	
	return headerParser;
}

std::vector<uint8_t> mp3::utils::readFrameData(int size, std::ifstream& file) {
	std::vector<uint8_t> frameData;
	
	for (int i = 0; i < size; ++i) {
		frameData.push_back((uint8_t)file.get());
	}
	
	return std::move(frameData);
}