#include "frame.h"

void mp3::frame::Frame::canChange() const {
	if (!getChange())
		throw std::runtime_error("Can not change this argument.");
}

void mp3::frame::TextFrame::show() const {
	std::string frameType = "Text Frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameValue = "\t\tValue: " + getValue() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameValue, frameEncoding);
	
	std::cout << '\n';
}

void mp3::frame::TextFrame::writeToFile(std::ofstream& file) {
	// write frame Name;
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), 1);
	// write frame Size
	uint32_t size = 1 + _value.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), 1);
	file.write(reinterpret_cast<char*>(&tmp), 1);
	// write encoding
	file.write(reinterpret_cast<char*>(&_encoding), 1);
	// write value
	for (auto& item: _value)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

std::pair<int*, int> mp3::frame::UFID::getBinaryData() const {
    std::pair<int*, int> data = {new int[_capacity], _capacity};

    for (int i = 0; i < _capacity; ++i)
        data.first[i] = _binaryData[i];

    return data;
}

void mp3::frame::UFID::show() const {
	std::string frameType = "Unique file identifier\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameOwnerIdentifier = "\t\tOwner Identifier: " + getOwnerIdentifier() + "\n";
	std::string frameBytes = "\t\tFrame bytes: ";
	int* binaryData = getBinaryData().first;
	for (int i = 0; i < getCapacity(); ++i)
		frameBytes += std::to_string(binaryData[i]);
	frameBytes += "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameOwnerIdentifier, frameBytes);
	
	std::cout << "\n";
}

void mp3::frame::UFID::writeToFile(std::ofstream& file) {
	// write name to file
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write frame size
	uint32_t size = _ownerIdentifier.size() + 1 + _capacity;
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write owner
	for (auto& item: _ownerIdentifier)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write binaryData
	for (int i = 0; i < _capacity; ++i)
		file.write(reinterpret_cast<char*>(&_binaryData[i]), sizeof(char));
}

void mp3::frame::TXXX::show() const {
	std::string frameType = "User defined text information frame\n";
	std::string frameName = "\tName: TXXX\n";
	std::string frameDescription = "\t\tDescription: " + getDescription() + "\n";
	std::string frameValue = "\t\tValue: " + getValue() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameDescription, frameValue, frameEncoding);
	
	std::cout << "\n";
}

void mp3::frame::TXXX::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string value = getValue();
	uint32_t size = 1 + _description.size() + 1 + value.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags;
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	auto encoding = getEncoding();
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write description;
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write value
	for (auto& item: value)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::URLFrames::show() const {
	std::string frameType = "URL frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameUrl = "\t\tURL: " + getLink() + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameUrl);
	
	std::cout << "\n";
}

void mp3::frame::URLFrames::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = _link.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write link
	for (auto& item: _link)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::WXXX::show() const {
	std::string frameType = "URL frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameUrl = "\t\tURL: " + getLink() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameDescription = "\t\tDescription: " + getDescription() + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameUrl, frameEncoding, frameDescription);
	
	std::cout << "\n";
}

void mp3::frame::WXXX::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string link = getLink();
	uint32_t size = 1 + _description.size() + 1 + link.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
