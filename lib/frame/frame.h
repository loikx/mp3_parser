#ifndef LOIKX_FRAME_H
#define LOIKX_FRAME_H

#include <vector>
#include <string>
#include "../utils/utils.h"

namespace mp3::frame {
    class Frame {
    protected:
        std::string _frameName;
		bool _change = true;
		
		void canChange() const;
	public:
	    virtual void writeToFile(std::ofstream& file) = 0;
    
        Frame(const std::string& name, bool change) {
			_frameName = name;
			_change = change;
		}

        virtual void show() const = 0;

        [[nodiscard]] const std::string& getName() const { return _frameName; }
		
		[[nodiscard]] bool getChange() const { return _change; }
	
	    template<class T>
	    T* toOtherFrame() {
			try {
				return reinterpret_cast<T*>(this);
			} catch (const std::exception& ex) {
				throw std::invalid_argument("Not this type frame. Can not cast");
			}
		}
    };

    class TextFrame: public Frame {
    private:
        std::string _value;
        int _encoding;
		
	    void writeToFile(std::ofstream& file) override;
    public:
        TextFrame(const std::string& name, const std::string& value, int encoding, bool change): Frame(name, change) {
			_encoding = encoding;
			_value = value;
		}

        [[nodiscard]] const std::string& getValue() const { return _value; }

        [[nodiscard]] int getEncoding() const { return _encoding; }
		
		void setValue(const std::string& value) {
			canChange();
			_value = value;
		}
		
		void setEncoding(int encoding) {
			canChange();
			_encoding = encoding;
		}

        void show() const override;
    };

    class UFID: public Frame {
    private:
        std::string _ownerIdentifier;
        int _binaryData[64]{};
        int _capacity = 0;

	    void writeToFile(std::ofstream& file) override;

    public:
        UFID(const std::string& ownerIdentifier, const int binaryData[64],
			 int capacity, bool change): Frame("UFID", change) {
	        _ownerIdentifier = ownerIdentifier;
	        for (int i = 0; i < 64; ++i)
		        _binaryData[i] = binaryData[i];
	        _capacity = capacity;
		}

        [[nodiscard]] const std::string& getOwnerIdentifier() const { return _ownerIdentifier; }

        [[nodiscard]] std::pair<int*, int> getBinaryData() const;

        [[nodiscard]] int getCapacity() const { return _capacity; }
		
		void setOwner(const std::string& owner) {
			canChange();
			_ownerIdentifier = owner;
		}
		
		void setCapacity(int capacity) {
			canChange();
			_capacity = capacity;
		}
		
		void addToBinaryData(int data);
		
		void setBinaryData(const int binaryData[64]);

        void show() const override;
    };

    class TXXX: public TextFrame {
    private:
        std::string _description;

    public:
	    void writeToFile(std::ofstream& file) override;
		
        TXXX(const std::string& value, int encoding,
			 const std::string& description, bool change): TextFrame("TXXX", value, encoding, change) {
			_description = description;
		}

        [[nodiscard]] const std::string& getDescription() const { return _description; }
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}

