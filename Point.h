#pragma once

struct Point
{
	Point() : x(0), y(0)
	{
	}

	Point(int x, int y) : x(x), y(y)
	{
	}

	Point(const Point& point) : x(point.x), y(point.y)
	{
	}

	~Point()
	{
	}

	int x = 0;
	int y = 0;
};