//	uint8_t encoding = _encoding;
	file.write(reinterpret_cast<char*>(&_encoding), sizeof(char));
	// write description
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write link
	for (auto& item: link)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::ETCO::show() const {
	std::string frameType = "Event timing codes\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameFormat = "\t\tFormat: ";
	if (getTimeStampFormat() == 1) {
		frameFormat += "Absolute time using MPEG [MPEG] frames\n";
	} else {
		frameFormat += "Absolute time using milliseconds\n";
	}
	
	
	std::string frameEventsAndTimes;
	
	const std::vector<std::string> timeStampType = {
			"\t\tPadding: ",
			"\t\tEnd of initial silence: ",
			"\t\tIntro start: ",
			"\t\tMain Part start: ",
			"\t\tOutro start: ",
			"\t\tOutro end: ",
			"\t\tVerse start: ",
			"\t\tRefrain start: ",
			"\t\tInterlude start: ",
			"\t\tTheme start: ",
			"\t\tVariation start: ",
			"\t\tKey change: ",
			"\t\tTime change: ",
			"\t\tMomentary unwanted noise: ",
			"\t\tSustained noise: ",
			"\t\tSustained noise end: ",
			"\t\tIntro end: ",
			"\t\tMain part end: ",
			"\t\tVerse end: ",
			"\t\tRefrain end: ",
			"\t\tTheme end: ",
			"\t\tProfanity: ",
			"\t\tProfanity end: "
	};
	
	for (const auto& item: getEvents()) {
		if (item.first < 0X16) {
			frameEventsAndTimes += timeStampType[item.first];
		} else if ((item.first <= 0xDF && item.first >= 0x17) || (item.first <= 0xFC && item.first >= 0xF0)) {
			frameEventsAndTimes += "\t\tReserved for future use: ";
		} else if (item.first <= 0xEF && item.first >= 0xE0) {
			frameEventsAndTimes += "\t\tNot predefined synch 0-F: ";
		} else if (item.first == 0xFD) {
			frameEventsAndTimes += "\t\tAudio end (start of silence): ";
		} else if (item.first == 0xFE) {
			frameEventsAndTimes += "\t\tAudio file ends: ";
		} else {
			frameEventsAndTimes += "\t\tOne more byte of events follows: ";
		}
		
		frameEventsAndTimes += std::to_string(item.second) + "\n";
	}
	
	mp3::utils::printFrames(frameType, frameName, frameFormat, frameEventsAndTimes);
	
	std::cout << "\n";
}

void mp3::frame::ETCO::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size =  1 + _eventsAndTimes.size() * 5;
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write time stamp format
	file.write(reinterpret_cast<char*>(&_timeStampFormat), sizeof(char));
	// write events
	for (auto& item: _eventsAndTimes) {
		file.write(reinterpret_cast<char*>(&item.first), sizeof(char));
		realSize[0] = item.second >> 24;
		realSize[1] = item.second >> 16;
		realSize[2] = item.second >> 8;
		realSize[3] = item.second >> 0;
		file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	}
}

void mp3::frame::USLT::show() const {
	std::string frameType = "Unsynchronised lyrics/text transcription\n";
	std::string frameName = "\tFrame name: USLT\n";
	std::string frameEncoding = "\t\tFrame encoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameLanguage =  "\t\tFrame language: " + getLanguage() + "\n";
	std::string frameDescription = "\t\tFrame description: " + getDescription() + "\n";
	std::string frameValue = "\t\tFrame value: " + getValue() + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameEncoding, frameLanguage,
	                        frameDescription, frameValue
	);
	
	std::cout << '\n';
}

void mp3::frame::USLT::writeToFile(std::ofstream& file) {
	// write name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string value = getValue();
	int encoding = getEncoding();
	uint32_t size = 1 + 3 + _description.size() + 1 + value.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write language
	for (auto& item: _language)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write description
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write text
	for (auto& item: value)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::COMM::show() const {
	std::string frameType = "Comments\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameLanguage = "\t\tLanguage: " + getLanguage() + "\n";
	std::string frameDescription = "\t\tDescription: " + getDescription() + "\n";
	std::string frameValue = "\t\tValue: " + getValue() + "\n";
	
	mp3::utils::printFrames(
			frameType, frameName,
			frameEncoding, frameLanguage,
			frameDescription, frameValue
	);
	
	std::cout << '\n';
}

void mp3::frame::COMM::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string value = getValue();
	uint32_t size = 1 + 3 + _description.size() + 1 + value.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	int encoding = getEncoding();
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write language
	for (auto& item: _language)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write description
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write text
	for (auto& item: value)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::RVA2::show() const {
	std::string frameType = "Relative volume adjustment\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameIdentification = "\t\tIdentification: " + getIdentification() + "\n";
	std::string adjustmentPoints;
	
	const std::vector<std::string> typeOfChannel = {
			"Other", "Master volume",
			"Front right", "Front left",
			"Back right", "Back left",
			"Front centre", "Back centre",
			"Subwoofer"
	};
	
	for (const auto& item: getPeaks()) {
		adjustmentPoints += "\t\tType of channel: " + typeOfChannel[item->channelType()] + " ";
		adjustmentPoints += "\t\tVolume adjustment: " + std::to_string(item->volumeAdjustment()) + " ";
		adjustmentPoints += "\t\tBits per peak: " + std::to_string(item->bitsRepresentingPeak()) + " ";
		adjustmentPoints += "\t\tPeak volume: " + std::to_string(item->peakVolume()) + "\n";
	}
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameIdentification, adjustmentPoints);
	
	std::cout << '\n';
}

