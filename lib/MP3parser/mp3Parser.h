#ifndef LOIKX_MP3_H
#define LOIKX_MP3_H

#include <string>
#include <vector>
#include "../header/header.h"
#include "../builder/builder.h"
#include <fstream>

namespace mp3 {
    class MP3MetaInformation {
    private:
        std::string _filePath;
        Header* _header = nullptr;
        std::vector<frame::Frame*> _frames;

    public:
        explicit MP3MetaInformation(std::string  path) : _filePath(std::move(path)) {}

        void parse();
		
		[[nodiscard]] std::vector<frame::Frame*> getFrameByName(const std::string& name) const;
		
		[[nodiscard]] const std::vector<frame::Frame*>& getFrames() const { return _frames; }
		
		void addFrame(frame::Frame* frame);
    };
}

#endif