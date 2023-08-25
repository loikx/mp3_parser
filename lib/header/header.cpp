#include "header.h"

mp3::Header::Header(uint8_t bytes[10]) {

    const uint8_t zeroASCIICode = '0';
    // First 5 bytes it's id3v2.4 or other versions
    _idvVersion += static_cast<char>(bytes[0]);
    _idvVersion += static_cast<char>(bytes[1]);
    _idvVersion += static_cast<char>(bytes[2]);
    _idvVersion += "v2.";
    _idvVersion += static_cast<char>(bytes[3] + zeroASCIICode);

	if (_idvVersion != "ID3v2.4")
		throw std::runtime_error("Not valid type of mp3. Needed id3v2.4");

    const auto& flags = std::bitset<8>(bytes[5]);
    _unSynchronisation = flags[0];
    _extendedHeader = flags[1];
    _experimentalIndicator = flags[2];
    _footer = flags[3];

    // Fill meta size with 4 bytes
    _metaSize = int(static_cast<unsigned char>(bytes[6]) << 24 |
                static_cast<unsigned char>(bytes[7]) << 16 |
                static_cast<unsigned char>(bytes[8]) << 8 |
                static_cast<unsigned char>(bytes[9]));
}

void mp3::createFile(const std::string& path, Header* header,
					 const std::vector<frame::Frame*>& frames, const std::vector<uint8_t>& binaryData) {
	std::ofstream file;
	file.open(path, std::ios::binary);
	uint8_t id3[5] = {73, 68, 51, 04, 00};
	file.write(reinterpret_cast<char*>(&id3), sizeof(id3));
	file.write(reinterpret_cast<char*>(&id3[4]), sizeof(char));
	uint8_t bytesMetaSize[4];
	bytesMetaSize[0] = header->getMetaSize() >> 24;
	bytesMetaSize[1] = header->getMetaSize() >> 16;
	bytesMetaSize[2] = header->getMetaSize() >> 8;
	bytesMetaSize[3] = header->getMetaSize() >> 0;
	file.write(reinterpret_cast<char*>(&bytesMetaSize), sizeof(uint32_t));
	
	for (const auto& item: frames)
		item->writeToFile(file);
	
	file.close();
}