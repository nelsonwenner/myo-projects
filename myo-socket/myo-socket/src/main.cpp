#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "DataCollector.cpp"
#include <myo/myo.hpp>

#define SERVER "127.0.0.1"
#define SERVERPORT 5000

int main()
{	
	ClientSocket* clientSocket = new ClientSocket();

	clientSocket->connecting(SERVER, SERVERPORT);

	DataCollector* dataCollector = new DataCollector(clientSocket);

	myo::Hub hub("com.developer.collector-data");

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