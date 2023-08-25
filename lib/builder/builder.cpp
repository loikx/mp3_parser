#include "builder.h"

void mp3::builders::FrameBuilder::setName(const std::string& name) {
    _frameName = name;
}

void mp3::builders::FrameBuilder::setChange(bool change) {
	_change = change;
}

void mp3::builders::TextFrameBuilder::setEncoding(int encoding) {
    _encoding = encoding;
}

void mp3::builders::TextFrameBuilder::setValue(const std::string& value) {
    _value = value;
}

mp3::frame::Frame* mp3::builders::TextFrameBuilder::generate() const {
	auto item = new mp3::frame::TextFrame(_frameName, _value, _encoding, _change);
    return item;
}

mp3::frame::Frame* mp3::builders::TextFrameBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync) {
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	}
	
	setEncoding(frameData[0]);
	
	std::string value;
	for (int i = 1; i < (int)frameData.size(); ++i)
		value += (char)frameData[i];
	
	setValue(value);
	
	return generate();
}

void mp3::builders::UFIDBuilder::setCapacity(int capacity) {
    _capacity = capacity;
}

void mp3::builders::UFIDBuilder::setBinaryData(const int data[64]) {
    for (int i = 0; i < _capacity; ++i) {
        _binaryData[i] = data[i];
    }
}

void mp3::builders::UFIDBuilder::setOwnerIdentifier(const std::string& ownerIdentifier) {
    _ownerIdentifier = ownerIdentifier;
}

mp3::frame::Frame* mp3::builders::UFIDBuilder::generate() const {
    return new mp3::frame::UFID(_ownerIdentifier, _binaryData, _capacity, _change);
}

mp3::frame::Frame* mp3::builders::UFIDBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	std::string owner;
	int i = 0;
	while (frameData[i] != 0) {
		owner += (char)frameData[i];
		++i;
	}
	
	++i;
	int j = 0;
	int binaryData[64];
	while (i < (int)frameData.size()) {
		binaryData[j] = frameData[i];
		++i;
		++j;
	}
	
	setOwnerIdentifier(owner);
	setCapacity(j);
	setBinaryData(binaryData);
	
	return generate();
}

void mp3::builders::TXXXBuilder::setDescription(const std::string& description) {
    _description = description;
}

mp3::frame::Frame* mp3::builders::TXXXBuilder::generate() const {
    return new frame::TXXX(_value, _encoding, _description, _change);
}

mp3::frame::Frame* mp3::builders::TXXXBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	std::string description;
	int i = 1;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	
	std::string value;
	++i;
	while (i < (int)frameData.size()) {
		value += (char)frameData[i];
		++i;
	}
	
	setDescription(description);
	setValue(value);
	
	return generate();
}

void mp3::builders::URLFrameBuilder::setLink(const std::string& link) {
    _link = link;
}

mp3::frame::Frame* mp3::builders::URLFrameBuilder::generate() const {
    return new mp3::frame::URLFrames(_frameName, _link, _change);
}

mp3::frame::Frame* mp3::builders::URLFrameBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	std::string link;
	for (const auto& item: frameData) {
		link += (char)item;
	}
	
	setLink(link);
	
	return generate();
}

void mp3::builders::WXXXBuilder::setDescription(const std::string& description) {
    _description = description;
}

void mp3::builders::WXXXBuilder::setEncoding(int encoding) {
	_encoding = encoding;
}

mp3::frame::Frame* mp3::builders::WXXXBuilder::generate() const {
    return new frame::WXXX(_frameName, _encoding, _link, _description, _change);
}

mp3::frame::Frame* mp3::builders::WXXXBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	int i = 1;
	std::string description;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	
	++i;
	std::string link;
	while (i < (int)frameData.size()) {
		link += (char)frameData[i];
		++i;
	}
	
	setDescription(description);
	setLink(link);
	
	return generate();
}

void mp3::builders::ETCOBuilder::setFormat(int format) {
    _timeStampFormat = format;
}

void mp3::builders::ETCOBuilder::clear() {
    _eventsAndTimes.clear();
    _eventsAndTimes = {};
}

