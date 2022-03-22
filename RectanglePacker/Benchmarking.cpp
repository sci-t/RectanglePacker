/******************************************************************************************************************************************
	 * File: Benchmarking.h
	 * Author: Lunev Arseniy (c) 2022
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "Benchmarking.h"
#include "Container.h"
#include "Rectangle.h"


double Benchmarking::calculateDensity(const Container& container)
{
    const auto& placedRectangles = container.getPlacedRectangles();
	int filledArea = 0;
	for (const auto& r : placedRectangles) {
        const Size rSize = r->getSize();
		filledArea += rSize.width * rSize.height;
	}

    const Size containerSize = container.getSize();
    const int containerArea = containerSize.width * containerSize.height;
	return static_cast<double>(filledArea) / static_cast<double>(containerArea);
}

void Benchmarking::logInfo(const std::vector<Container>& containers)
{
	std::clog << "\nDensities:\n";
	for (const auto& c : containers)
    {
        const double density = calculateDensity(c);
		std::clog << "container #" << c.getContainerId() << ": " << density << "\n";
	}
	std::clog << "Result number of containers = " << static_cast<int>(containers.size()) << "\n\n";
}
