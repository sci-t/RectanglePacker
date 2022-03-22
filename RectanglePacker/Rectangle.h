/******************************************************************************************************************************************
	 * File: Rectangle.h
	 * Author: Lunåv Arseniy (c) 2022
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include "pch.h"


class Rectangle
{
public:
	Rectangle();
	Rectangle(const Size& size);

	virtual ~Rectangle() = default;
	Rectangle(const Rectangle&) = default;
	Rectangle(Rectangle&&) = default;
	Rectangle& operator=(const Rectangle&) = default;
	Rectangle& operator=(Rectangle&&) = default;

	const Size& getSize();
	void setSize(const Size& size);
	const std::array<Point, 4>& getCorners();
	int getContainerId();

	bool placeToShelf(const ShelfCorner& shelfCorner, const Size& containerSize, int containerId,
		bool isMainShelfs, double tiltAngle);
	bool placeToShelf(const ShelfCorner& shelfCorner, const Size& containerSize, int containerId,
	const std::list<std::shared_ptr<Rectangle> >& placedRectangles);

	void placeToPosition(int containerId);
	void placeToPosition(const std::array<Point, 4>& corners, int containerId);
	void placeByDefault();

	void flip();
	bool isIntersect(std::shared_ptr<Rectangle> rect, const std::array<Point, 4>& currentCorners);

private:

	Size m_size;
	int m_containerId;

	std::array<Point, 4> m_corners;

	void rotate(const Point& rotCenter, double angle);

    static bool checkIfInside(const Size& containerSize, const std::array<Point, 4>& currentCorners,
                              bool isMainShelfs);

	bool isInside(const Point& point, const std::array<Point, 4>& rectangleCorners,
		double precision);
	bool isInside(const std::array<Point, 4>& containerCorners,
		const std::array<Point, 4>& rectangleCorners);

    static double computeSideSign(const Point& p, const Point& someCorner, const Point& nextCorner);
};
