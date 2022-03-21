/******************************************************************************************************************************************
     * File: DataProvider.h
     * Author: Lunåv Arseniy (c) 2020
     * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "DataProvider.h"
#include <regex>
#include <sstream>


DataProvider::DataProvider()
{
}

DataProvider::~DataProvider()
{
}

bool DataProvider::parseData(Size& size, std::vector<std::shared_ptr<Rectangle> >& rectangles)
{
    std::clog << "Reading data...\n\n";

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
        parsedData.push_back(parsedRow);
    }

    if (parsedData.empty()) {
        std::cerr << "No input data.\n";
        return false;
    }

    size = { parsedData[0][0], parsedData[0][1] };
    if (size.width < size.height) {
        std::swap(size.width, size.height);
    }

    for (auto it = parsedData.begin() + 1, itLEnd = parsedData.end(); it != itLEnd; ++it) {
        Rectangle r({ (*it)[0], (*it)[1] });
        rectangles.push_back(std::make_shared<Rectangle>(r));
    }

    return true;
}

void DataProvider::outputData(const Size& containerSize, std::vector<std::shared_ptr<Rectangle> >& rectangles)
{
    int containersNumber = (*rectangles.rbegin())->getContainerId() + 1;

    std::cout << containersNumber << "," <<
        containerSize.width << "," << containerSize.height << "\n";
    for (auto& r : rectangles) {
        int containerId = r->getContainerId();
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