void mp3::builders::ETCOBuilder::setEvents(const std::vector<std::pair<int, int>>& events) {
    _eventsAndTimes = events;
}

mp3::frame::Frame* mp3::builders::ETCOBuilder::generate() const {
    return new mp3::frame::ETCO(_frameName, _timeStampFormat, _eventsAndTimes, _change);
}

mp3::frame::Frame* mp3::builders::ETCOBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setFormat(frameData[0]);
	
	int i = 1;
	std::vector<std::pair<int, int>> events;
	while (i < (int)frameData.size()) {
		std::pair<int, int> timeStamp;
		timeStamp.first = frameData[i];
		timeStamp.second = frameData[i + 4] | (frameData[i + 3] << 8)
				| (frameData[i + 2] << 16) | (frameData[i + 1] << 24);
		events.push_back(timeStamp);
		i += 5;
	}
	
	setEvents(events);
	
	return generate();
}

void mp3::builders::USLTBuilder::setDescription(const std::string& description) {
    _description = description;
}

void mp3::builders::USLTBuilder::setLanguage(const std::string& language) {
    _language = language;
}

mp3::frame::Frame* mp3::builders::USLTBuilder::generate() const {
    return new mp3::frame::USLT(_frameName, _encoding, _value, _language, _description, _change);
}

mp3::frame::Frame* mp3::builders::USLTBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	setEncoding(frameData[0]);
	
	std::string lang = std::string(1, (char)frameData[1])
			+ std::string(1, (char)frameData[2]) + std::string(1, (char)frameData[3]);
	setLanguage(lang);
	
	int i = 4;
	std::string description;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	
	++i;
	std::string text;
	while (i < (int)frameData.size()) {
		text += (char)frameData[i];
		++i;
	}
	
	setDescription(description);
	setValue(text);
	
	return generate();
}

void mp3::builders::COMMBuilder::setLanguage(const std::string& language) {
    _language = language;
}

void mp3::builders::COMMBuilder::setDescription(const std::string& description) {
    _description = description;
}

mp3::frame::Frame* mp3::builders::COMMBuilder::generate() const {
    return  new mp3::frame::COMM(_frameName, _encoding, _language, _description, _value, _change);
}

mp3::frame::Frame* mp3::builders::COMMBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	std::string lang = std::string(1, (char)frameData[1])
			+ std::string(1, (char)frameData[2]) + std::string(1, (char)frameData[3]);
	setLanguage(lang);
	
	int i = 4;
	std::string description;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	
	setDescription(description);
	++i;
	
	std::string value;
	while (i < (int)frameData.size()) {
		value += (char)frameData[i];
		++i;
	}
	
	setValue(value);
	
	return generate();
}

void mp3::builders::RVA2Builder::setIdentification(const std::string& identification) {
    _identification = identification;
}

void mp3::builders::RVA2Builder::addPeak(mp3::utils::Peak* peak) {
    _peaks.push_back(peak);
}

mp3::frame::Frame* mp3::builders::RVA2Builder::generate() const {
    return new mp3::frame::RVA2("RVA2", _identification, _peaks, _change);
}

void mp3::builders::RVA2Builder::clear() {
    for (auto& _peak: _peaks) {
        delete _peak;
    }
}

mp3::frame::Frame* mp3::builders::RVA2Builder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	int i = 0;
	std::string identification;
	while (frameData[i] != 0) {
		identification += (char)frameData[i];
		++i;
	}
	
	setIdentification(identification);
	
	++i;
	while (i < (int)frameData.size()) {
		uint8_t channelType = frameData[i];
		uint16_t volumeAdjustment = frameData[i + 2] | (frameData[i + 1] << 8);
		uint8_t bitsRepresentingPeak = frameData[i + 3];
		
		// calculate how many bytes we need on this peak volume
		int peakVolumeSize;
		if (bitsRepresentingPeak % 8 == 0) {
			peakVolumeSize = bitsRepresentingPeak / 8;
		} else {
			peakVolumeSize = bitsRepresentingPeak / 8 + 1;
		}
		int bytesCount = peakVolumeSize * 8;
		uint64_t peak = frameData[i + 4 + peakVolumeSize - 1] << bytesCount;
		for (int j = i + 4 + peakVolumeSize - 2; j >= i + 4; --j) {
			bytesCount -= 8;
			peak = peak | (frameData[j] << bytesCount);
		}
		
		auto newPeak = new mp3::utils::Peak(channelType, volumeAdjustment,
											bitsRepresentingPeak, peak);
		
		i += 4 + peakVolumeSize;
		addPeak(newPeak);
	}
	
	return generate();
}

