#include "fssimplewindow.h"
#include "DrawingUtilNG.h"
#include "Params.h"
#include "Road.h"
#include "Car.h"
#include "Prop.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include <chrono>

int main(void) {
	Road road;
	Car playerCar;
	Prop prop;

	YsSoundPlayer effectSoundPlayer;
	YsSoundPlayer::SoundData accSound;
	YsSoundPlayer::SoundData deaccSound;

	YsSoundPlayer backgroundSoundPlayer;
	YsSoundPlayer::SoundData backgroundMusic;

	YsRawPngDecoder png;

	//road.setPlayer(playerCar);
	prop.setPlayer(playerCar);

	FsOpenWindow(16, 16, 600, 800, 1);
	FsPollDevice();

	int i = 0;
	int nCar = 1;

	bool car1Exist = false;
	bool car2Exist = false;

	srand((unsigned)time(NULL));

	int propCount = 0;
	int moveCount = 0;
	Car car1;
	Car car2;

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	bool letsRock = false;

	if (YSOK == accSound.LoadWav("acc.wav")
		&& YSOK == deaccSound.LoadWav("deacc.wav")
		&& YSOK == backgroundMusic.LoadWav("Highway_to_Hell.wav")
		&& YSOK == png.Decode("ACDC.png")) {
		png.Flip();
		effectSoundPlayer.Start();
		backgroundSoundPlayer.Start();
		backgroundSoundPlayer.PlayOneShot(backgroundMusic);

		while (i <= 8000) {
			end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;
			std::cout << elapsed_seconds.count() << std::endl;
			if (elapsed_seconds.count() > 53 && elapsed_seconds.count() < 68) {
				letsRock = true;
			}
			else {
				letsRock = false;
			}

			FsPollDevice();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			road.paint();
			playerCar.paint();
			prop.paint();

			glColor3ub(255, 0, 0);
			glRasterPos2i(150, 100);  // sets position
			YsGlDrawFontBitmap20x32("High Way to Hell"); // there are other font sizes

			// paint speed text
			string velY = "Your speed: " + to_string(playerCar.getCurVel().y * 200);
			glColor3ub(0, 0, 255);
			glRasterPos2i(150, 150);
			YsGlDrawFontBitmap16x24(velY.c_str());

			// if the number of cars <= 3, and no prop is effective, then maybe generate a car
				// the generated car is always slower than the player
			if (nCar <= 3 && !prop.accIsEffective() && !prop.deaccIsEffective() && !car1Exist) {
				float randNumGenCar = (float)rand() / RAND_MAX;
				if (randNumGenCar > 0.99) { // generate car1, always on the left
					car1.setColor((float)rand() / RAND_MAX * 256, (float)rand() / RAND_MAX * 256, (float)rand() / RAND_MAX * 256);
					car1.setCurPos(145, -car1.length / 2);

					car1.setCurVel(0, (float)rand() / RAND_MAX * 0.2 + 0.2);
					car1Exist = true;
					nCar = nCar + 1;
				}
			}
			if (nCar <= 3 && !prop.accIsEffective() && !prop.deaccIsEffective() && !car2Exist) {
				float randNumGenCar = (float)rand() / RAND_MAX;
				if (randNumGenCar > 0.99) { // generate car2, always on the right
					car2.setColor((float)rand() / RAND_MAX * 256, (float)rand() / RAND_MAX * 256, (float)rand() / RAND_MAX * 256);
					car2.setCurPos(600 - 145, -car2.length / 2);

					car2.setCurVel(0, (float)rand() / RAND_MAX * 0.2 + 0.2);
					car2Exist = true;
					nCar = nCar + 1;
				}
			}

			// update car1 state
			if (car1Exist) {
				car1.paint();
				car1.setCurPos(car1.getCurPos().x, car1.getCurPos().y - car1.getCurVel().y * DT + playerCar.getCurVel().y * DT);
				if (car1.getCurPos().y - car1.length / 2 >= WINDOW_Y_SIZE) {
					car1Exist = false;
					nCar = nCar - 1;
				}
			}

			// update car2 state
			if (car2Exist) {
				car2.paint();
				car2.setCurPos(car2.getCurPos().x, car2.getCurPos().y - car2.getCurVel().y * DT + playerCar.getCurVel().y * DT);
				if (car2.getCurPos().y - car2.length / 2 >= WINDOW_Y_SIZE) {
					car2Exist = false;
					nCar = nCar - 1;
				}
			}

			if (letsRock == false) {
				// if the playerCar hit the accel prop, the playerCar will accelerate, then decelerate
				bool hitAccProp = prop.hitAccProp();
				if (hitAccProp) {
					effectSoundPlayer.PlayOneShot(accSound);
				}
				if (prop.accIsEffective()) {
					if (propCount <= 200) { // accelerate
						playerCar.accel({ 0, 0.00025 });
						road.playerCarVelY = playerCar.getCurVel().y;
						propCount++;
					}
					else if (propCount > 200 && propCount <= 400) { // then deaccelerate
						if (playerCar.getCurVel().y > 0.5) {
							playerCar.accel({ 0, -0.00025 });
						}
						else {
							playerCar.setCurVel(DEFAULT_VEL_X, DEFAULT_VEL_Y);
							road.playerCarVelY = playerCar.getCurVel().y;
						}

						road.playerCarVelY = playerCar.getCurVel().y;
						propCount++;
					}
					else {
						playerCar.setCurVel(DEFAULT_VEL_X, DEFAULT_VEL_Y);
						road.playerCarVelY = playerCar.getCurVel().y;
						propCount = 0;
						std::cout << "acc stoped" << std::endl;
						prop.stopPropEffect();
						prop.resetProp();
					}
				}

				// if the playerCar hit the deacc prop
				//bool hitDeaccProp = prop.hitDeaccProp(); // check whether the prop is hitted
				//if (hitDeaccProp) {
				//	effectSoundPlayer.PlayOneShot(deaccSound);
				//}
				//if (prop.deaccIsEffective()) {
				//	if (propCount <= 200) {
				//		playerCar.accel({ 0, -0.00025 });
				//		road.playerCarVelY = playerCar.getCurVel().y;
				//		propCount++;
				//	}
				//	else if (propCount > 200 && propCount <= 400) {
				//		playerCar.accel({ 0, 0.00025 });
				//		road.playerCarVelY = playerCar.getCurVel().y;
				//		propCount++;
				//	}
				//	else {
				//		playerCar.setCurVel(DEFAULT_VEL_X, DEFAULT_VEL_Y);
				//		road.playerCarVelY = playerCar.getCurVel().y;
				//		propCount = 0;
				//		std::cout << "deacc stoped" << std::endl;
				//		prop.stopPropEffect();
				//		prop.resetProp();
				//	}
				//}

				

				// if the car is going to hit car1
				if (car1Exist
					&& playerCar.getCurPos().x == 145
					&& playerCar.getCurPos().y - car1.getCurPos().y <= 100
					&& playerCar.getCurPos().y > car1.getCurPos().y
					&& !playerCar.haveTraj) {

					// first, deaccelerate!
					if (playerCar.getCurVel().y > car1.getCurVel().y) {
						playerCar.setCurVel(0, car1.getCurVel().y);
						road.playerCarVelY = playerCar.getCurVel().y;
						prop.stopPropEffect();
					}
					// then, if the player is slow enough, we decide whether the player need to move
					else {
						// if there is no car 2, we move directly
						if (!car2Exist) {
							playerCar.genMoveTraj({ 600 - 145, 600 }, 1000);
							playerCar.haveTraj = true;
						}
						// if car 2 exists, we need to make a decision
						else {
							if (abs(car2.getCurPos().y - car2.getCurVel().y * 1000 + playerCar.getCurVel().y * 1000 - DEFAULT_START_Y) >= 100) {
								playerCar.genMoveTraj({ 600 - 145, 600 }, 1000);
								playerCar.haveTraj = true;
							}
						}
					}
				}

				// if the car is going to hit car2
				if (car2Exist
					&& playerCar.getCurPos().x == 600 - 145
					&& playerCar.getCurPos().y - car2.getCurPos().y <= 100
					&& playerCar.getCurPos().y > car2.getCurPos().y
					&& !playerCar.haveTraj) {

					// first, deaccelerate!
					if (playerCar.getCurVel().y > car2.getCurVel().y) {
						playerCar.setCurVel(0, car2.getCurVel().y);
						road.playerCarVelY = playerCar.getCurVel().y;
						prop.stopPropEffect();
					}
					// then, if the player is slow enough, we decide whether the player need to move
					else {
						// if there is no car 1, we move directly
						if (!car1Exist) {
							playerCar.genMoveTraj({ 145, 600 }, 1000);
							playerCar.haveTraj = true;
						}
						// if car 1 exists, we need to make a decision
						else {
							if (abs(car1.getCurPos().y - car1.getCurVel().y * 1000 + playerCar.getCurVel().y * 1000 - DEFAULT_START_Y) >= 100) {
								playerCar.genMoveTraj({ 145, 600 }, 1000);
								playerCar.haveTraj = true;
							}
						}
					}
				}

				// excute the traj
				if (playerCar.haveTraj) {
					playerCar.setCurPos(playerCar.xTraj[moveCount], playerCar.getCurPos().y);
					moveCount++;
					if (moveCount == playerCar.xTraj.size()) {
						moveCount = 0;
						playerCar.xTraj.clear();
						playerCar.haveTraj = false;
						playerCar.setCurVel(0, DEFAULT_VEL_Y); // after moving, let the player reset the velocity
						road.playerCarVelY = playerCar.getCurVel().y;
					}
				}
			}
			if (letsRock == true) {
				playerCar.setCurVel(0, 1.5);
				road.playerCarVelY = playerCar.getCurVel().y;
				
				glColor3ub(255, 0, 0);
				glRasterPos2i(300, 400);
				YsGlDrawFontBitmap20x32("LET'S ROCK!!!");
			}

			// paint ACDC logo
			glRasterPos2d(250, 60);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba); // bad way

			// paint trees
			glColor3ub(0, 200, 0);

			for (int i = 0; i <= 850 / 50; i++) {
				DrawingUtilNG::drawCircle(0, 50 * (i - 1), 50, 50, true);
			}

			for (int i = 0; i <= 850 / 50; i++) {
				DrawingUtilNG::drawCircle(600, 50 * (i - 1), 50, 50, true);
			}

			if (prop.accIsEffective()) {
				glColor3ub(255, 255, 255);
				glRasterPos2i(150, 300);
				YsGlDrawFontBitmap16x24("Accelerate!");
			}
			if (prop.deaccIsEffective()) {
				glColor3ub(255, 255, 255);
				glRasterPos2i(150, 300);
				YsGlDrawFontBitmap16x24("Deaccelerate!");
			}

			if (i > 7000 && i < 8000) {
				float R = (i - 7000) * (-255) / 1000 + 255;
				glColor3ub(R, 0, 0);
				DrawingUtilNG::drawRectangle(0, 0, WINDOW_X_SIZE, 0.8 * (i - 7000), true);
			}

			//if (i > 8000) {
			//	glColor3ub(0, 0, 0);
			//	DrawingUtilNG::drawRectangle(0, 0, WINDOW_X_SIZE, WINDOW_Y_SIZE, true);
			//	glColor3ub(255, 0, 0);
			//	YsGlDrawFontBitmap20x32("You have reached hell!"); // there are other font sizes
			//}

			FsSwapBuffers();
			FsSleep(DT);

			i++;
		}

		effectSoundPlayer.End();
	}

	return 0;
}