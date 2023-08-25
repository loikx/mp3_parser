#include "mp3Parser.h"

void mp3::MP3MetaInformation::parse() {
	std::ifstream file;
	file.open(_filePath, std::ios::binary);
	
	if (!file.is_open())
		throw std::runtime_error("Can not not open file: " + _filePath);
	
	uint8_t headerBytes[10];
	for (uint8_t& headerByte : headerBytes) {
		headerByte = file.get();
	}
	
    _header = new Header(headerBytes);
	
	if (_header->getExtendedHeader()) {
		// skip extended header
		uint32_t extendedHeaderSize;
		file.read(reinterpret_cast<char*>(&extendedHeaderSize), sizeof(extendedHeaderSize));
		// skip flags
		file.get();
		file.get();
		
		for (int i = 0; i < extendedHeaderSize; ++i) {
			file.get();
		}
	}
	
	bool flag;
	do {
		flag = false;
		builders::FrameBuilder* builder = nullptr;
		std::string frameName = std::string(1, (char)file.get()) + std::string(1, (char)file.get())
				+ std::string(1, (char)file.get()) + std::string(1, (char)file.get());
		
		std::reverse(frameName.begin(), frameName.end());
		
		if (frameName[0] == 'T' && frameName != "TXXX") {
			flag = true;
			builder = new mp3::builders::TextFrameBuilder();

		} else if (frameName[0] == 'W' && frameName != "WXXX") {
			flag = true;
			builder = new mp3::builders::URLFrameBuilder();
			
		} else if (std::find(mp3::utils::skipFrames.begin(), mp3::utils::skipFrames.end(), frameName)
						!= mp3::utils::skipFrames.end()) {
			flag = true;
			auto header = mp3::utils::parseFrameHeader(file);
			for (int i = 0; i < header->size; ++i)
				file.get();
			
		} else if (frameName == "TXXX") {
			flag = true;
			builder = new builders::TXXXBuilder();
			
		} else if (frameName == "WXXX") {
			flag = true;
			builder = new builders::WXXXBuilder();
			
		} else if (frameName == "UFID") {
			flag = true;
			builder = new builders::UFIDBuilder();
			
		} else if (frameName == "ETCO") {
			flag = true;
			builder = new builders::ETCOBuilder();
			
		} else if (frameName == "USLT") {
			flag = true;
			builder = new builders::USLTBuilder();
			
		} else if (frameName == "SYLT") {
			flag = true;
			builder = new builders::SYLTBuilder();
			
		} else if (frameName == "COMM") {
			flag = true;
			builder = new builders::COMMBuilder();
			
		} else if (frameName == "RVA2") {
			flag = true;
			builder = new builders::RVA2Builder();
			
		} else if (frameName == "EQU2") {
			flag = true;
			builder = new builders::EQU2Builder();
			
		} else if (frameName == "PCNT") {
			flag = true;
			builder = new builders::PCNTBuilder();
			
		} else if (frameName == "RBUF") {
			flag = true;
			builder = new builders::RBUFBuilder();
			
		} else if (frameName == "LINK") {
			flag = true;
			builder = new builders::LINKBuilder();
			
		} else if (frameName == "POSS") {
			flag = true;
			builder = new builders::POSSBuilder();
			
		} else if (frameName == "USER") {
			flag = true;
			builder = new builders::USERBuilder();
			
		} else if (frameName == "OWNE") {
			flag = true;
			builder = new builders::OWNEBuilder();
			
		} else if (frameName == "COMR") {
			flag = true;
			builder = new builders::COMRBuilder();
			
		} else if (frameName == "ENCR") {
			flag = true;
			builder = new builders::ENCRBuilder();
			
		} else if (frameName == "GRID") {
			flag = true;
			builder = new builders::GRIDBuilder();
			
		} else if (frameName == "PRIV") {
			flag = true;
			builder = new builders::PRIVBuilder();
			
		} else if (frameName == "SEEK") {
			flag = true;
			builder = new builders::SEEKBuilder();
			builder->setName(frameName);
			auto tmp = builder->buildFromFile(file, _header->getUnSynchronisation());
			file.ignore((long long)tmp->toOtherFrame<mp3::frame::SEEK>()->getOffset());
			addFrame(tmp);
			continue;
		} else if (frameName == "POPM") {
			builder = new mp3::builders::POMPBuilder();
			flag = true;
		}
		
		if (builder != nullptr) {
			builder->setName(frameName);
			addFrame(builder->buildFromFile(file, _header->getUnSynchronisation()));
		}
	} while (flag && !file.eof());
}

std::vector<mp3::frame::Frame*> mp3::MP3MetaInformation::getFrameByName(const std::string& name) const {
	std::vector<mp3::frame::Frame*> frames;
	
	for (const auto& item: _frames) {
		if (item->getName() == name) {
			frames.push_back(item);
		}
	}
	
	return frames;
}

void mp3::MP3MetaInformation::addFrame(frame::Frame* frame) {
	_frames.push_back(frame);
}