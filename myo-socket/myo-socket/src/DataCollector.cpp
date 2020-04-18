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
		
		if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {

			/*Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the Myo becoming locked. */
			myo->unlock(myo::Myo::unlockHold);

			/* Notify the Myo that the pose has resulted in an action, in this case changing the text on the screen. The Myo will vibrate. */
			myo->notifyUserAction();

			if (this->currentPose == myo::Pose::fingersSpread) {
				std::cout << "Left\n";
				this->clientSocket->sendFrame((char*)"left");
			} 
			else if (this->currentPose == myo::Pose::fist) {
				std::cout << "Right\n";
				this->clientSocket->sendFrame((char*)"right");
			}
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

