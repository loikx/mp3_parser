#ifndef LOIKX_BUILDER_H
#define LOIKX_BUILDER_H

#include <string>
#include <iostream>
#include <fstream>
#include "../frame/frame.h"

namespace mp3::builders {
    class FrameBuilder {
    protected:
        std::string _frameName;
		bool _change;
		
    public:
        [[nodiscard]] virtual frame::Frame* generate() const = 0;

        void setName(const std::string& name);
		
		void setChange(bool change);
		
		[[nodiscard]] virtual frame::Frame* buildFromFile(std::ifstream& file, bool sync) = 0;
    };

    class TextFrameBuilder : public FrameBuilder {
    protected:
        std::string _value;
        int _encoding;

    public:
        void setEncoding(int encoding);

        void setValue(const std::string& value);

        [[nodiscard]] frame::Frame* generate() const override;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) override;
    };

    class UFIDBuilder: public FrameBuilder {
    private:
        std::string _ownerIdentifier;
        int _binaryData[64]{};
        int _capacity = 0;

    public:
        void setCapacity(int capacity);

        void setOwnerIdentifier(const std::string& ownerIdentifier);

        void setBinaryData(const int data[64]);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) override;
    };

    class TXXXBuilder: public TextFrameBuilder {
    private:
        std::string _description;

    public:
        void setDescription(const std::string& description);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class URLFrameBuilder: public FrameBuilder {
    protected:
        std::string _link;

    public:
        void setLink(const std::string& link);

        [[nodiscard]] frame::Frame* generate() const override;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) override;
    };

    class WXXXBuilder : public URLFrameBuilder {
    private:
        std::string _description;
		int _encoding;

    public:
        void setDescription(const std::string& description);
		
		void setEncoding(int encoding);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class ETCOBuilder: public FrameBuilder {
    private:
        int _timeStampFormat;
        std::vector<std::pair<int, int>> _eventsAndTimes;

    public:
        void setFormat(int format);

        void setEvents(const std::vector<std::pair<int, int>>& events);

        void clear();

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class USLTBuilder: public TextFrameBuilder {
    private:
        std::string _description;
        std::string _language;

    public:
        void setLanguage(const std::string& language);

        void setDescription(const std::string& description);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class SYLTBuilder: public TextFrameBuilder {
	private:
		std::string _language;
		uint8_t _timeStampFormat;
		uint8_t _contentType;
		std::string _description;
		std::vector<std::pair<std::string, int>> _syncText;
		
	public:
		void setLanguage(const std::string& language);
		
		void setTimeStampFormat(uint8_t timeFormat);
		
		void setContentType(uint8_t content);
		
		void setDescription(const std::string& description);
		
		void setSyncText(const std::vector<std::pair<std::string, int>>& syncText);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class COMMBuilder: public TextFrameBuilder {
    private:
        std::string _language;
        std::string _description;

    public:
        void setLanguage(const std::string& language);

        void setDescription(const std::string& description);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class RVA2Builder: public FrameBuilder {
    private:
        std::string _identification;
        std::vector<mp3::utils::Peak*> _peaks;

    public:
        void setIdentification(const std::string& identification);

        void addPeak(mp3::utils::Peak* peak);

        void clear();

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class EQU2Builder: public FrameBuilder {
    private:
        int _interpolation;
        std::string _identification;
        std::vector<std::pair<int, int>> _adjustmentPoints;

    public:
        void setInterpolation(int interpolation);

        void setIdentification(const std::string& identification);

        void addAdjustment(const std::pair<int, int>& adjustmentPoint);

        void clear();

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class PCNTBuilder: public FrameBuilder {
    private:
        uint64_t _cnt = 0;

    public:
        void setCnt(uint64_t cnt);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class POMPBuilder: public FrameBuilder {
    private:
        std::string _email;
        uint8_t _rating;
        uint64_t _cnt;

    public:
        void setEmail(const std::string& email);

        void setRating(uint8_t rating);

        void setCnt(uint64_t cnt);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class RBUFBuilder: public FrameBuilder {
    private:
        uint32_t _bufferSize = 0;
        bool _flag = false;
        uint64_t _nextTagOffset = 0;

    public:
        void setBufferSize(uint32_t size);

        void setFlag(bool flag);

        void setOffset(uint64_t offset);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };

    class LINKBuilder: public FrameBuilder {
    private:
        std::string _frameIdentifier;
        std::string _url;
        std::string _id;

    public:
        void setIdentifier(const std::string& identifier);

        void setURL(const std::string& url);

        void setId(const std::string& id);

        [[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
    };
	
	class POSSBuilder: public FrameBuilder {
	private:
		int _timeStampFormat = 0;
		uint64_t _position = 0;
		
	public:
		void setFormat(int format);
		
		void setPosition(uint64_t position);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class USERBuilder: public TextFrameBuilder {
	private:
		std::string _language;
		
	public:
		void setLanguage(const std::string& lang);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class OWNEBuilder: public FrameBuilder {
	protected:
		int _textEncoding;
		std::string _price;
		std::string _date;
		std::string _seller;
		
	public:
		void setEncoding(int encoding);
		
		void setPrice(const std::string& price);
		
		void setDate(const std::string& date);
		
		void setSeller(const std::string& seller);
		
		[[nodiscard]] frame::Frame* generate() const override;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) override;
	};
	
	class COMRBuilder: public OWNEBuilder {
	private:
		std::string _contactURL;
		uint8_t _receivedAs = 0;
		std::string _description;
		std::string _pictureType;
		std::vector<uint8_t> _bytes;
		
	public:
		void setContact(const std::string& contact);
		
		void setReceived(uint8_t received);
		
		void setDescription(const std::string& description);
		
		void setPicture(const std::string& picture);
		
		void addByte(uint8_t byte);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class ENCRBuilder: public FrameBuilder {
	private:
		std::string _ownerIdentifier;
		uint8_t _methodSymbol = 0;
		std::vector<uint8_t> _binaryData;
		
	public:
		void setOwner(const std::string& owner);
		
		void setMethod(uint8_t method);
		
		void addByte(uint8_t byte);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class GRIDBuilder: public FrameBuilder {
	private:
		std::string _ownerIdentifier;
		uint8_t _groupSymbol = 0;
		std::vector<uint8_t> _binaryData;
		
	public:
		void setOwner(const std::string& owner);
		
		void setGroup(uint8_t symbol);
		
		void addByte(uint8_t byte);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class PRIVBuilder: public FrameBuilder {
	private:
		std::string _ownerIdentifier;
		std::vector<uint8_t> _binaryData;
		
	public:
		void setOwner(const std::string& owner);
		
		void addByte(uint8_t byte);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
	
	class SEEKBuilder: public FrameBuilder {
	private:
		uint64_t _offset = 0;
		
	public:
		void setOffset(uint64_t offset);
		
		[[nodiscard]] frame::Frame* generate() const final;
		
		[[nodiscard]] frame::Frame* buildFromFile(std::ifstream& file, bool sync) final;
	};
}

#endif