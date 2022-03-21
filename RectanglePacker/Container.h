/******************************************************************************************************************************************
	 * File: Container.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once

#include "pch.h"
#include "Rectangle.h"


class Container
{
public:
	Container();
	Container(int containerId, const Size& size);
	~Container();

	const Size& getSize();
	void setSize(const Size& size);

	int getContainerId();
	bool getIsEmpty();

	bool placeFirst(std::shared_ptr<Rectangle> rectangle);
	bool placeNext(std::shared_ptr<Rectangle> rectangle);
	bool placeOpposite(std::shared_ptr<Rectangle> rectangle);

	const std::list<std::shared_ptr<Rectangle> >& getPlacedRectangles();
	void updateOppositeShelfSlots();
	void excludeRectangle(std::shared_ptr<Rectangle> rectangle);

private:
	int containerId;
	Size size;
	bool isEmpty;
	double tiltAngle;

	std::list<std::shared_ptr<Rectangle> > placedRectangles;
	std::vector<ShelfCorner> mainShelfSlots;     // main slots for placing a rectangle
	std::vector<ShelfCorner> oppositeShelfSlots; // slots at main shelfs from the opposite side
	std::vector<ShelfCorner> tiltShelfSlots;     // additional slots for placing a rectangle in case of a non-zero tilt angle
};