void mp3::frame::RVA2::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write frame size
	uint32_t size = _identification.size() + 1 + 12 * _peaks.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write identification
	for (auto& item: _identification)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated bit
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write peaks
	for (const auto& item: _peaks) {
		uint8_t channel = item->channelType();
		uint16_t volumeAdjustment = item->volumeAdjustment();
		uint8_t bitsRepresentingPeak = item->bitsRepresentingPeak();
		uint64_t peakVolume = item->peakVolume();
		// write channel
		file.write(reinterpret_cast<char*>(&channel), sizeof(char));
		// write volume adjustment
		uint8_t volumeSize[2];
		volumeSize[0] = volumeAdjustment >> 8;
		volumeSize[1] = volumeAdjustment >> 0;
		file.write(reinterpret_cast<char*>(&volumeSize), sizeof(uint16_t));
		// write bits per peak
		tmp = 64;
		file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
		// write peak volume
		uint8_t bytesVolumePeak[8];
		bytesVolumePeak[0] = peakVolume >> 56;
		bytesVolumePeak[1] = peakVolume >> 48;
		bytesVolumePeak[2] = peakVolume >> 40;
		bytesVolumePeak[3] = peakVolume >> 32;
		bytesVolumePeak[4] = peakVolume >> 24;
		bytesVolumePeak[5] = peakVolume >> 16;
		bytesVolumePeak[6] = peakVolume >> 8;
		bytesVolumePeak[7] = peakVolume >> 0;
		file.write(reinterpret_cast<char*>(&bytesVolumePeak), tmp / 8);
	}
}

void mp3::frame::EQU2::show() const {
	std::string frameType = "Equalisation\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string interpolation = "\t\tInterpolation: " + std::to_string(getInterpolationMethod()) + "\n";
	std::string identification = "\t\tIdentification: " + getIdentification() + "\n";
	std::string adjustmentsPoints;
	for (const auto& item: getAdjustmentPoints()) {
		adjustmentsPoints += "\t\tFrequency: " + std::to_string(item.first) + " ";
		adjustmentsPoints += "Volume adjustment: " + std::to_string(item.second) + "\n";
	}
	
	mp3::utils::printFrames(frameType, frameName,
	                        interpolation, identification,
	                        adjustmentsPoints);
	std::cout << '\n';
}

void mp3::frame::EQU2::writeToFile(std::ofstream& file) {
	// write name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = 1 + _identification.size() + 1 + 4 * _adjustmentPoints.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write interpolation
	file.write(reinterpret_cast<char*>(&_interpolationMethod), sizeof(char));
	// write identification
	for (auto& item: _identification)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
	// write points
	for (auto& item: _adjustmentPoints) {
		uint8_t data[2];
		data[0] = item.first >> 8;
		data[1] = item.first >> 0;
		file.write(reinterpret_cast<char*>(&data), sizeof(uint16_t));
		data[0] = item.second >> 8;
		data[1] = item.second >> 0;
		file.write(reinterpret_cast<char*>(&data), sizeof(uint16_t));
	}
}

void mp3::frame::PCNT::show() const {
	std::string frameType = "Play counter\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameCnt = "\t\tPlay counter: " + std::to_string(getCnt()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameCnt);
	
	std::cout << '\n';
}

void mp3::frame::PCNT::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint8_t size = 8;
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&size), sizeof(char));
	// write flags
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write counter
	uint8_t realCNT[8];
	realCNT[0] = _cnt >> 56;
	realCNT[1] = _cnt >> 48;
	realCNT[2] = _cnt >> 40;
	realCNT[3] = _cnt >> 32;
	realCNT[4] = _cnt >> 24;
	realCNT[5] = _cnt >> 16;
	realCNT[6] = _cnt >> 8;
	realCNT[7] = _cnt >> 0;
	file.write(reinterpret_cast<char*>(&realCNT), sizeof(uint64_t));
}

void mp3::frame::POMP::show() const {
	std::string frameType = "Popularimeter\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameEmail = "\t\tEmail: " + getEmail() + "\n";
	std::string frameRating = "\t\tRating: " + std::to_string(getRating()) + "\n";
	std::string frameCnt = "\t\tCounter: " + std::to_string(getCnt()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameEmail, frameRating,
	                        frameCnt);
	
	std::cout << '\n';
}