void mp3::builders::EQU2Builder::setInterpolation(int interpolation) {
    _interpolation = interpolation;
}

void mp3::builders::EQU2Builder::setIdentification(const std::string& identification) {
    _identification = identification;
}

void mp3::builders::EQU2Builder::addAdjustment(const std::pair<int, int>& adjustmentPoint) {
    _adjustmentPoints.push_back(adjustmentPoint);
}

mp3::frame::Frame* mp3::builders::EQU2Builder::generate() const {
    return new mp3::frame::EQU2(_frameName, _interpolation,
                                _identification, _adjustmentPoints, _change);
}

void mp3::builders::EQU2Builder::clear() {
    _adjustmentPoints.clear();
    _adjustmentPoints = {};
}

mp3::frame::Frame* mp3::builders::EQU2Builder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setInterpolation(frameData[0]);
	
	int i = 1;
	std::string identification;
	while (frameData[i] != 0) {
		identification += (char)frameData[i];
		++i;
	}
	
	setIdentification(identification);
	++i;
	
	int frequency;
	int volumeAdjustment;
	while (i < (int)frameData.size()) {
		frequency = frameData[i + 1] | (frameData[i] << 8);
		volumeAdjustment = frameData[i + 3] | (frameData[i + 2] << 8);
		std::pair<int, int> tmp;
		tmp.first = frequency;
		tmp.second = volumeAdjustment;
		addAdjustment(tmp);
		i += 4;
	}
	
	return generate();
}

void mp3::builders::PCNTBuilder::setCnt(uint64_t cnt) {
    _cnt = cnt;
}

mp3::frame::Frame* mp3::builders::PCNTBuilder::generate() const {
    auto* frame = new mp3::frame::PCNT(_frameName, _cnt, _change);
    return frame;
}

mp3::frame::Frame* mp3::builders::PCNTBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	int byteCnt = (header->size - 1) * 8;
	uint64_t cnt = frameData[0] << byteCnt;
	for (int i = 1; i < (int)frameData.size(); ++i) {
		byteCnt -= 8;
		cnt = cnt | (frameData[i] << byteCnt);
	}
	
	setCnt(cnt);
	
	return generate();
}

void mp3::builders::POMPBuilder::setEmail(const std::string& email) {
    _email = email;
}

void mp3::builders::POMPBuilder::setRating(uint8_t rating) {
    _rating = rating;
}

void mp3::builders::POMPBuilder::setCnt(uint64_t cnt) {
    _cnt = cnt;
}

mp3::frame::Frame* mp3::builders::POMPBuilder::generate() const {
    return new mp3::frame::POMP(_frameName, _email,
                                _rating, _cnt, _change);
}

mp3::frame::Frame* mp3::builders::POMPBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	int i = 0;
	std::string email;
	while (frameData[i] != 0) {
		email += (char)frameData[i];
		++i;
	}
	
	setEmail(email);
	++i;
	setRating(frameData[i]);
	
	++i;
	int byteCnt = 56;
	uint64_t counter = frameData[i] << byteCnt;
	++i;
	while (i < (int)frameData.size()) {
		byteCnt -= 8;
		counter = counter | (frameData[i] << byteCnt);
		++i;
	}
	setCnt(counter);
	
	return generate();
}

void mp3::builders::RBUFBuilder::setFlag(bool flag) {
    _flag = flag;
}

