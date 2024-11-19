#ifndef TABLE_EDGE_HPP
#define TABLE_EDGE_HPP

#include "Rectangle.hpp"

enum TableEdgeType {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

class TableEdge : public Rectangle {
public:
	bool visible;
	TableEdgeType type;

	TableEdge(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, TableEdgeType type);
	TableEdge(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, TableEdgeType type, bool visible);
	void drawEdge(const char* vsSource, const char* fsSource);
	void renderEdge();
};

#endif // TABLE_EDGE_HPP