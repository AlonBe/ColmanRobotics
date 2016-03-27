/*
 * main.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: colman
 */

#include <libplayerc++/playerc++.h>
#include <iostream>
#include <stdio.h>

using namespace PlayerCc;

int main() {

	PlayerClient pc("localhost", 6665);
	SonarProxy sp(&pc);
	Position2dProxy pp(&pc);

	pp.SetMotorEnable(true);
	while (true) {
		pc.Read();

		if (sp[2] < 0.7) {
			// If both right and left are not the best option
			if (sp[1] < 0.7 && sp[3] < 0.7) {
				pp.SetSpeed(-0.5, 3);
				std::cout << "1";
			}
			else if (sp[1] < sp[3]) {
				pp.SetSpeed(0, 0.5);
				std::cout << "2";
			}
			else {
				pp.SetSpeed(0, -0.5);
				std::cout << "3";
			}
		}
		else if (sp[1] < 0.7 || sp[0] < 0.4) {
			pp.SetSpeed(0.1, 0.3);
			std::cout << "4";
		}
		else if (sp[3] < 0.7 || sp[4] < 0.4) {
			pp.SetSpeed(0.1, -0.3);
			std::cout << "5";
		}
		else {
			pp.SetSpeed(0.5, 0);
			std::cout << "6";
		}
	}
	return 0;
}
