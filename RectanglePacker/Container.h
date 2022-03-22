/******************************************************************************************************************************************
     * File: Container.h
     * Author: Lunev Arseniy (c) 2022
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include "pch.h"
class Rectangle;


class Container
{
public:
    Container();
    Container(int containerId, const Size& size);

    virtual ~Container() = default;
    Container(const Container&) = default;
    Container& operator=(const Container&) = default;
    Container(Container&&) noexcept = default;
    Container& operator=(Container&&) = default;

    const Size& getSize() const;
    void setSize(const Size& size);

    int getContainerId() const;
    bool getIsEmpty() const;

    bool placeFirst(const std::shared_ptr<Rectangle>& rectangle);
    bool placeNext(const std::shared_ptr<Rectangle>& rectangle);
    bool placeOpposite(const std::shared_ptr<Rectangle>& rectangle);

    const std::list<std::shared_ptr<Rectangle> >& getPlacedRectangles() const;
    void updateOppositeShelfSlots();
    void excludeRectangle(const std::shared_ptr<Rectangle>& rectangle);

private:
    int m_containerId;
    Size m_size;
    bool m_isEmpty;
    double m_tiltAngle;

    std::list<std::shared_ptr<Rectangle> > m_placedRectangles;
    std::vector<ShelfCorner> m_mainShelfSlots;     // main slots for placing a rectangle
    std::vector<ShelfCorner> m_oppositeShelfSlots; // slots at main shelfs from the opposite side
    std::vector<ShelfCorner> m_tiltShelfSlots;     // additional slots for placing a rectangle in case of a non-zero tilt angle
};
