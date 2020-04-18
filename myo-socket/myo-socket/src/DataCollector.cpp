#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <iomanip>
#include <array>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "ClientSocket.cpp"
#include <myo/myo.hpp>


class DataCollector : public myo::DeviceListener {
private:
	ClientSocket* clientSocket;
	myo::Pose currentPose;
	myo::Arm whichArm;
	bool isUnlocked;
	bool onArm;

public:
	DataCollector(ClientSocket* socket) {
		this->clientSocket = socket;
		this->isUnlocked = false;
		this->onArm = false;
	}

	/* Overrider */
	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		this->isUnlocked = false;
		this->onArm = false;
	}

	/* Overrider */
	void onPose(myo::Myo * myo, uint64_t timestamp, myo::Pose pose) {
		this->currentPose = pose;
		
		if (pose != myo::Pose::unknown) {

			myo->unlock(myo::Myo::unlockHold);

			myo->notifyUserAction();
			
			this->socketSendPose();
		}
	}

	/* Overrider */
	void onArmSync(myo::Myo * myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
		this->onArm = true;
		this->whichArm = arm;
	}

	/* Overrider */
	void onArmUnsync(myo::Myo * myo, uint64_t timestamp) {
		this->onArm = false;
	}

	/* Overrider */
	void onUnlock(myo::Myo * myo, uint64_t timestamp) {
		isUnlocked = true;
	}

	/* Overrider */
	void onLock(myo::Myo * myo, uint64_t timestamp) {
		isUnlocked = false;
	}

	void socketSendPose() {
		std::string poseString = this->currentPose.toString();
		this->clientSocket->sendFrame((char*)poseString.c_str());
	}

	void print() {
		std::cout << '\r';

		if (onArm) {

			std::string poseString = this->currentPose.toString();

			std::cout << '[' << (this->isUnlocked ? "unlocked" : "locked  ") << ']'
				<< '[' << (this->whichArm == myo::armLeft ? "L" : "R") << ']'
				<< '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
		} else {
			std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
		}

		std::cout << std::flush;
	}
};