        void show() const final;
    };

    class URLFrames : public Frame {
    protected:
        std::string _link;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        URLFrames(const std::string& name, const std::string& link,
				  bool change): Frame(name, change) {
			_link = link;
		}

        [[nodiscard]] const std::string& getLink() const { return _link; }
		
		void setLink(const std::string& link) {
			canChange();
			_link = link;
		}

        void show() const override;
    };

    class WXXX: public URLFrames {
    private:
        std::string _description;
		int _encoding;
		
	    void writeToFile(std::ofstream& file) override;

    public:
        WXXX(const std::string& name, int encoding, const std::string& link,
			 const std::string& description, bool change): URLFrames(name, link, change) {
			_description = description;
			_encoding = encoding;
		}

        [[nodiscard]] const std::string& getDescription() const { return _description; }
		
		[[nodiscard]] int getEncoding() const { return _encoding; }
		
		void setEncoding(int encoding) {
			canChange();
			_encoding = encoding;
		}
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}

        void show() const final;
    };

    class ETCO: public Frame {
    private:
        int _timeStampFormat;
        std::vector<std::pair<int, int>> _eventsAndTimes;
	
	    void writeToFile(std::ofstream& file) override;

    public:
        ETCO(const std::string& name, int format,
			 const std::vector<std::pair<int, int>>& events, bool change): Frame(name, change) {
			_timeStampFormat = format;
			_eventsAndTimes = events;
		}

        [[nodiscard]] int getTimeStampFormat() const { return _timeStampFormat; }

        [[nodiscard]] const std::vector<std::pair<int, int>>& getEvents() const { return _eventsAndTimes; }
		
		void setTimeStampFormat(int timeStampFormat) {
			canChange();
			_timeStampFormat = timeStampFormat;
		}

        void show() const final;
    };

    class USLT: public TextFrame {
    private:
        std::string _language;
        std::string _description;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        USLT(const std::string& name, int encoding,
             const std::string& value, const std::string& language,
             const std::string& description, bool change): TextFrame(name, value, encoding, change) {
			_language = language;
			_description = description;
		}

        [[nodiscard]] const std::string& getDescription() const { return _description; };

        [[nodiscard]] const std::string& getLanguage() const { return _language; }
		
		void setLanguage(const std::string& language) {
			canChange();
			_language = language;
		}
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}

        void show() const final;
    };

    class SYLT: public TextFrame {
	private:
		std::string _language;
		uint8_t _timeStampFormat;
		uint8_t _contentType;
		std::string _description;
		std::vector<std::pair<std::string, int>> _syncText;
		
	    void writeToFile(std::ofstream& file) override;
		
	public:
		SYLT(const std::string& name, int encoding,
			 const std::string& value, const std::string& language,
			 uint8_t timeStamp, uint8_t contentType,
			 const std::string& description,
			 const std::vector<std::pair<std::string, int>>& syncText,
			 bool change): TextFrame(name, value, encoding, change) {
			_language = language;
			_timeStampFormat = timeStamp;
			_contentType = contentType;
			_description = description;
			_syncText = syncText;
		}
		
		void show() const final;
		
		[[nodiscard]] const std::string& getLanguage() const { return _language; }
		
		[[nodiscard]] uint8_t getTimeStamp() const { return _timeStampFormat; }
		
		[[nodiscard]] uint8_t getContentType() const { return _contentType; }
	
	    [[nodiscard]] const std::string& getDescription() const { return _description; }
		
		[[nodiscard]] const std::vector<std::pair<std::string, int>>& getSyncText() const { return _syncText; }
		
		void setLanguage(const std::string& language) {
			canChange();
			_language = language;
		}
		
		void setTimeStampFormat(int timeStampFormat) {
			canChange();
			_timeStampFormat = timeStampFormat;
		}
		
		void setContentType(uint8_t content) {
			canChange();
			_contentType = content;
		}
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}
		
		void setSyncText(const std::vector<std::pair<std::string, int>>& syncText) {
			canChange();
			_syncText = syncText;
		}
    };

    class COMM: public TextFrame {
    private:
        std::string _language;
        std::string _description;
	
	    void writeToFile(std::ofstream& file) override;

    public:
        COMM(const std::string& name, int encoding,
             const std::string& language, const std::string& description,
             const std::string& value, bool change): TextFrame(name, value, encoding, change) {
			_language = language;
			_description = description;
		}

        [[nodiscard]] const std::string& getLanguage() const { return _language; }

        [[nodiscard]] const std::string& getDescription() const { return _description; }
		
		void setLanguage(const std::string& language) {
			canChange();
			_language = language;
		}
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}

        void show() const final;
    };

    class RVA2: public Frame {
    private:
        std::string _identification;
        std::vector<mp3::utils::Peak*> _peaks;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        RVA2(const std::string& name,
             const std::string& identification,
             const std::vector<mp3::utils::Peak*>& peaks, bool change): Frame(name, change) {
	        _identification = identification;
			_peaks = peaks;
		}

        [[nodiscard]] const std::string& getIdentification() const { return _identification; }

        [[nodiscard]] const std::vector<mp3::utils::Peak*>& getPeaks() const { return _peaks; }
		
		void setIdentification(const std::string& identification) {
			canChange();
			_identification = identification;
		}
		
		void addPeak(mp3::utils::Peak* peak) {
			canChange();
			_peaks.push_back(peak);
		}
		
		void setPeaks(const std::vector<mp3::utils::Peak*>& peaks) {
			canChange();
			_peaks = peaks;
		}

        void show() const final;
    };

    class EQU2: public Frame {
    private:
        int _interpolationMethod;
        std::string _identification;
        std::vector<std::pair<int, int>> _adjustmentPoints;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        EQU2(const std::string& name,
             int interpolation,
             const std::string& identification,
             const std::vector<std::pair<int, int>>& adjustment, bool change): Frame(name, change) {
			_interpolationMethod = interpolation;
			_identification = identification;
			_adjustmentPoints = adjustment;
		}

        [[nodiscard]] int getInterpolationMethod() const { return _interpolationMethod; }

        [[nodiscard]] const std::string& getIdentification() const { return _identification; }

        [[nodiscard]] const std::vector<std::pair<int, int>>& getAdjustmentPoints() const { return _adjustmentPoints; }
		
		void setInterpolation(int interpolation) {
			canChange();
			_interpolationMethod = interpolation;
		}
		
		void setIdentification(const std::string& identification) {
			canChange();
			_identification = identification;
		}
		
		void addAdjustmentPoint(const std::pair<int, int>& point) {
			canChange();
			_adjustmentPoints.push_back(point);
		}
		
		void setAdjustmentPoints(const std::vector<std::pair<int, int>>& points) {
			canChange();
			_adjustmentPoints = points;
		}

        void show() const final;
    };

    class PCNT: public Frame {
    private:
        uint64_t _cnt;
		
	    void writeToFile(std::ofstream& file) override;
    public:
		
        PCNT(const std::string& name, uint64_t cnt, bool change): Frame(name, change) {
			_cnt = cnt;
		}
		
        [[nodiscard]] uint64_t getCnt() const { return _cnt; }
		
		void setCnt(uint64_t cnt) {
			canChange();
			_cnt = cnt;
		}

        void show() const final;
    };

    class POMP: public Frame {
    private:
        std::string _email;
        uint8_t _rating;
        uint64_t _cnt;
		
	    void writeToFile(std::ofstream& file) override;

    public:
        POMP(const std::string& name, const std::string& email,
             uint8_t rating, uint64_t cnt, bool change): Frame(name, change) {
			_email = email;
			_rating = rating;
			_cnt = cnt;
		}

        [[nodiscard]] const std::string& getEmail() const { return _email; }

        [[nodiscard]] uint8_t getRating() const { return _rating; }

        [[nodiscard]] uint64_t getCnt() const { return _cnt; }
		
		void setEmail(const std::string& email) {
			canChange();
			_email = email;
		}
		
		void setRating(uint8_t rating) {
			canChange();
			_rating = rating;
		}
		
		void setCnt(uint64_t cnt) {
			canChange();
			_cnt = cnt;
		}

        void show() const final;
    };

    class RBUF: public Frame {
    private:
        uint32_t _bufferSize;
        bool _flag;
        uint32_t _nextTagOffset;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        RBUF(const std::string& name, uint32_t size,
             bool flag, uint32_t offset, bool change): Frame(name, change) {
			_bufferSize = size;
			_flag = flag;
			_nextTagOffset = offset;
		}

        [[nodiscard]] bool getFlag() const { return _flag; }

        [[nodiscard]] uint32_t getSize() const { return _bufferSize; }

        [[nodiscard]] uint32_t getOffset() const { return _nextTagOffset; }
		
		void setBufferSize(uint32_t bufferSize) {
			canChange();
			_bufferSize = bufferSize;
		}
		
		void setFlag(bool flag) {
			canChange();
			_flag = flag;
		}
		
		void setNextTagOffset(uint64_t offset) {
			canChange();
			_nextTagOffset = offset;
		}

        void show() const final;
    };

    class LINK: public Frame {
    private:
        std::string _frameIdentifier;
        std::string _url;
        std::string _id;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        LINK(const std::string& name, const std::string& frame,
             const std::string& url, const std::string& id, bool change): Frame(name, change) {
			_frameIdentifier = frame;
			_url = url;
			_id = id;
		}

        [[nodiscard]] const std::string& getIdentifier() const { return _frameIdentifier; }

        [[nodiscard]] const std::string& getUrl() const { return _url; }

        [[nodiscard]] const std::string& getId() const { return _id; }
		
		void setFrameIndentifier(const std::string& frameIdentifier) {
			canChange();
			_frameIdentifier = frameIdentifier;
		}
		
		void setUrl(const std::string& url) {
			canChange();
			_url = url;
		}
		
		void setId(const std::string& id) {
			canChange();
			_id = id;
		}

        void show() const final;
    };

    class POSS: public Frame {
    private:
        int _timeStampFormat;
        uint64_t _position;
	
	    void writeToFile(std::ofstream& file) override;
		
    public:
        POSS(const std::string& name, int format,
			 uint64_t position, bool change): Frame(name, change) {
			_timeStampFormat = format;
			_position = position;
		}

        [[nodiscard]] int getTimeStampFormat() const { return _timeStampFormat; }

        [[nodiscard]] uint64_t getPosition() const { return _position; }
		
		void setTimeStampFormat(int timeStampFormat) {
			canChange();
			_timeStampFormat = timeStampFormat;
		}
		
		void setPosition(uint64_t position) {
			canChange();
			_position = position;
		}
		
		void show() const final;
    };
	
	class USER: public TextFrame {
	private:
		std::string _language;
		
		void writeToFile(std::ofstream& file) override;

	public:
		USER(const std::string& name, int encoding,
			 const std::string& language, const std::string& value,
			 bool change): TextFrame(name, value, encoding, change) {
			_language = language;
		}
		
		[[nodiscard]] const std::string& getLanguage() const { return _language; }
		
		void setLanguage(const std::string& language) {
			canChange();
			_language = language;
		}
		
		void show() const final;
	};
	
	class OWNE: public Frame {
	private:
		int _textEncoding;
		std::string _price;
		std::string _date;
		std::string _seller;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		OWNE(const std::string& name, int encoding,
			 const std::string& price, const std::string& date,
			 const std::string& seller, bool change): Frame(name, change) {
			_textEncoding = encoding;
			_date = date;
			_price = price;
			_seller = seller;
		}
		
		[[nodiscard]] int getEncoding() const { return _textEncoding; }
		
		[[nodiscard]] const std::string& getPrice() const { return _price; }
		
		[[nodiscard]] const std::string& getDate() const { return _date; }
		
		[[nodiscard]] const std::string& getSeller() const { return _seller; }
		
		void setEncoding(int encoding) {
			canChange();
			_textEncoding = encoding;
		}
		
		void setPrice(const std::string& price) {
			canChange();
			_price = price;
		}
		
		void setDate(const std::string& date) {
			canChange();
			_date = date;
		}
		
		void setSeller(const std::string& seller) {
			canChange();
			_seller = seller;
		}
		
		void show() const override;
	};
	
	class COMR: public OWNE {
	private:
		std::string _contactURL;
		uint8_t _receivedAs;
		std::string _description;
		std::string _pictureType;
		std::vector<uint8_t> _bytes;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		COMR(const std::string& name, int encoding,
			 const std::string& price, const std::string& date,
			 const std::string& seller, const std::string& contact,
			 uint8_t receivedAs, const std::string& description,
			 const std::string& picture, const std::vector<uint8_t>& bytes,
			 bool change): OWNE(name, encoding, price, date, seller, change) {
			_receivedAs = receivedAs;
			_description = description;
			_pictureType = picture;
			_bytes = bytes;
			_contactURL = contact;
		}
		
		void show() const final;
		
		[[nodiscard]] const std::string& getContact() const { return _contactURL; }
		
		[[nodiscard]] uint8_t getReceived() const { return _receivedAs; }
		
		[[nodiscard]] const std::string& getDescription() const { return _description; }
		
		[[nodiscard]] const std::string& getPictureType() const { return _pictureType; }
		
		[[nodiscard]] const std::vector<uint8_t>& getBinary() const { return _bytes; }
		
		void setContact(const std::string& contact) {
			canChange();
			_contactURL = contact;
		}
		
		void setRecieved(uint8_t received) {
			canChange();
			_receivedAs = received;
		}
		
		void setDescription(const std::string& description) {
			canChange();
			_description = description;
		}
		
		void setPicture(const std::string& picture) {
			canChange();
			_pictureType = picture;
		}
	};
	
	class ENCR: public Frame {
	private:
		std::string _ownerIdentifier;
		uint8_t _methodSymbol;
		std::vector<uint8_t> _binaryData;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		ENCR(const std::string& name, const std::string& owner,
			 uint8_t methodSymbol, const std::vector<uint8_t>& binaryData, bool change): Frame(name, change) {
			_ownerIdentifier = owner;
			_methodSymbol = methodSymbol;
			_binaryData = binaryData;
		}
		
		void show() const final;
		
		[[nodiscard]] const std::string& getOwnerIdentifier() const { return _ownerIdentifier; }
		
		[[nodiscard]] uint8_t getMethodSymbol() const { return _methodSymbol; }
		
		[[nodiscard]] const std::vector<uint8_t>& getBinaryData() const { return _binaryData; }
		
		void setOwnerIdentifier(const std::string& owner) {
			canChange();
			_ownerIdentifier = owner;
		}
		
		void setMethod(uint8_t method) {
			canChange();
			_methodSymbol = method;
		}
		
		void addBinaryData(uint8_t byte) {
			canChange();
			_binaryData.push_back(byte);
		}
		
		void setBinaryData(const std::vector<uint8_t>& bytes) {
			canChange();
			_binaryData = bytes;
		}
	};
	
	class GRID: public Frame {
	private:
		std::string _ownerIdentifier;
		uint8_t _groupSymbol;
		std::vector<uint8_t> _binaryData;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		GRID(const std::string& name, const std::string& owner,
			 uint8_t group, const std::vector<uint8_t>& data,
			 bool change): Frame(name, change) {
			_ownerIdentifier = owner;
			_groupSymbol = group;
			_binaryData = data;
		}
		
		void show() const final;
		
		[[nodiscard]] const std::string& getOwnerIdentifier() const { return _ownerIdentifier; }
		
		[[nodiscard]] uint8_t getGroupSymbol() const { return _groupSymbol; }
		
		[[nodiscard]] const std::vector<uint8_t>& getData() const { return _binaryData; }
		
		void setOwnerIdentifier(const std::string& owner) {
			canChange();
			_ownerIdentifier = owner;
		}
		
		void setGroup(uint8_t group) {
			canChange();
			_groupSymbol = group;
		}
		
		void addBinaryData(uint8_t byte) {
			canChange();
			_binaryData.push_back(byte);
		}
		
		void setBinaryData(const std::vector<uint8_t>& data) {
			canChange();
			_binaryData = data;
		}
	};
	
	class PRIV: public Frame {
	private:
		std::string _ownerIdentifier;
		std::vector<uint8_t> _binaryData;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		PRIV(const std::string& name, const std::string& owner,
			 const std::vector<uint8_t>& data, bool change): Frame(name, change) {
			_ownerIdentifier = owner;
			_binaryData = data;
		}
		
		void show() const final;
		
		[[nodiscard]] const std::string& getOwnerIdentifier() const { return _ownerIdentifier; }
		
		[[nodiscard]] const std::vector<uint8_t>& getBinaryData() const { return _binaryData; }
		
		void setOwnerIdentifier(const std::string& owner) {
			canChange();
			_ownerIdentifier = owner;
		}
		
		void addBinaryData(uint8_t byte) {
			canChange();
			_binaryData.push_back(byte);
		}
		
		void setBinaryData(const std::vector<uint8_t>& bytes) {
			canChange();
			_binaryData = bytes;
		}
	};
	
	class SEEK: public Frame {
	private:
		uint64_t _offset;
		
		void writeToFile(std::ofstream& file) override;
		
	public:
		SEEK(const std::string& frameName, uint64_t offset, bool change): Frame(frameName, change) {
			_offset = offset;
		}
		
		void show() const final;
		
		[[nodiscard]] uint64_t getOffset() const { return _offset; }
		
		void setOffset(uint64_t offset) {
			canChange();
			_offset = offset;
		}
	};
}

#endif