void mp3::frame::POMP::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = _email.size() + 2 + 8;
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write email
	for (auto& item: _email)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write rating
	file.write(reinterpret_cast<char*>(&_rating), sizeof(char));
	// write counter
	uint8_t data[8];
	data[0] = _cnt >> 56;
	data[1] = _cnt >> 48;
	data[2] = _cnt >> 40;
	data[3] = _cnt >> 32;
	data[4] = _cnt >> 24;
	data[5] = _cnt >> 16;
	data[6] = _cnt >> 8;
	data[7] = _cnt >> 0;
	file.write(reinterpret_cast<char*>(&data), sizeof(uint64_t));
}

void mp3::frame::RBUF::show() const {
	std::string frameType = "Recommended buffer size\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameSize = "\t\tBuffer size: " + std::to_string(getSize()) + "\n";
	std::string frameFlag = "\t\tFlag: ";
	
	if (getFlag())
		frameFlag += "True";
	else
		frameFlag += "False";
	
	frameFlag += "\n";
	
	std::string frameOffset = "\t\tOffset: " + std::to_string(getOffset()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameSize, frameFlag,
	                        frameOffset);
	
	std::cout << '\n';
}

void mp3::frame::RBUF::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write frame size
	uint8_t realSize[4];
	uint32_t size = 8;
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write buffer size
	uint8_t buffsize[3];
	buffsize[0] = _bufferSize >> 16;
	buffsize[1] = _bufferSize >> 8;
	buffsize[2] = _bufferSize >> 0;
	file.write(reinterpret_cast<char*>(&buffsize), sizeof(buffsize));
	// write flag
	file.write(reinterpret_cast<char*>(&_flag), sizeof(char));
	// write next tag offset
	realSize[0] = (uint32_t)_nextTagOffset >> 24;
	realSize[1] = (uint32_t)_nextTagOffset >> 16;
	realSize[2] = (uint32_t)_nextTagOffset >> 8;
	realSize[3] = (uint32_t)_nextTagOffset >> 0;
	uint8_t offset = _nextTagOffset;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(int32_t));
}

void mp3::frame::LINK::show() const {
	std::string frameType = "Linked information\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameURL = "\t\tURL: " + getUrl() + "\n";
	std::string frameId = "\t\tId: " + getId() + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameURL, frameId);
	
	std::cout << '\n';
}

void mp3::frame::LINK::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = 4 + _url.size() + 1 + _id.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flag
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write frame identifier
	for (auto& item: _frameIdentifier)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write url
	for (auto& item: _url)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write id
	for (auto& item: _id)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::POSS::show() const {
	std::string frameType = "Position synchronisation frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameFormat = "\t\tFormat: " + std::to_string(getTimeStampFormat()) + "\n";
	std::string framePosition = "\t\tPosition: " + std::to_string(getPosition()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameFormat, framePosition);
	
	std::cout << '\n';
}

void mp3::frame::POSS::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = 5;
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write time stamp format
	file.write(reinterpret_cast<char*>(&_timeStampFormat), sizeof(char));
	// write position
	realSize[0] = _position >> 24;
	realSize[1] = _position >> 16;
	realSize[2] = _position >> 8;
	realSize[3] = _position >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
}

void mp3::frame::USER::show() const {
	std::string frameType = "Terms of use frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameLanguage = "\t\tLanguage: " + getLanguage() + "\n";
	std::string frameValue = "\t\tValue: " + getValue() + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameEncoding, frameLanguage, frameValue);
	
	std::cout << '\n';
}

void mp3::frame::USER::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string value = getValue();
	uint32_t size = 1 + 3 + value.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	uint8_t encoding = getEncoding();
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write language
	for (auto& item: _language)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write text
	for (auto& item: value)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::OWNE::show() const {
	std::string frameType = "Ownership frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string framePrice = "\t\tPrice: " + getPrice() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameDate = "\t\tDate: " + getDate() + "\n";
	std::string frameSeller = "\t\tSeller: " + getSeller() + "\n";
	
	mp3::utils::printFrames(frameType, frameName,
	                        frameEncoding, framePrice,
	                        frameDate, frameSeller);
	
	std::cout << '\n';
}

