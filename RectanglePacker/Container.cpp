/******************************************************************************************************************************************
	 * File: Container.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "Container.h"


Container::Container()
	: containerId(MISSING_CONTAINER_ID),
	size({}),
	isEmpty(true),
	tiltAngle(0)
{
	ShelfCorner initialShelfCorner = { Vector(0, 1), Point(0, 0), Vector(1, 0) };
	mainShelfSlots.push_back(initialShelfCorner);
}

Container::Container(int containerId, const Size& size)
	: containerId(containerId),
	size(size),
	isEmpty(true),
	tiltAngle(0)
{
	ShelfCorner initialShelfCorner = { Vector(0, 1), Point(0, 0), Vector(1, 0) };
	mainShelfSlots.push_back(initialShelfCorner);
}

Container::~Container()
{
}

int Container::getContainerId()
{
	return containerId;
}

bool Container::getIsEmpty()
{
	return isEmpty;
}

bool Container::placeFirst(std::shared_ptr<Rectangle> rectangle)
{
	const Size& rSize = rectangle->getSize();
	double rH = rSize.height;
	double rW = rSize.width;

	double h = this->size.height;
	double w = this->size.width;

	if (rW < rH || w < h) {
		std::cerr << "It is assumed that the width is equal or greater than the height at the start.\n";
		return false;
	}

	if (rH > h || (rH > w)) {
		Size s = rectangle->getSize();
		std::clog << "The rectangle  " << s.width << " x " << s.height << "  is ignored.\n";
		return false;
	}

	if (rW > w) {
		// rH * sin(alpha) + rW * cos(alpha) == w, where alpha is the tilt angle of the placed rectangle
		double a = pow(rH, 2) + pow(rW, 2);
		double d = pow(a - pow(w, 2), 0.5);
		double tiltAngleSin = (w * rH + rW * d) / a;
		double tiltAngleCos = (w * rW - rH * d) / a;

		double p2y = rW * tiltAngleSin + rH * tiltAngleCos;
		if (p2y > h) {
			return false;  // In this case at least one rectangle corner is outside the container.
		}
		tiltAngle = acos(tiltAngleCos);

		Point p0 = { rH * tiltAngleSin, 0 };
		Point p1 = { 0, rH * tiltAngleCos };
		Point p2 = { w - rH * tiltAngleSin, p2y };
		Point p3 = { w, rW * tiltAngleSin };

		std::array<Point, 4> rectangleCorners = { p0, p1, p2, p3 };
		rectangle->placeToPosition(rectangleCorners, containerId);

		mainShelfSlots[0] = { {0, 1}, p1, p2 - p1 };
		tiltShelfSlots.push_back({ {0, -1}, p3, p0 - p3 });
	}
	else {
		rectangle->placeToPosition(containerId);
		mainShelfSlots[0] = { {0, 1}, {0, rH}, {1, 0} };
		mainShelfSlots.push_back({ {0, 1}, rectangle->getCorners()[3], { 1, 0 } });
	}
	
	isEmpty = false;
	placedRectangles.push_back(rectangle);
	return true;
}

bool Container::placeNext(std::shared_ptr<Rectangle> rectangle)
{
	for (auto it = mainShelfSlots.begin(), itEnd = mainShelfSlots.end(); it != itEnd; ++it) {
		if (rectangle->placeToShelf(*it, size, containerId, true, tiltAngle)) {
			Point p1 = rectangle->getCorners()[1];
			Point p2 = rectangle->getCorners()[2];
			Point p3 = rectangle->getCorners()[3];
			(*it) = { {0, 1}, p1, p2 - p1 };
			if (it == mainShelfSlots.end() - 1) {
				mainShelfSlots.push_back({ {0, 1}, p3, p2 - p1 });
			}
			placedRectangles.push_back(rectangle);
			return true;
		}

	}

	for (auto it = tiltShelfSlots.begin(), itEnd = tiltShelfSlots.end(); it != itEnd; ++it) {
		if (rectangle->placeToShelf(*it, size, containerId, false, tiltAngle)) {
			Point p1 = rectangle->getCorners()[1];
			Point p2 = rectangle->getCorners()[2];
			Point p3 = rectangle->getCorners()[3];
			(*it) = { {0, -1}, p1, p2 - p1 };
			if (it == tiltShelfSlots.end() - 1) {
				tiltShelfSlots.push_back({ {0, -1}, p3, p2 - p1 });
			}
			placedRectangles.push_back(rectangle);
			return true;
		}
	}

	return false;
}

bool Container::placeOpposite(std::shared_ptr<Rectangle> rectangle)
{
	for (auto it = oppositeShelfSlots.rbegin(); it != oppositeShelfSlots.rend(); ++it) {
		if (rectangle->placeToShelf(*it, size, containerId, placedRectangles)) {
			// (*it) = { {0, -1}, {(*it).p.x - double(rectangle->getSize().width), (*it).p.y}, {-1, 0} };
			(*it).p.x = (*it).p.x - double(rectangle->getSize().width);
			placedRectangles.push_back(rectangle);
			return true;
		}
	}

	return false;
}

const std::list<std::shared_ptr<Rectangle> >& Container::getPlacedRectangles()
{
	return placedRectangles;
}

void Container::updateOppositeShelfSlots()
{
	oppositeShelfSlots.resize(mainShelfSlots.size());

	for (auto itM = mainShelfSlots.begin() + 1, itO = oppositeShelfSlots.begin(),
		itMEnd = mainShelfSlots.end(); itM != itMEnd; ++itM, ++itO) {

		(*itO) = { {0, -1}, {(*itM).p.x, double(size.height)}, {-1, 0} };
	}
	*(oppositeShelfSlots.end() - 1) = { 
		{0, -1}, 
		{double(size.width), double(size.height)}, 
		{-1, 0} 
	};
}

void Container::excludeRectangle(std::shared_ptr<Rectangle> rectangle)
{
	placedRectangles.remove(rectangle);
}

const Size& Container::getSize()
{
	return size;
}

void Container::setSize(const Size& size)
{
	this->size = size;
}