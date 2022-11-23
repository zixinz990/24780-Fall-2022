#include <iostream>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "StringPlus.h"
#include "DrawingUtilNG.h"
#include "Track2D.h"
#include "StringPlus.h"
#include "yspng.h"
#include "SlideBox.h"

const double GRAVITY = 9.80665;  // this implies units are meters and seconds

SlideBox::SlideBox()
{
	// some default parameters for quick testing
	props["width"] = 2.;
	props["height"] = 1.5;
	props["mass"] = 3;   // actually does not affect calculations (cancels out)
	props["mu"] = 0.1;   // coeff of friction b/w box and track
	props["initDist"] = 0;  // at start of slide
	props["initVel"] = 0;   // starts from rest
	props["initAccel"] = 0; // no push

	theTrack = nullptr;
	reset();

	// load texture data
	YsRawPngDecoder pngTemp;
	woodTexture = pngTemp.Decode("wood texture.png");

	glGenTextures(1, &woodTexture); // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, woodTexture); // Making the texture identifier current (or bring it to the deck)

	// set up parameters for the current texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,  // Level of detail
		GL_RGBA,  // the "A" in RGBA will include the transparency
		pngTemp.wid,  // the hippos width and height
		pngTemp.hei,
		0,  // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pngTemp.rgba);
}

void SlideBox::loadFromConsole()
{
	using namespace std;

	cout << endl << "Enter parameters for adding a slide box to model:" << endl;
	props["width"] = StringPlus::getDouble(cin, "                               Box width (m) >> ");
	props["height"] = StringPlus::getDouble(cin, "                              Box height (m) >> ");
	props["initDist"] = StringPlus::getDouble(cin, "             Inital position along track (m) >> ");

	// add more stuff . . .
	reset();
}

void SlideBox::move(double deltaT)
{
	if (theTrack != nullptr && theTrack->getLength() > 0) {
		// change distance, velocity, and acceleration according to equations of motion
		// update acceleration
		float currAngle = theTrack->getAngle(currDist);
		currAccel = -GRAVITY * sin(currAngle * atan(1.0) / 45.0);
		// update velocity
		currVel = currVel + currAccel * deltaT;
		// update distance
		currDist = currDist + currVel * deltaT;
		if (currDist > theTrack->getLength()) {
			reset();
		}
	}
}

void SlideBox::paint()
{
	// use transformations to paint the box at it's current position and angle
	float currAngle = theTrack->getAngle(currDist);  // uncomment after implemant getAngle
	Point2D currPosition = theTrack->getCoords(currDist);  // uncomment after implement getCoords

	float wid = props["width"];
	float hei = props["height"];

	glTranslatef(currPosition.x, currPosition.y, 0);
	glRotatef(currAngle, 0, 0, 1); // around z axis

	// color is taken care of elsewhere
	DrawingUtilNG::drawRectangle(-wid / 2., 0., wid, hei, true);

	//// do the texture thing here
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//// enable textures
	//glEnable(GL_TEXTURE_2D);

	//// invoke the texture ID
	//glBindTexture(GL_TEXTURE_2D, woodTexture);
	//// set color "tint"
	//glColor4d(100.0, 100.0, 100.0, 0.0);

	glBegin(GL_QUADS);

	// For each vertex, assign texture coordinate before vertex coordinate.
	glTexCoord2d(0.0, 0.0); // these are "percentage" of the image
	glVertex2f(-wid / 2, hei);   // these are actual pixel coordinates on screen

	glTexCoord2d(1.0, 0.0);
	glVertex2f(wid / 2, hei);

	glTexCoord2d(1.0, 1.0);
	glVertex2f(wid / 2, 0); // kept xSize and ySize for this purpose

	glTexCoord2d(0.0, 1.0);
	glVertex2f(-wid / 2, 0);

	glEnd();

	// disbale textures
	//glDisable(GL_TEXTURE_2D);

	glRotatef(-currAngle, 0, 0, 1);
	glTranslatef(-currPosition.x, -currPosition.y, 0);
}

std::ostream& operator<<(std::ostream& os, const SlideBox& aBox)
{
	auto oldPrecision = os.precision();
	os.precision(3);

	std::string spacer;
	if (&os == &std::cout)
		spacer = ", "; // if outputting to console, use commas and keep on same line
	else
		spacer = "\n"; // if outputting to file, put each property on separate line

	// print properties in prescribed order
	std::vector<std::string> printOrder =
	{ "width", "height", "mu", "mass", "initDist", "initVel", "initAccel" };

	for (int i = 0; i < aBox.props.size(); i++) {
		if (aBox.props.find(printOrder[i]) != aBox.props.end()) { // if property is in map

			if (i > 0) os << spacer;
			os << printOrder[i] << "=" << aBox.props.at(printOrder[i]);// [] doesn't work here
		}
	}

	// print properties in alphabetical order (directly from map)
	//for (auto& item : aBox.props) {
	//  if (i>0) os << spacer;
	//	os << item.first << "=" << item.second;
	//}

	os.precision(oldPrecision);
	return os;
}
