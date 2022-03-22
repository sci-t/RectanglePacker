/******************************************************************************************************************************************
	 * File: Benchmarking.h
	 * Author: Lunåv Arseniy (c) 2022
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include "pch.h"
class Container;


class Benchmarking final
{
public:
    static double calculateDensity(const Container& container);
    static void logInfo(const std::vector<Container>& containers);
};

