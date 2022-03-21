/******************************************************************************************************************************************
	 * File: Benchmarking.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once

#include "pch.h"
#include "Rectangle.h"
#include "Container.h"


class Benchmarking
{
public:
	Benchmarking();
	~Benchmarking();
	Benchmarking(const Benchmarking&) = delete;
	Benchmarking& operator=(const Benchmarking&) = delete;

	double calculateDensity(Container& container);

	void logInfo(const std::vector<Container>& containers);
};

