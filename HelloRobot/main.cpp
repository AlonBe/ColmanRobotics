/*
* main.cpp
*
* Alon Berkman 302444252
* Talya Ben Haim 205412570
*
*/

#include <libplayerc++/playerc++.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace PlayerCc;

int main()
{
	PlayerClient pc("localhost", 6665);
	SonarProxy sp(&pc);
	Position2dProxy pp(&pc);
	pp.SetMotorEnable(true);

	while (true)
	{
		double distance, angle;

		pc.Read();

		if (sp[2] < 0.7)
		{
			angle = 0;
			distance = sp[2];

			// If both right and left are not the best option
			if (sp[1] < 0.7 && sp[3] < 0.7)
			{
				pp.SetSpeed(-0.5, 3);
			}
			else if (sp[1] < sp[3])
			{
				pp.SetSpeed(0, 0.5);
			}
			else
			{
				pp.SetSpeed(0, -0.5);
			}
		}
		else if (sp[1] < 0.7)
		{
			distance = sp[1];
			angle = 25;

			pp.SetSpeed(0.1, 0.3);
		}
		else if(sp[0] < 0.4)
		{
			distance = sp[1];
			angle = 90;

		    pp.SetSpeed(0.1, 0.3);
		}
		else if (sp[3] < 0.7)
		{
			distance = sp[3];
			angle = -25;

			pp.SetSpeed(0.1, -0.3);
			std::cout << "5";
		}
		else if(sp[4] < 0.4)
		{
			distance = sp[3];
			angle = -90;

			pp.SetSpeed(0.1, -0.3);
		}
		else
		{
			distance = 0.8;
			pp.SetSpeed(0.5, 0);
		}

		double positionX = pp.GetXPos() + distance * cos(pp.GetYaw() + angle);
		double positionY = pp.GetYPos() + distance * sin(pp.GetYaw() + angle);

		if (distance != 0.8)
		{
			std::cout << "Obstacle location:(" << positionX << "," << positionY << ")\n";
		}
	}

	return 0;
}
