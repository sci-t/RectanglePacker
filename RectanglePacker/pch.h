/******************************************************************************************************************************************
     * File: pch.h
     * Author: Lunev Arseniy (c) 2022
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include <regex>
#include <cmath>
#include <vector>
#include <array>
#include <memory>
#include <numeric>
#include <iostream>
#include <list>


struct Size
{
    int width;
    int height;
};

/**
 * \brief The template class for the points and vectors.
 * \details Note: It's possible to solve the problem using integer precision for the containers with a zero tilt angle.
*/
template <class T>
struct Point2D
{
    Point2D() : x(0), y(0) {}
    Point2D(T x, T y) : x(x), y(y) {}

    virtual ~Point2D() = default;
    Point2D(const Point2D&) = default;
    Point2D& operator=(const Point2D&) = default;
    Point2D(Point2D&&) = default;
    Point2D& operator=(Point2D&&) = default;

    T x;
    T y;

    /** \return The distance to the origin */
    double dist()
    {
        return pow(x * x + y * y, 0.5);
    }

    /** \return The normalized direction */
    Point2D dir()
    {
        return (*this) / dist();
    }

    friend Point2D operator+(const Point2D& p0, const Point2D& p1)
    {
        return { p0.x + p1.x, p0.y + p1.y };
    }
    friend Point2D operator-(const Point2D& p0, const Point2D& p1)
    {
        return  { p0.x - p1.x, p0.y - p1.y };
    }
    friend Point2D operator*(const Point2D& p, const T& c)
    {
        return { p.x * c, p.y * c };
    }
    friend Point2D operator*(const T& c, const Point2D& p)
    {
        return { p.x * c, p.y * c };
    }
    friend Point2D operator/(const Point2D& p, const T& c)
    {
        return  { p.x / c, p.y / c };
    }
};

/** \brief The template class for the shelf corners — the slots for placing of rectangles.*/
template <class T>
struct ShelfCorner2D
{
    using Vector = Point2D<T>;
    using Point = Point2D<T>;

    Vector hV;  // The vector along the height of placed rectangles
    Point p;    // The angular point
    Vector wV;  // The vector along the width of placed rectangles
};


// Let's set two identical type names for better readability
using Point = Point2D<double>;
using Vector = Point2D<double>;

using ShelfCorner = ShelfCorner2D<double>;

constexpr int MISSING_CONTAINER_ID = -1;  // default id
constexpr double POINT_PRECISION = 0.5;   // 0.5 for a pixel
constexpr int TIME_LIMIT = 1000;          // ms