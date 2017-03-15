#pragma once

#include "Libraries.h"

#define SCALE_MAP 2
#define GRID_ROWS 1000
#define GRID_COLS 1000
#define GRID_WIDTH = 40
#define GRID_HEIGHT = 18

class PathFinder 
{
  public:
    PathFinder();
    void FindPath(Vector2d start, Vector2d end, ostream * ios);

  private:
    int UpdateCellValue(int n, int m, double value, queue<Vector2i>& queue);
    bool IsWalkableCell(int n, int m, double cost, vector<Vector2i>& path);
    bool IsOccludedPath(Vector2i start, Vector2i end);
    bool IsHorizontalValid(Vector2i start, Vector2i end);
    bool IsVerticalValid(Vector2i start, Vector2i end);
    bool IsDiagonalValid(Vector2i start, Vector2i end, int dx, int dy, double slope);
    void Propagate(int startX, int startY, int endX, int endY);
    vector<Vector2i> ExtractPath(int startX, int startY, int endX, int endY);
    vector<Vector2i> SmoothPath(vector<Vector2i>& path);
    void PrintGrid(vector<Vector2i>& waypoints);
    void PrintPaths(vector<Vector2i>& waypoints);
    void PrintPath(Vector2i start, Vector2i end);
    void HorizontalLine(Vector2i start, Vector2i end);
    void VerticalLine(Vector2i start, Vector2i end);
    void DiagonalLine(Vector2i start, Vector2i end, int dx, int dy, double slope);
    double gridMap[GRID_ROWS][GRID_COLS];
    int width, height;
    int xOffset, yOffset;
};