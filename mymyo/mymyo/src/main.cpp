#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "DataCollector.cpp"
#include <myo/myo.hpp>


int main()
{
	myo::Hub hub("com.example.collector-data");

	std::cout << "Waiting for connection myo...\n";
	
	myo::Myo* myo = hub.waitForMyo(10000);

	if (!myo) { throw std::runtime_error("Unable to find a Myo!"); }

	std::cout << "\n" << "Connected to a Myo armband!" << "\n\n";

	myo->setStreamEmg(myo::Myo::streamEmgEnabled);

	DataCollector dataCollector;

	hub.addListener(&dataCollector);

	while (true) {
		hub.run(1000 / 20);
		dataCollector.print();
	}
}
