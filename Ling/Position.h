#pragma once
class Position
{
public:
	Position();
	Position(const int& x = 0,const int& y = 0);
	~Position();
public:
	int x{ 0 }, y{0};
private:
};

