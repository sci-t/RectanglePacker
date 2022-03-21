/******************************************************************************************************************************************
	 * File: Benchmarking.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#include "Benchmarking.h"


Benchmarking::Benchmarking()
{
}

Benchmarking::~Benchmarking()
{
}

double Benchmarking::calculateDensity(Container& container)
{
	auto placedRectangles = container.getPlacedRectangles();
	int filledArea = 0;
	for (auto& r : placedRectangles) {
		Size rSize = r->getSize();
		filledArea += rSize.width * rSize.height;
	}

	Size containerSize = container.getSize();
	int containerArea = containerSize.width * containerSize.height;
	return double(filledArea) / double(containerArea);
}

void Benchmarking::logInfo(const std::vector<Container>& containers)
{
	std::clog << "\nDensities:\n";
	for (auto it = containers.begin(), itEnd = containers.end(); it != itEnd; ++it) {
		Container container = (*it);
		double density = calculateDensity(container);
		std::clog << "container #" << container.getContainerId() << ": " << density << "\n";
	}
	std::clog << "Result number of containers = " << static_cast<int>(containers.size()) << "\n\n";
}