void mp3::builders::RBUFBuilder::setBufferSize(uint32_t size) {
    _bufferSize = size;
}

void mp3::builders::RBUFBuilder::setOffset(uint64_t offset) {
    _nextTagOffset = offset;
}

mp3::frame::Frame* mp3::builders::RBUFBuilder::generate() const {
    return new mp3::frame::RBUF(_frameName, _bufferSize, _flag, _nextTagOffset, _change);
}

mp3::frame::Frame* mp3::builders::RBUFBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	uint32_t bufferSize = frameData[2] | (frameData[1] << 8) | (frameData[0] << 16);
	int flag = (frameData[3] == 1);
	
	setBufferSize(bufferSize);
	setFlag(flag);
	
	uint64_t offset = frameData[7] | (frameData[6] << 8) | (frameData[5] << 16) | (frameData[4] << 24);
	
	setOffset(offset);
	
	return generate();
}

void mp3::builders::LINKBuilder::setIdentifier(const std::string& identifier) {
    _frameIdentifier = identifier;
}

void mp3::builders::LINKBuilder::setId(const std::string& id) {
    _id = id;
}

void mp3::builders::LINKBuilder::setURL(const std::string& url) {
    _url = url;
}

mp3::frame::Frame* mp3::builders::LINKBuilder::generate() const {
    return new mp3::frame::LINK(_frameName, _frameIdentifier, _url, _id, _change);
}

mp3::frame::Frame* mp3::builders::LINKBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	std::string identifier = std::string(1, (char)frameData[0]) + std::string(1, (char)frameData[1]) +
			std::string(1, (char)frameData[2]) + std::string(1, (char)frameData[3]);
	setIdentifier(identifier);
	
	int i = 4;
	std::string url;
	while (frameData[i] != 0) {
		url += (char)frameData[i];
		++i;
	}
	
	setURL(url);
	
	++i;
	std::string id;
	while (i < (int)frameData.size()) {
		id += (char)frameData[i];
		++i;
	}
	
	setId(id);
	
	return generate();
}

void mp3::builders::POSSBuilder::setFormat(int format) {
	_timeStampFormat = format;
}

void mp3::builders::POSSBuilder::setPosition(uint64_t position) {
	_position = position;
}

mp3::frame::Frame* mp3::builders::POSSBuilder::generate() const {
	return new mp3::frame::POSS(_frameName, _timeStampFormat, _position, _change);
}

mp3::frame::Frame* mp3::builders::POSSBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setFormat(frameData[0]);
	
	int cnt = (header->size - 2) * 8;
	uint64_t position = frameData[1] << cnt;
	int i = 2;
	while (i < (int)frameData.size()) {
		cnt -= 8;
		position = position | (frameData[i] << cnt);
		++i;
	}
	
	setPosition(position);
	
	return generate();
}

void mp3::builders::USERBuilder::setLanguage(const std::string& lang) {
	_language = lang;
}

mp3::frame::Frame* mp3::builders::USERBuilder::generate() const {
	return new mp3::frame::USER(_frameName, _encoding, _language, _value, _change);
}

mp3::frame::Frame* mp3::builders::USERBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	std::string lang = std::string(1, (char)frameData[1]) + std::string(1, (char)frameData[2]) +
			std::string(1, (char)frameData[3]);
	setLanguage(lang);
	
	std::string value;
	int i = 4;
	while (i < (int)frameData.size()) {
		value += (char)frameData[i];
		++i;
	}
	setValue(value);
	
	return generate();
}

void mp3::builders::OWNEBuilder::setEncoding(int encoding) {
	_textEncoding = encoding;
}

void mp3::builders::OWNEBuilder::setPrice(const std::string& price) {
	_price = price;
}

void mp3::builders::OWNEBuilder::setDate(const std::string& date) {
	_date = date;
}

void mp3::builders::OWNEBuilder::setSeller(const std::string& seller) {
	_seller = seller;
}

mp3::frame::Frame* mp3::builders::OWNEBuilder::generate() const {
	return new mp3::frame::OWNE(_frameName, _textEncoding,
								_price, _date, _seller, _change);
}