void mp3::frame::OWNE::writeToFile(std::ofstream& file) {
	// write name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = 1 + _price.size() + 1 + 8 + _seller.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write text encoding
	file.write(reinterpret_cast<char*>(&_textEncoding), sizeof(char));
	// write price
	for (auto& item: _price)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write  date
	for (auto& item: _date)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write seller
	for (auto& item: _seller)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::COMR::show() const {
	std::string frameType = "Commercial frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string framePrice = "\t\tPrice: " + getPrice() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameDate = "\t\tDate: " + getDate() + "\n";
	std::string frameSeller = "\t\tSeller: " + getSeller() + "\n";
	std::string frameReceived = "\t\tReceived: " + std::to_string(getReceived()) + "\n";
	std::string frameDescription = "\t\tDescription: " + getDescription() + "\n";
	std::string framePicture = "\t\tPicture type: " + getPictureType() + "\n";
	std::string frameContact = "\t\tContact: " + getContact() + "\n";
	
	
	mp3::utils::printFrames(frameType, frameName,
							framePrice, frameEncoding,
							frameDate, frameSeller,
							frameReceived, frameDescription,
							frameContact);
	
	std::cout << '\n';
}

void mp3::frame::COMR::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	std::string price = getPrice();
	std::string date = getDate();
	std::string seller = getSeller();
	uint32_t size = 1 + price.size() + 1 + date.size() + _contactURL.size() + 1 + 1;
	size += seller.size() + 1 + _description.size() + 1 + _pictureType.size() + 1 + _bytes.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	uint8_t encoding = getEncoding();
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write price
	for (auto& item: price)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write valid until
	for (auto& item: date)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write contact url
	for (auto& item: _contactURL)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// received as
	file.write(reinterpret_cast<char*>(&_receivedAs), sizeof(char));
	// write seller name
	for (auto& item: seller)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write description
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byre
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write picture
	for (auto& item: _pictureType)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write seller logo
	for (auto& item: _bytes)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::ENCR::show() const {
	std::string frameType = "Encryption method registration\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameOwner = "\t\tOwner: " + getOwnerIdentifier() + "\n";
	std::string frameMethodSymbol = "\t\tMethod: " + std::to_string(getMethodSymbol()) + "\n";
	std::string frameBinaryData = "\t\tData: ";
	
	for (const auto& item: getBinaryData()) {
		frameBinaryData += std::to_string(item);
	}
	
	frameBinaryData += "\n";
	
	mp3::utils::printFrames(frameType, frameName,
							frameOwner, frameMethodSymbol,
							frameBinaryData);
	
	std::cout << '\n';
}

void mp3::frame::ENCR::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = _ownerIdentifier.size() + 1 + 1 + _binaryData.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write owner identifier
	for (auto& item: _ownerIdentifier)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write method
	file.write(reinterpret_cast<char*>(&_methodSymbol), sizeof(char));
	// write data
	for (auto& item: _binaryData)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::GRID::show() const {
	std::string frameType = "Group identification registration\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameOwner = "\t\tOwner identifier: " + getOwnerIdentifier() + "\n";
	std::string frameGroupSymbol = "\t\tGroup symbol: " + std::to_string(getGroupSymbol()) + "\n";
	std::string frameBinaryData = "\t\tBinary data: ";
	
	for (const auto& item: getData()) {
		frameBinaryData += std::to_string(item);
	}
	
	frameBinaryData += '\n';
	
	mp3::utils::printFrames(frameType, frameName,
							frameOwner, frameGroupSymbol,
							frameBinaryData);
	
	std::cout << '\n';
}

void mp3::frame::GRID::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = _ownerIdentifier.size() + 1 + 1 + _binaryData.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write identifier
	for (auto& item: _ownerIdentifier)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write group symbol
	file.write(reinterpret_cast<char*>(&_groupSymbol), sizeof(char));
	// write binary data
	for (auto& item: _binaryData)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::PRIV::show() const  {
	std::string frameType = "Private frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameOwner = "\t\tOwner: " + getOwnerIdentifier() + "\n";
	std::string frameData = "\t\tData: ";
	
	for (const auto& item: getBinaryData()) {
		frameData += std::to_string(item);
	}
	
	frameData += '\n';
	
	mp3::utils::printFrames(frameType, frameName,
							frameOwner, frameData);
	
	std::cout << '\n';
}

void mp3::frame::PRIV::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = _ownerIdentifier.size() + 1 + _binaryData.size();
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write owner
	for (auto& item: _ownerIdentifier)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separated byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write private data
	for (auto& item: _binaryData)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
}

