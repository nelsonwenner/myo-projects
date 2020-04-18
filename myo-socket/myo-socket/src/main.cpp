#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "DataCollector.cpp"
#include <myo/myo.hpp>


int main()
{	
	char SERVERPORT[12];
	char SERVER[12];

	ClientSocket* clientSocket = new ClientSocket();

	std::cout << "Enter the server [IP] to connect: ";
	std::cin >> SERVER;

	std::cout << "\nEnter the server [PORT] to connect: ";
	std::cin >> SERVERPORT;

	clientSocket->connecting((char*)SERVER, std::stoi(SERVERPORT));

	DataCollector* dataCollector = new DataCollector(clientSocket);

	try {
		myo::Hub hub("com.developer.socket-send");

		std::cout << "Waiting for connection myo...\n";

		myo::Myo* myo = hub.waitForMyo(10000);

		if (!myo) { throw std::runtime_error("Unable to find a Myo!"); }

		std::cout << "\n" << "Connected to a Myo armband!" << "\n";

		myo->setStreamEmg(myo::Myo::streamEmgEnabled);

		hub.addListener(dataCollector);

		while (true) {
			hub.run(1000 / 20);
			dataCollector->print();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
		std::cerr << "Press enter to continue.";
		clientSocket->closeSocket();
		return 1;
	}
}