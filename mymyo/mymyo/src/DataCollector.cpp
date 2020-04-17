#include <iostream>
#include <sstream>
#include <string>
#include <array>

#include <myo/myo.hpp>


class DataCollector : public myo::DeviceListener {
	private:
		std::array<int8_t, 8> data;
	
	public: 
		DataCollector() {}

		/* Overrider */
		void onEmgData(myo::Myo *myo, uint64_t timestamp, const int8_t *msg) {
			
			for (int i=0; i < 8; i++) { this->data[i] = msg[i]; }
		}

		void print() {
			std::cout << '\r';

			for (int i=0; i < this->data.size(); i++) {
				std::ostringstream buffer;
				buffer << static_cast<int>(this->data[i]);
				std::string msg = buffer.str();
				std::cout << '[' << msg << std::string(4 - msg.size(), ' ') << ']';
			}

			std::cout << std::flush;
		}
};

