/******************************************************************************************************************************************
     * File: DataProvider.h
     * Author: Lunåv Arseniy (c) 2022
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "DataProvider.h"
#include "Rectangle.h"
#include <regex>
#include <sstream>


bool DataProvider::parseData(Size& size, std::vector<std::shared_ptr<Rectangle> >& rectangles)
{
    std::clog << "Insert data:\n(the size for all containers and the sizes for every rectangle)\n\nwidth,height\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::regex re(R"([0-9]+\,[0-9]+)");
        if (!std::regex_match(line, re)) {
            std::cerr << "The input data format is incorrect.\n";
            return false;
        }

        std::stringstream lineStream(line);
        std::string cell;
        std::vector<int > parsedRow;
        while (std::getline(lineStream, cell, ',')) {
            parsedRow.push_back(std::stoi(cell));
        }
        m_parsedData.push_back(parsedRow);
    }

    if (m_parsedData.empty()) {
        std::cerr << "No input data.\n";
        return false;
    }

    size = { m_parsedData[0][0], m_parsedData[0][1] };
    if (size.width < size.height) {
        std::swap(size.width, size.height);
    }

    for (auto it = m_parsedData.begin() + 1, itLEnd = m_parsedData.end(); it != itLEnd; ++it) {
        Rectangle r({ (*it)[0], (*it)[1] });
        rectangles.push_back(std::make_shared<Rectangle>(r));
    }

    return true;
}

void DataProvider::outputData(const Size& containerSize, const std::vector<std::shared_ptr<Rectangle>>& rectangles)
{
    const int containersNumber = (*rectangles.rbegin())->getContainerId() + 1;

    std::cout << containersNumber << "," <<
        containerSize.width << "," << containerSize.height << "\n";
    for (auto& r : rectangles) {
        const int containerId = r->getContainerId();
        if (containerId != MISSING_CONTAINER_ID) {
            auto& vv = r->getCorners();
            std::cout << containerId;
            for (auto& v : vv) {
                std::cout << "," << v.x << "," << v.y;
            }
            std::cout << "\n";
        }
    }
}
