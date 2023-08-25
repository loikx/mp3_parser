#ifndef LOIKX_UTILS_H
#define LOIKX_UTILS_H

#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>

namespace mp3::utils {
    template<typename T>
    void printFrames(T arg) {
        std::cout << arg;
    }

    template<typename T, typename... ArgTypes>
    void printFrames(T t, ArgTypes... args) {
        std::cout << t;
        printFrames(args...);
    }
	
	std::vector<uint8_t> removeUnSyncBytes(const std::vector<uint8_t>& binaryData);
	
    class Peak {
    private:
        uint8_t _typeChannel;
        uint16_t _volumeAdjustment;
        uint8_t _bitsRepresentingPeak;
        uint64_t _peakVolume;

    public:
        explicit Peak(uint8_t typeChannel, uint16_t volumeAdjustment,
                      uint8_t bitsRepresentingPeak,
                      uint64_t peakVolume) : _typeChannel(typeChannel), _volumeAdjustment(volumeAdjustment),
                                             _bitsRepresentingPeak(bitsRepresentingPeak), _peakVolume(peakVolume) {}

        [[nodiscard]] int channelType() const { return _typeChannel; }

        [[nodiscard]] int volumeAdjustment() const { return _volumeAdjustment; }

        [[nodiscard]] int bitsRepresentingPeak() const { return _bitsRepresentingPeak; }

        [[nodiscard]] uint64_t peakVolume() const { return _peakVolume; }
    };
	
	std::string encodingName(int encoding);
	
	struct FrameHeader {
		uint16_t size = 0;
		bool tagAlterPreservation;
		bool fileAlterPreservation;
		bool readOnly;
		bool groupIdentity;
		bool compression;
		bool encryption;
		bool unSynchronisation;
		bool dataLength;
		
		void parse(std::ifstream& file);
	};
	
	FrameHeader* parseFrameHeader(std::ifstream& file);
	
	std::vector<uint8_t> readFrameData(int size, std::ifstream& file);

	const std::vector<std::string> urlFrames = {
			"WCOM", "WCOP", "WOAF", "WOAR", "WOAS",
			"WORS", "WPAY", "WPUB"
	};
	
	const std::vector<std::string> skipFrames = {
			"AENC", "APIC", "ASPI", "GEOB", "MCDI",
			"MLLT", "RVRB", "SIGN", "SYTC", "TFLT",
			"TMED"
	};
}

#endif