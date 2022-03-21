/******************************************************************************************************************************************
	 * File: Rectangle.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "Rectangle.h"


Rectangle::Rectangle()
	: size({ 0, 0 }),
	containerId(MISSING_CONTAINER_ID),
	corners({})
{
}

Rectangle::Rectangle(const Size& size)
	: size(size),
	containerId(MISSING_CONTAINER_ID)
{
	placeByDefault();
}

Rectangle::~Rectangle()
{
}

void Rectangle::placeByDefault()
{
	if (size.width < size.height) {
		flip();
	}
	corners[0] = { 0, 0 };
	corners[1] = { 0, double(size.height) };
	corners[2] = { double(size.width), double(size.height) };
	corners[3] = { double(size.width), 0 };
}

void Rectangle::flip()
{
	std::swap(size.width, size.height);
}

bool Rectangle::isIntersect(std::shared_ptr<Rectangle> rect0,
	const std::array<Point, 4>& currentCorners)
{
	std::array<Point, 4> corners0 = rect0->getCorners();

	// Check if some corners are inside anothoer rectangle
	for (auto& corner : corners0) {
		if (isInside(corner, currentCorners, -POINT_PRECISION)) {
			return true;
		}
	}
	for (auto& corner : currentCorners) {
		if (isInside(corner, corners0, -POINT_PRECISION)) {
			return true;
		}
	}

	// Additional cases when some edges coincide:
	Point center = (currentCorners[0] + currentCorners[1] + currentCorners[2] + currentCorners[3]) / 4.0;
	if (isInside(center, corners0, -POINT_PRECISION)) {
		return true;
	}
	Point center0 = (corners0[0] + corners0[1] + corners0[2] + corners0[3]) / 4.0;
	if (isInside(center0, currentCorners, -POINT_PRECISION)) {
		return true;
	}
	
	return false;
}

void Rectangle::rotate(const Point& rotCenter, double angle)
{
	for (auto& v : corners) {
		v = v - rotCenter;
		v = { v.x * cos(angle) - v.y * sin(angle),
			  v.x * sin(angle) + v.y * cos(angle) };
		v = v + rotCenter;
	}
}

void Rectangle::setSize(const Size& size)
{
	this->size = size;
	placeByDefault();
}

const std::array<Point, 4>& Rectangle::getCorners()
{
	return corners;
}

int Rectangle::getContainerId()
{
	return containerId;
}

const Size& Rectangle::getSize()
{
	return size;
}

/** Place the rectangle to the shelf corner if it's possible. */
bool Rectangle::placeToShelf(const ShelfCorner& shelfCorner, const Size& containerSize,
	int containerId, bool isMainShelfs, double tiltAngle)
{
	std::array<Point, 4> currentCorners;

	const double& h = size.height;
	const double& w = size.width;

	const auto& p = shelfCorner.p;
	const auto& hDir = Vector(shelfCorner.hV).dir();
	const auto& wDir = Vector(shelfCorner.wV).dir();


	if (tiltAngle < DBL_MIN) {
		currentCorners[0] = p;
		currentCorners[1] = p + hDir * h;
		currentCorners[2] = p + wDir * w + hDir * h;
		currentCorners[3] = p + wDir * w;
	}
	else {
		currentCorners[0] = p + wDir * h * tan(tiltAngle);
		currentCorners[1] = p + hDir * h / cos(tiltAngle);
		currentCorners[2] = p + wDir * w + hDir * h / cos(tiltAngle);
		currentCorners[3] = p + wDir * (w + h * tan(tiltAngle));
	}

	if (!checkIfInside(containerSize, currentCorners, isMainShelfs)) {
		return false;
	}

	placeToPosition(currentCorners, containerId);

	return true;
}

/** Place the rectangle to the shelf corner if it's possible. */
bool Rectangle::placeToShelf(const ShelfCorner& shelfCorner, const Size& containerSize, int containerId,
	const std::list<std::shared_ptr<Rectangle> >& placedRectangles)
{
	std::array<Point, 4> currentCorners;

	const double& h = size.height;
	const double& w = size.width;

	const auto& p = shelfCorner.p;
	const auto& hDir = Vector(shelfCorner.hV).dir();
	const auto& wDir = Vector(shelfCorner.wV).dir();

	currentCorners[0] = p;
	currentCorners[1] = p + hDir * h;
	currentCorners[2] = p + wDir * w + hDir * h;
	currentCorners[3] = p + wDir * w;


	for (auto& c : currentCorners) {
		std::array<Point, 4> containerCorners = {
			Point({0, 0}),
			Point({0, double(containerSize.height)}),
			Point({double(containerSize.width), double(containerSize.height)}),
			Point({double(containerSize.width), 0})
		};
		if (!isInside(containerCorners, currentCorners)) {
			return false;
		}
	}

	for (auto r : placedRectangles) {
		if (isIntersect(r, currentCorners)) {
			return false;
		}
	}

	placeToPosition(currentCorners, containerId);

	return true;
}

bool Rectangle::checkIfInside(const Size& containerSize,
	const std::array<Point, 4>& currentCorners, bool isMainShelfs)
{
	if (!isMainShelfs) {
		return currentCorners[2].y > -POINT_PRECISION && currentCorners[3].x > -POINT_PRECISION;
	}
	
	return currentCorners[2].y < double(containerSize.height) + POINT_PRECISION
		&& currentCorners[3].x < double(containerSize.width) + POINT_PRECISION;
}

bool Rectangle::isInside(const Point& point, const std::array<Point, 4>& rectangleCorners,
	double precision)
{
	double sign0 = computeSideSign(point, rectangleCorners[0], rectangleCorners[1]);
	double sign1 = computeSideSign(point, rectangleCorners[1], rectangleCorners[2]);
	double sign2 = computeSideSign(point, rectangleCorners[2], rectangleCorners[3]);
	double sign3 = computeSideSign(point, rectangleCorners[3], rectangleCorners[0]);
	if ( (sign0 <  precision && sign1 <  precision && sign2 <  precision && sign3 <  precision) ||
		 (sign0 > -precision && sign1 > -precision && sign2 > -precision && sign3 > -precision)    ) {
		return true;
	}
	return false;
}

bool Rectangle::isInside(const std::array<Point, 4>& containerCorners,
	const std::array<Point, 4>& rectangleCorners)
{
	for (auto &p : rectangleCorners) {
		if (!isInside(p, containerCorners, POINT_PRECISION)) {
			return false;
		}
	}
	return true;
}

double Rectangle::computeSideSign(const Point& p, const Point& someCorner, const Point& nextCorner)
{
	return (nextCorner.x - someCorner.x) * (p.y - someCorner.y) - 
		(nextCorner.y - someCorner.y) * (p.x - someCorner.x);
}

void Rectangle::placeToPosition(int containerId)
{
	this->containerId = containerId;
}

void Rectangle::placeToPosition(const std::array<Point, 4>& corners, int containerId)
{
	this->corners = corners;
	this->containerId = containerId;
}
