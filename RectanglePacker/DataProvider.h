/******************************************************************************************************************************************
	 * File: DataProvider.h
	 * Author: Lunåv Arseniy (c) 2022
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include "pch.h"
class Rectangle;


class DataProvider final
{
public:
	bool parseData(Size& size, std::vector<std::shared_ptr<Rectangle> > &rectangles);
    static void outputData(const Size& containerSize, const std::vector<std::shared_ptr<Rectangle>>& rectangles);

private:
	std::vector<std::vector<int> > m_parsedData;
};

