/*
 *    sfall
 *    Copyright (C) 2008 - 2021 Timeslip and sfall team
 *
 */

#pragma once

namespace HRP
{

class EdgeBorder {
public:
	class Edge {
	public:
		POINT center;    // x/y center of current map screen?
		RECT borderRect; // right is less than left
		RECT rect_2;     //
		RECT tileRect;
		RECT squareRect; // angel clipping
		long clipData;   // angel clip type
		Edge* prevEdgeData = nullptr; // unused (use in 3.06)
		Edge* nextEdgeData = nullptr;

		void Release() {
			Edge* edge = this->nextEdgeData;
			while (edge) {
				Edge* edgeNext = edge->nextEdgeData;
				delete edge;
				edge = edgeNext;
			};
			this->nextEdgeData = nullptr;
		}

		~Edge() {
			if (nextEdgeData) delete nextEdgeData;
		}
	};

	static void init();

	static Edge* CurrentMapEdge();
	static long EdgeBorder::EdgeVersion();

	static long GetCenterTile(long tile, long mapLevel);
	static long CheckBorder(long tile);
};

}