/******************************************************************************************************************************************
     * File: RectanglePacker.cpp
     * Description: This is the program for solving of NP-problem of packing with additional allowable rotations.
     * Author: Lunеv Arseniy (c) 2020
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "pch.h"
#include "Rectangle.h"
#include "Container.h"
#include "DataProvider.h"
#include "Benchmarking.h"


/** Main algorithm of packing with rototions. */
void packUp(std::vector<std::shared_ptr<Rectangle> >& rectangles,
    std::vector<Container>& containers, Size& containerSize)
{
    std::sort(rectangles.begin(), rectangles.end(),
        [](auto a, auto b) {
            return a->getSize().width > b->getSize().width;
        }
    );

    containers.reserve(rectangles.size() + 1);
    int containerId = 0;
    containers.push_back(Container(containerId++, containerSize));

    for (auto& r : rectangles) {
        bool isPlaced = false;

        auto contIt = containers.begin();
        while (!isPlaced && contIt != containers.end()) {
            Container& currentContainer = *contIt;

            if (!currentContainer.getIsEmpty()) {
                isPlaced = currentContainer.placeNext(r);
            }
            else {
                isPlaced = currentContainer.placeFirst(r);
                if (isPlaced) {
                    containers.push_back(Container(containerId++, containerSize));
                }
            }

            ++contIt;
        }
    }
    containers.pop_back();
}

/** Experimental improvement of packing from opposite side on shelfs. */
void packBack(std::vector<std::shared_ptr<Rectangle> >& rectangles,
    std::vector<Container>& containers, Size& containerSize)
{
    for (auto& r : rectangles) {
        r->flip();
    }
    std::sort(rectangles.begin(), rectangles.end(),
        [](auto a, auto b) {
            return a->getContainerId() < b->getContainerId() ||
                (a->getContainerId() == b->getContainerId() &&
                    a->getSize().height > b->getSize().height);
        }
    );

    for (auto& cont : containers) {
        cont.updateOppositeShelfSlots();
    }

    for(auto contRIt = containers.rbegin(), contRItEnd = containers.rend();
        contRIt != contRItEnd; ++contRIt) {
        for (auto& r : (*contRIt).getPlacedRectangles()) {
            bool isReplaced = false;
            for (auto contIt = containers.begin(); 
                (*contIt).getContainerId() != (*contRIt).getContainerId(); ++contIt) {
                    
                isReplaced = (*contIt).placeOpposite(r);
                if (isReplaced) {
                    break;
                }
            }
        }
    }
}

void cleanUp(std::vector<std::shared_ptr<Rectangle> >& rectangles)
{
    std::sort(rectangles.begin(), rectangles.end(),
        [](auto a, auto b) {
            return a->getContainerId() < b->getContainerId() ||
                (a->getContainerId() == b->getContainerId() &&
                    a->getSize().height < b->getSize().height);
        }
    );
}

int main()
{
    Size containerSize;
    std::vector<std::shared_ptr<Rectangle> > rectangles;
    DataProvider parser;
    if (!parser.parseData(containerSize, rectangles)) {
        return EXIT_FAILURE;
    }

    std::vector<Container> containers;

    size_t start_solve_time = clock();

    packUp(rectangles, containers, containerSize);
    packBack(rectangles, containers, containerSize);
    cleanUp(rectangles);

    std::clog << "Solving time:\n  " << (double(clock() - start_solve_time)) << " ms\n\n";

    parser.outputData(containerSize, rectangles);

    return EXIT_SUCCESS;
}