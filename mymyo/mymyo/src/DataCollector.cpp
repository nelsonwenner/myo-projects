#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <iomanip>
#include <array>
#include <stdexcept>
#include <string>
#include <algorithm>

#include <myo/myo.hpp>


class DataCollector : public myo::DeviceListener {
	private:
		std::array<int8_t, 8> data;
		int roll_w, pitch_w, yaw_w;
		myo::Pose currentPose;
		myo::Arm whichArm;
		bool isUnlocked;
		bool onArm;

	public: 
		DataCollector() {
			this->isUnlocked = false;
			this->roll_w = 0;
			this->pitch_w = 0;
			this->yaw_w = 0;
			this->onArm = false;
		}

		/* Overrider */
		void onUnpair(myo::Myo* myo, uint64_t timestamp) {
			this->isUnlocked = false;
			this->roll_w = 0;
			this->pitch_w = 0;
			this->yaw_w = 0;
			this->onArm = false;
		}

		/* Overrider */
		void onEmgData(myo::Myo *myo, uint64_t timestamp, const int8_t *msg) {
			for (int i=0; i < 8; i++) { this->data[i] = msg[i]; }
		}

		/* Overrider */
		void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
			using std::atan2;
			using std::asin;
			using std::sqrt;
			using std::max;
			using std::min;

			/* Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion */
			float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()), 1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
			float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
			float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
			
			/* Convert the floating point angles in radians to a scale from 0 to 18. */
			this->roll_w = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 18);
			this->pitch_w = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 18);
			this->yaw_w = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 18);
		}

		/* Overrider */
		void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
			this->currentPose = pose;

			if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
				/*Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the Myo becoming locked. */
				myo->unlock(myo::Myo::unlockHold);

				/* Notify the Myo that the pose has resulted in an action, in this case changing the text on the screen. The Myo will vibrate. */
				myo->notifyUserAction();
			}
			else {
				/* Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses are being performed, but lock after inactivity. */
				myo->unlock(myo::Myo::unlockTimed);
			}
		}

		/* Overrider */
		void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
			this->onArm = true;
			this->whichArm = arm;
		}

		/* Overrider */
		void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
			this->onArm = false;
		}

		/* Overrider */
		void onUnlock(myo::Myo* myo, uint64_t timestamp) {
			isUnlocked = true;
		}

		/* Overrider */
		void onLock(myo::Myo* myo, uint64_t timestamp) {
			isUnlocked = false;
		}
		
		void print() {
			std::cout << '\r';

			std::cout << '[' << std::string(this->roll_w, '*') << std::string(18 - this->roll_w, ' ') << ']'
					  << '[' << std::string(this->pitch_w, '*') << std::string(18 - this->pitch_w, ' ') << ']'
					  << '[' << std::string(this->yaw_w, '*') << std::string(18 - this->yaw_w, ' ') << ']';

			if (onArm) {
				
				std::string poseString = this->currentPose.toString();

				std::cout << '[' << (this->isUnlocked ? "unlocked" : "locked  ") << ']'
						  << '[' << (this->whichArm == myo::armLeft ? "L" : "R") << ']'
						  << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
			} else {
				/* Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on. */
				std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
			}

			std::cout << std::flush;
		}
};

