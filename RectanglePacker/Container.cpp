/******************************************************************************************************************************************
     * File: Container.h
     * Author: Lunev Arseniy (c) 2022
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "Container.h"
#include "Rectangle.h"


Container::Container()
    : m_containerId(MISSING_CONTAINER_ID),
    m_size({}),
    m_isEmpty(true),
    m_tiltAngle(0)
{
    const ShelfCorner initialShelfCorner = { Vector(0, 1), Point(0, 0), Vector(1, 0) };
    m_mainShelfSlots.push_back(initialShelfCorner);
}

Container::Container(int containerId, const Size& size)
    : m_containerId(containerId),
    m_size(size),
    m_isEmpty(true),
    m_tiltAngle(0)
{
    const ShelfCorner initialShelfCorner = { Vector(0, 1), Point(0, 0), Vector(1, 0) };
    m_mainShelfSlots.push_back(initialShelfCorner);
}

int Container::getContainerId() const
{
    return m_containerId;
}

bool Container::getIsEmpty() const
{
    return m_isEmpty;
}

bool Container::placeFirst(const std::shared_ptr<Rectangle>& rectangle)
{
    const Size& rSize = rectangle->getSize();
    double rH = rSize.height;
    const double rW = rSize.width;

    const double h = this->m_size.height;
    const double w = this->m_size.width;

    if (rW < rH || w < h) {
        std::cerr << "It is assumed that the width is equal or greater than the height at the start.\n";
        return false;
    }

    if (rH > h || (rH > w)) {
        std::clog << "The rectangle  " << rSize.width << " x " << rSize.height << "  is ignored.\n";
        return false;
    }

    if (rW > w) {
        // rH * sin(alpha) + rW * cos(alpha) == w, where alpha is the tilt angle of the placed rectangle
        const double a = pow(rH, 2) + pow(rW, 2);
        const double d = pow(a - pow(w, 2), 0.5);
        const double tiltAngleSin = (w * rH + rW * d) / a;
        const double tiltAngleCos = (w * rW - rH * d) / a;

        const double p2y = rW * tiltAngleSin + rH * tiltAngleCos;
        if (p2y > h) {
            std::clog << "The rectangle  " << rSize.width << " x " << rSize.height << "  is ignored.\n";
            return false;  // In this case at least one rectangle corner is outside the container.
        }
        m_tiltAngle = acos(tiltAngleCos);

        const Point p0 = { rH * tiltAngleSin, 0 };
        const Point p1 = { 0, rH * tiltAngleCos };
        const Point p2 = { w - rH * tiltAngleSin, p2y };
        const Point p3 = { w, rW * tiltAngleSin };

        const std::array<Point, 4> rectangleCorners = { p0, p1, p2, p3 };
        rectangle->placeToPosition(rectangleCorners, m_containerId);

        m_mainShelfSlots[0] = { {0, 1}, p1, p2 - p1 };
        m_tiltShelfSlots.push_back({ {0, -1}, p3, p0 - p3 });
    }
    else {
        rectangle->placeToPosition(m_containerId);
        m_mainShelfSlots[0] = { {0, 1}, {0, rH}, {1, 0} };
        m_mainShelfSlots.push_back({ {0, 1}, rectangle->getCorners()[3], { 1, 0 } });
    }

    m_isEmpty = false;
    m_placedRectangles.push_back(rectangle);
    return true;
}

bool Container::placeNext(const std::shared_ptr<Rectangle>& rectangle)
{
    for (auto it = m_mainShelfSlots.begin(), itEnd = m_mainShelfSlots.end(); it != itEnd; ++it) {
        if (rectangle->placeToShelf(*it, m_size, m_containerId, true, m_tiltAngle)) {
            const Point p1 = rectangle->getCorners()[1];
            const Point p2 = rectangle->getCorners()[2];
            const Point p3 = rectangle->getCorners()[3];
            (*it) = { {0, 1}, p1, p2 - p1 };
            if (it == m_mainShelfSlots.end() - 1) {
                m_mainShelfSlots.push_back({ {0, 1}, p3, p2 - p1 });
            }
            m_placedRectangles.push_back(rectangle);
            return true;
        }

    }

    for (auto it = m_tiltShelfSlots.begin(), itEnd = m_tiltShelfSlots.end(); it != itEnd; ++it) {
        if (rectangle->placeToShelf(*it, m_size, m_containerId, false, m_tiltAngle)) {
            const Point p1 = rectangle->getCorners()[1];
            const Point p2 = rectangle->getCorners()[2];
            const Point p3 = rectangle->getCorners()[3];
            (*it) = { {0, -1}, p1, p2 - p1 };
            if (it == m_tiltShelfSlots.end() - 1) {
                m_tiltShelfSlots.push_back({ {0, -1}, p3, p2 - p1 });
            }
            m_placedRectangles.push_back(rectangle);
            return true;
        }
    }

    return false;
}

bool Container::placeOpposite(const std::shared_ptr<Rectangle>& rectangle)
{
    for (auto it = m_oppositeShelfSlots.rbegin(); it != m_oppositeShelfSlots.rend(); ++it) {
        if (rectangle->placeToShelf(*it, m_size, m_containerId, m_placedRectangles)) {
            // (*it) = { {0, -1}, {(*it).p.x - double(rectangle->getSize().width), (*it).p.y}, {-1, 0} };
            (*it).p.x = (*it).p.x - static_cast<double>(rectangle->getSize().width);
            m_placedRectangles.push_back(rectangle);
            return true;
        }
    }

    return false;
}

const std::list<std::shared_ptr<Rectangle> >& Container::getPlacedRectangles() const
{
    return m_placedRectangles;
}

void Container::updateOppositeShelfSlots()
{
    m_oppositeShelfSlots.resize(m_mainShelfSlots.size());

    for (auto itM = m_mainShelfSlots.begin() + 1, itO = m_oppositeShelfSlots.begin(),
        itMEnd = m_mainShelfSlots.end(); itM != itMEnd; ++itM, ++itO) {

        (*itO) = { {0, -1}, {(*itM).p.x, static_cast<double>(m_size.height)}, {-1, 0} };
    }
    *(m_oppositeShelfSlots.end() - 1) = {
        {0, -1},
        {static_cast<double>(m_size.width), static_cast<double>(m_size.height)},
        {-1, 0}
    };
}

void Container::excludeRectangle(const std::shared_ptr<Rectangle>& rectangle)
{
    m_placedRectangles.remove(rectangle);
}

const Size& Container::getSize() const
{
    return m_size;
}

void Container::setSize(const Size& size)
{
    this->m_size = size;
}