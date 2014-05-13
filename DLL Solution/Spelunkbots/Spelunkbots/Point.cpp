#include "stdafx.h"
#include "Point.h"


Point::Point()
{
    parent = NULL;
    closed = false;
    opened = false;

    x = y = f = g = h = 0;
}

Point::Point(int x, int y, bool w)
{
    Point();

    this->walkable = w;
    this->x = x;
    this->y = y;
}

double* Point::getPositionX()
{
    return new Position((float)(x * 64), (float)(y * 64));
}

Point* Point::getParent()
{
    return parent;
}

void Point::setParent(Point *p)
{
    parent = p;
}

int Point::getX()
{
    return x;
}

int Point::getY()
{
    return y;
}

float Point::getXf()
{
    return (float)x;
}

float Point::getYf()
{
    return (float)y;
}

int Point::getGScore(Point *p)
{
    return p->g + ((x == p->x || y == p->y) ? 10 : 14);
}

int Point::getHScore(Point *p)
{
    return (abs(p->x - x) + abs(p->y - y)) * 10;
}

int Point::getGScore()
{
    return g;
}

int Point::getHScore()
{
    return h;
}

int Point::getFScore()
{
    return f;
}

void Point::computeScores(Point *end)
{
    g = getGScore(parent);
    h = getHScore(end);
    f = g + h;
}

bool Point::hasParent()
{
    return parent != NULL;
}