mp3::frame::Frame* mp3::builders::OWNEBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	int i = 1;
	std::string price;
	while (frameData[i] != 0) {
		price += (char)frameData[i];
		++i;
	}
	setPrice(price);
	++i;
	
	std::string date;
	for (int j = 0; j < 8; ++j) {
		date += (char)frameData[i + j];
	}
	setDate(date);
	
	i += 8;
	std::string seller;
	while (i < (int)frameData.size()) {
		seller += (char)frameData[i];
		++i;
	}
	setSeller(seller);
	
	return generate();
}

void mp3::builders::SYLTBuilder::setLanguage(const std::string& language) {
	_language = language;
}

void mp3::builders::SYLTBuilder::setDescription(const std::string& description) {
	_description = description;
}

void mp3::builders::SYLTBuilder::setContentType(uint8_t content) {
	_contentType = content;
}

void mp3::builders::SYLTBuilder::setTimeStampFormat(uint8_t timeFormat) {
	_timeStampFormat = timeFormat;
}

mp3::frame::Frame* mp3::builders::SYLTBuilder::generate() const {
	return new mp3::frame::SYLT(_frameName, _encoding,
								_value, _language,
								_timeStampFormat, _contentType,
								_description, _syncText, _change);
}

void mp3::builders::SYLTBuilder::setSyncText(const std::vector<std::pair<std::string, int>>& syncText) {
	_syncText = syncText;
}

mp3::frame::Frame* mp3::builders::SYLTBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	std::string lang = std::string(1, (char)frameData[1])
			+ std::string(1, (char)frameData[2]) + std::string(1, (char)frameData[3]);
	setLanguage(lang);
	
	setTimeStampFormat(frameData[4]);
	setContentType(frameData[5]);
	
	int i = 6;
	std::string description;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	
	setDescription(description);
	++i;
	
	std::vector<std::pair<std::string, int>> syncText;
	while (i < (int)frameData.size()) {
		std::string identifier;
		while (frameData[i] != 0) {
			identifier += (char)frameData[i];
			++i;
		}
		
		++i;
		uint32_t timeStamp = frameData[i + 3] | (frameData[i + 2] << 8)
		                     | (frameData[i + 1] << 16) | (frameData[i] << 24);
		
		syncText.emplace_back(identifier, timeStamp);
		i += 4;
	}
	
	setSyncText(syncText);
	
	return generate();
}

void mp3::builders::COMRBuilder::setContact(const std::string& contact) {
	_contactURL = contact;
}

void mp3::builders::COMRBuilder::setDescription(const std::string& description) {
	_description = description;
}

void mp3::builders::COMRBuilder::setReceived(uint8_t received) {
	_receivedAs = received;
}

void mp3::builders::COMRBuilder::setPicture(const std::string& picture) {
	_pictureType = picture;
}

void mp3::builders::COMRBuilder::addByte(uint8_t byte) {
	_bytes.push_back(byte);
}

mp3::frame::Frame* mp3::builders::COMRBuilder::generate() const {
	return new mp3::frame::COMR(_frameName, _textEncoding,
								_price, _date, _seller,
								_contactURL, _receivedAs,
								_description, _pictureType,
								_bytes, _change);
}

mp3::frame::Frame* mp3::builders::COMRBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setEncoding(frameData[0]);
	
	std::string price;
	int i = 1;
	while (frameData[i] != 0) {
		price += (char)frameData[i];
		++i;
	}
	setPrice(price);
	++i;
	
	std::string validUntil;
	for (int j = 0; j < 8; ++j) {
		validUntil += (char)frameData[i + j];
	}
	setDate(validUntil);
	i += 8;
	
	std::string url;
	while (frameData[i] != 0) {
		url += (char)frameData[i];
		++i;
	}
	setContact(url);
	++i;
	
	setReceived(frameData[i]);
	++i;
	
	std::string seller;
	while (frameData[i] != 0) {
		seller += (char)frameData[i];
		++i;
	}
	setSeller(seller);
	++i;
	
	std::string description;
	while (frameData[i] != 0) {
		description += (char)frameData[i];
		++i;
	}
	setDescription(description);
	++i;
	
	std::string pictureType;
	while (frameData[i] != 0) {
		pictureType += (char)frameData[i];
		++i;
	}
	setPicture(pictureType);
	++i;
	
	while (i < (int)frameData.size()) {
		addByte(frameData[i]);
		++i;
	}
	
	return generate();
}

