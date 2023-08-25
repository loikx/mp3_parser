#ifndef LOIKX_HEADER_H
#define LOIKX_HEADER_H

#include <bitset>
#include <iostream>
#include <vector>
#include "../frame/frame.h"
#include <fstream>

const int HEADER_SIZE = 10;

namespace mp3 {
    class Header {
    private:
        std::string _idvVersion;
        int _metaSize;
        bool _unSynchronisation;
        bool _extendedHeader;
        bool _experimentalIndicator;
        bool _footer;

    public:
        explicit Header(uint8_t bytes[10]);

        [[nodiscard]] const std::string& getVersion() const { return _idvVersion; }

        [[nodiscard]] int getMetaSize() const { return _metaSize; }

        [[nodiscard]] bool getUnSynchronisation() const { return _unSynchronisation; }

        [[nodiscard]] bool getExtendedHeader() const { return _extendedHeader; }

        [[nodiscard]] bool getExperimentalIndicator() const { return _experimentalIndicator; }

        [[nodiscard]] bool getFooter() const { return _footer; }
    };
	
	void createFile(const std::string& path, Header* header,
					const std::vector<frame::Frame*>& frames, const std::vector<uint8_t>& binaryData);
}

#endif