void mp3::frame::SEEK::show() const {
	std::string frameType = "Seek frame\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameOffset = "\t\tOffset: " + std::to_string(getOffset()) + "\n";
	
	mp3::utils::printFrames(frameType, frameName, frameOffset);
	
	std::cout << '\n';
}

void mp3::frame::SEEK::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write size
	uint32_t size = 4;
	file.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write offset
	uint8_t byteOffset[4];
	byteOffset[0] = _offset >> 24;
	byteOffset[1] = _offset >> 16;
	byteOffset[2] = _offset >> 8;
	byteOffset[3] = _offset >> 0;
	file.write(reinterpret_cast<char*>(&byteOffset), sizeof(uint32_t));
}

void mp3::frame::SYLT::show() const {
	std::string frameType = "Synchronised lyrics/text\n";
	std::string frameName = "\tName: " + getName() + "\n";
	std::string frameEncoding = "\t\tEncoding: " + mp3::utils::encodingName(getEncoding()) + "\n";
	std::string frameValue = "\t\tValue: " + getValue() + "\n";
	std::string frameLanguage = "\t\tLanguage: " + getLanguage() + "\n";
	std::string frameTimeStamp = "\t\tTime stamp: " + std::to_string(getTimeStamp()) + "\n";
	std::string frameContentType = "\t\tContent type: " + std::to_string(getContentType()) + "\n";
	std::string frameDescription = "\t\tDescription: " + getDescription() + "\n";
	std::string frameText = "\t\tText: \n";
	
	for (const auto& item: getSyncText()) {
		frameText += "\t\t\tIdentifier: " + item.first + " Time: " + std::to_string(item.second) + "\n";
	}
	
	mp3::utils::printFrames(frameType, frameName,
							frameEncoding, frameValue,
							frameLanguage, frameTimeStamp,
							frameContentType, frameDescription,
							frameText);
	
	std::cout << '\n';
}

void mp3::frame::SYLT::writeToFile(std::ofstream& file) {
	// write frame name
	for (auto& item: _frameName)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write frame size
	uint32_t size = 1 + 3 + 1 + 1 + _description.size() + 1;
	for (auto& item: _syncText) {
		size += item.first.size() + 1 + 4;
	}
	uint8_t realSize[4];
	realSize[0] = size >> 24;
	realSize[1] = size >> 16;
	realSize[2] = size >> 8;
	realSize[3] = size >> 0;
	file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	// write flags
	uint8_t tmp = 0;
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write encoding
	uint8_t encoding = getEncoding();
	file.write(reinterpret_cast<char*>(&encoding), sizeof(char));
	// write language
	for (auto& item: _language)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write time stamp format
	file.write(reinterpret_cast<char*>(&_timeStampFormat), sizeof(char));
	// write content type
	file.write(reinterpret_cast<char*>(&_contentType), sizeof(char));
	// write description
	for (auto& item: _description)
		file.write(reinterpret_cast<char*>(&item), sizeof(char));
	// write separate byte
	file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
	// write other info
	for (auto& item: _syncText) {
		// write sync identifier
		for (auto& symbol: item.first)
			file.write(reinterpret_cast<char*>(&symbol), sizeof(char));
		// write separate byte
		file.write(reinterpret_cast<char*>(&tmp), sizeof(char));
		// write time stamp
		realSize[0] = item.second >> 24;
		realSize[1] = item.second >> 16;
		realSize[2] = item.second >> 8;
		realSize[3] = item.second >> 0;
		file.write(reinterpret_cast<char*>(&realSize), sizeof(uint32_t));
	}
}

void mp3::frame::UFID::setBinaryData(const int binaryData[64]) {
	canChange();
	
	for (int i = 0; i < 64; ++i) {
		_binaryData[i] = binaryData[i];
	}
}

void mp3::frame::UFID::addToBinaryData(int data) {
	canChange();

	if (_capacity + 1 >= 64)
		throw std::runtime_error("Too many elements in binary data can not add more");
	
	_binaryData[_capacity] = data;
	++_capacity;
}