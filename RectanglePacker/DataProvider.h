/******************************************************************************************************************************************
	 * File: DataProvider.h
	 * Author: Lunåv Arseniy (c) 2020
	 * Email: lunars@mail.ru
******************************************************************************************************************************************/
#pragma once
#include "pch.h"
#include "Rectangle.h"


class DataProvider
{
public:
	DataProvider();
	~DataProvider();

	bool parseData(Size& size, std::vector<std::shared_ptr<Rectangle> > &rectangles);
	void outputData(const Size& containerSize, std::vector<std::shared_ptr<Rectangle> >& rectangles);


private:
	std::vector<std::vector<int> > parsedData;
};