void mp3::builders::ENCRBuilder::setMethod(uint8_t method) {
	_methodSymbol = method;
}

void mp3::builders::ENCRBuilder::setOwner(const std::string& owner) {
	_ownerIdentifier = owner;
}

void mp3::builders::ENCRBuilder::addByte(uint8_t byte) {
	_binaryData.push_back(byte);
}

mp3::frame::Frame* mp3::builders::ENCRBuilder::generate() const {
	return new mp3::frame::ENCR(_frameName, _ownerIdentifier,
								_methodSymbol, _binaryData, _change);
}

mp3::frame::Frame* mp3::builders::ENCRBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	std::string owner;
	int i = 0;
	while (frameData[i] != 0) {
		owner += (char)frameData[i];
		++i;
	}
	setOwner(owner);
	++i;
	
	setMethod(frameData[i]);
	++i;
	
	while (i < (int)frameData.size()) {
		addByte(frameData[i]);
		++i;
	}
	
	return generate();
}

void mp3::builders::GRIDBuilder::setGroup(uint8_t symbol) {
	_groupSymbol = symbol;
}

void mp3::builders::GRIDBuilder::setOwner(const std::string& owner) {
	_ownerIdentifier = owner;
}

void mp3::builders::GRIDBuilder::addByte(uint8_t byte) {
	_binaryData.push_back(byte);
}

mp3::frame::Frame* mp3::builders::GRIDBuilder::generate() const {
	return new mp3::frame::GRID(_frameName, _ownerIdentifier,
								_groupSymbol, _binaryData, _change);
}

mp3::frame::Frame* mp3::builders::GRIDBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	int i = 0;
	std::string owner;
	while (frameData[i] != 0) {
		owner += (char)frameData[i];
		++i;
	}
	setOwner(owner);
	++i;
	
	setGroup(frameData[i]);
	++i;
	
	while (i < (int)frameData.size()) {
		addByte(frameData[i]);
		++i;
	}
	
	return generate();
}

void mp3::builders::PRIVBuilder::setOwner(const std::string& owner) {
	_ownerIdentifier = owner;
}

void mp3::builders::PRIVBuilder::addByte(uint8_t byte) {
	_binaryData.push_back(byte);
}

mp3::frame::Frame* mp3::builders::PRIVBuilder::generate() const {
	return new mp3::frame::PRIV(_frameName, _ownerIdentifier, _binaryData, _change);
}

mp3::frame::Frame* mp3::builders::PRIVBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	int i = 0;
	std::string owner;
	while (frameData[i] != 0) {
		owner += (char)frameData[i];
		++i;
	}
	setOwner(owner);
	++i;
	
	while (i < (int)frameData.size()) {
		addByte(frameData[i]);
		++i;
	}
	
	return generate();
}

void mp3::builders::SEEKBuilder::setOffset(uint64_t offset) {
	_offset = offset;
}

mp3::frame::Frame* mp3::builders::SEEKBuilder::generate() const {
	return new mp3::frame::SEEK(_frameName, _offset, _change);
}

mp3::frame::Frame* mp3::builders::SEEKBuilder::buildFromFile(std::ifstream& file, bool sync) {
	auto header = mp3::utils::parseFrameHeader(file);
	
	if (!header)
		return nullptr;
	
	setChange(header->readOnly);
	
	auto frameData = mp3::utils::readFrameData(header->size, file);
	if (sync)
		frameData = mp3::utils::removeUnSyncBytes(frameData);
	
	setOffset(frameData[3] | (frameData[2] << 8) | (frameData[1] << 16) | (frameData[0] << 24));
	
	return generate();
}