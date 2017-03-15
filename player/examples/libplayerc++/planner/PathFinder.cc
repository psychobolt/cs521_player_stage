#include "Libraries.h"
#include "PlanModule.h"
#include "PathFinder.h"

PathFinder::PathFinder() 
{
  for (int m = 0; m < GRID_ROWS; m++)
    for (int n = 0; n < GRID_ROWS; n++)
      gridMap[m][n] = 0.0;
  ifstream map("scaled_hospital_section.pnm");
  char line[80], value;
  map.getline(line, 80);
  int maxVal;
  map >> width >> height >> maxVal; // TODO binary to int
  width = 543;
  height = 221;
  for (int i = 0; i < height; i++)    
    for (int j = 0; j < width; j++) {
      map >> value;
      if (!value)
        gridMap[i][j] = 1.0;
  }
  map.close();
}

void PathFinder::FindPath(Vector2d start, Vector2d end, ostream * ios) 
{
  int startX = round(width * ((start[0] + 20) / 40));
  int startY = round(height * ((-start[1] + 9) / 18));
  int endX = round(width * ((end[0] + 20) / 40));
  int endY = round(height * ((-end[1] + 9) / 18));

  Propagate(startX, startY, endX, endY);
  vector<Vector2i> path = ExtractPath(startX, startY, endX, endY);
  vector<Vector2i> waypoints  = SmoothPath(path);
  if (PlanModule::CLIENT.DEBUG) PrintGrid(path);
  for (vector<Vector2i>::iterator iter = waypoints.begin(); iter != waypoints.end(); iter++) 
  {
    Vector2i v = *iter;
    if (v == waypoints.front()) { // Skip start point

    }
    double x = v[0] / (double) width * 40 - 20;
    double y = -v[1] / (double) height * 18 + 9;
    (*ios) << x  << " " << y << endl;
  }
}

void PathFinder::Propagate(int startX, int startY, int endX, int endY)
{
  queue<Vector2i> q;
  Vector2i v(endX, endY);
  q.push(v);
  double lvl = 2.0;
  gridMap[endY][endX] = 2.0;
  bool **visited = new bool*[height];
  for (int i = 0; i < height; i++)
    visited[i] = new bool[width];
  while (!q.empty()) 
  {
    Vector2i head = q.front();
    int m = head[1];
    int n = head[0];
    if (!visited[m][n]) // check if not visited
    {
      if (m == startY && n == startX) 
      {
        break;
      }
      else
      {
        double level = gridMap[m][n] + 1.0;
        if (level > lvl) {
          lvl = level;
        }
        UpdateCellValue(n-1, m-1, level, q); // Top Left
        UpdateCellValue(n, m-1, level, q); // Top
        UpdateCellValue(n+1, m-1, level, q); // Top Right
        UpdateCellValue(n+1, m, level, q); // Right
        UpdateCellValue(n+1, m+1, level, q); // Bottom Right
        UpdateCellValue(n, m+1, level, q); // Bottom
        UpdateCellValue(n-1, m+1, level, q); // Bottom Left
        UpdateCellValue(n-1, m, level, q); // Left
      }
    }
    visited[m][n] = true;
    q.pop();
  }
  LINFO << "| Goal Distance: " << lvl;
}

vector<Vector2i> PathFinder::ExtractPath(int startX, int startY, int endX, int endY) 
{
  vector<Vector2i> path;
  Vector2i start(startX, startY);
  path.push_back(start);
  while(true)
  {
    Vector2i p = path.back();
    int n = p[0];
    int m = p[1];
    if (n == endX && m == endY) return path;
    int cost = gridMap[m][n] - 1;
    if (IsWalkableCell(n-1, m-1, cost, path)) continue;
    if (IsWalkableCell(n, m-1, cost, path)) continue;
    if (IsWalkableCell(n+1, m-1, cost, path)) continue;
    if (IsWalkableCell(n+1, m, cost, path)) continue;
    if (IsWalkableCell(n+1, m+1, cost, path)) continue;
    if (IsWalkableCell(n, m+1, cost, path)) continue;
    if (IsWalkableCell(n-1, m+1, cost, path)) continue;
    if (IsWalkableCell(n-1, m, cost, path)) continue;
  }
  Vector2i end(endX, endY);
  path.push_back(end);
  return path;
}

vector<Vector2i> PathFinder::SmoothPath(vector<Vector2i>& path)
{
  list<int> indices;
  for (int i = 0; i < path.size(); i++)
    indices.push_back(i);
  for (list<int>::iterator iter = indices.begin(); iter != indices.end(); iter++)
  {
    int i = *iter;
    for (int j = i + 1; j < path.size() - 1; j++)
    {
      if (!IsOccludedPath(path[i], path[j + 1])) 
      {
        indices.remove(j);
      }
    }
  }
  // for (list<int>::iterator iter = indices.end(); iter != indices.begin(); iter--)
  // {
  //   int i = *iter;
  //   for (int j = i - 2; j > 1; j--)
  //   {
  //     if (!IsOccludedPath(path[j + 1], path[i])) 
  //     {
  //       indices.remove(j);
  //     }
  //   }
  // }
  vector<Vector2i> result;
  for (list<int>::iterator iter = indices.begin(); iter != indices.end(); iter++)
    result.push_back(path[*iter]);
  return result;
}

bool PathFinder::IsOccludedPath(Vector2i start, Vector2i end)
{
  int dx = end[0] - start[0];
  int dy = end[1] - start[1];
  double slope = dx == 0 ? numeric_limits<double>::infinity() : (dy / (double) dx);
  int startX = std::min(start[0], end[0]);
  int startY = slope >= 0 ? std::min(start[1], end[1]) : std::max(start[1], end[1]);
  int endX = std::max(start[0], end[0]);
  int endY = std::max(start[1], end[1]);
  Vector2i v1(startX, startY);
  Vector2i v2(endX, endX);
  if (slope == 0)
  {
    return !IsHorizontalValid(v1, v2);
  }
  else if (slope == numeric_limits<double>::infinity())
  {
    return !IsVerticalValid(v1, v2);
  }
  return !IsDiagonalValid(v1, v2, dx, dy, slope);
}

bool PathFinder::IsHorizontalValid(Vector2i start, Vector2i end)
{
  int x = start[0];
  int y = start[1];
  while (x < end[0])
  {
    if (gridMap[y][x] == 1.0)
    {
      return false;
    }
    x++;
  }
  return true;
}

bool PathFinder::IsVerticalValid(Vector2i start, Vector2i end)
{
  int x = start[0];
  int y = start[1];
  while (y < end[1])
  {
    if (gridMap[y][x] == 1.0)
    {
      return false;
    }
    y++;
  }
  return true;
}

bool PathFinder::IsDiagonalValid(Vector2i start, Vector2i end, int dx, int dy, double slope)
{
  bool moveY = abs(slope) > 1;
  dx = abs(dx);
  dy = abs(dy);
  int increment = moveY ? 2 * dx : 2 * dy;
  int incrementXY = moveY ? 2 * (dx - dy) : 2 * (dy - dx);
  int x = start[0];
  int y = start[1];
  int d = moveY ? (2 * dx - dy) : (2 * dy - dx);
  int yOffset = slope > 0 ? 1 : -1;
  while (moveY ? y < end[1] : x < end[0]) 
  {
    if (moveY)
    {
      y += yOffset;
    } 
    else 
    {
      x++;
    }
    if (d > 0) 
    {
      d += incrementXY;
      if (moveY)
      {
        x++;
      }
      else 
      {
        y += yOffset;
      }
    } 
    else 
    {
      d += increment;
    }
    if (gridMap[y][x] == 1.0)
    {
      return false;
    }
  }
  if (gridMap[y][x] == 1.0)
  {
    return false;
  }
  return true;
}

int PathFinder::UpdateCellValue(int n, int m, double level, queue<Vector2i>& q)
{
  if (m < 0 || height < m) return -1;
  if (n < 0 || width < n) return -1;
  if (gridMap[m][n] != 0.0) return -1;
  gridMap[m][n] = level;
  Vector2i v(n, m);
  q.push(v);
  return 0;
}

bool PathFinder::IsWalkableCell(int n, int m, double cost, vector<Vector2i>& path)
{
  if (m < 0 || height < m) return false;
  if (n < 0 || width < n) return false;
  if (gridMap[m][n] != cost) return false;
  Vector2i v(n, m);
  path.push_back(v);
  return true;
}

void PathFinder::PrintGrid(vector<Vector2i>& waypoints)
{
  ofstream out("grid.txt");
  double copy[height][width];
  for (int m = 0; m < height; m++)
    for (int n = 0; n < width; n++)
      copy[m][n] = gridMap[m][n];
  Vector2i start = waypoints.front();
  Vector2i end = waypoints.back();
  for (vector<Vector2i>::iterator iter = waypoints.begin(); iter != waypoints.end(); iter++)
  {
    Vector2i v = *iter;
    copy[v[1]][v[0]] = 2.0;
  }
  PrintPaths(waypoints); //TODO pass copy
  for (int m = 0; m < height; m++)
  {
    for (int n = 0; n < width; n++)
    {
      double value = gridMap[m][n];
      // double value = copy[m][n];
      if (n == start[0] && m == start[1]) {
        out << 'S';
      }
      else if (n == end[0] && m == end[1]) 
      {
        out << 'E';
      } 
      else 
      if (value > 9.0) 
      {
        out << '_';
      }
      else if (value == 2.0)  
      {
        out << 'X';
      }
      else 
      {
        out << value;
      }
    }
    out << endl;
  }
}

void PathFinder::PrintPaths(vector<Vector2i>& waypoints)
{
  for (int i = 0; i < waypoints.size() - 1; i++)
  {
    PrintPath(waypoints[i], waypoints[i+1]);
  }
}

void PathFinder::PrintPath(Vector2i start, Vector2i end)
{
  int dx = end[0] - start[0];
  int dy = end[1] - start[1];
  double slope = dx == 0 ? numeric_limits<double>::infinity() : (dy / (double) dx);
  int startX = std::min(start[0], end[0]);
  int startY = slope >= 0 ? std::min(start[1], end[1]) : std::max(start[1], end[1]);
  int endX = std::max(start[0], end[0]);
  int endY = std::max(start[1], end[1]);
  Vector2i v1(startX, startY);
  Vector2i v2(endX, endX);
  if (slope == 0)
  {
    HorizontalLine(v1, v2);
  }
  else if (slope == numeric_limits<double>::infinity())
  {
    VerticalLine(v1, v2);
  }
  return DiagonalLine(v1, v2, dx, dy, slope);
}

void PathFinder::HorizontalLine(Vector2i start, Vector2i end)
{
  int x = start[0];
  int y = start[1];
  while (x < end[0])
  {
    gridMap[y][x] = 2.0;
    x++;
  }
}

void PathFinder::VerticalLine(Vector2i start, Vector2i end)
{
  int x = start[0];
  int y = start[1];
  while (y < end[1])
  {
    gridMap[y][x] = 2.0;
    y++;
  }
}

void PathFinder::DiagonalLine(Vector2i start, Vector2i end, int dx, int dy, double slope)
{
  bool moveY = abs(slope) > 1;
  dx = abs(dx);
  dy = abs(dy);
  int increment = moveY ? 2 * dx : 2 * dy;
  int incrementXY = moveY ? 2 * (dx - dy) : 2 * (dy - dx);
  int x = start[0];
  int y = start[1];
  int d = moveY ? (2 * dx - dy) : (2 * dy - dx);
  int yOffset = slope > 0 ? 1 : -1;
  while (moveY ? y < end[1] : x < end[0]) 
  {
    if (moveY)
    {
      y += yOffset;
    } 
    else 
    {
      x++;
    }
    if (d > 0) 
    {
      d += incrementXY;
      if (moveY)
      {
        x++;
      }
      else 
      {
        y += yOffset;
      }
    } else {
      d += increment;
    }
    gridMap[y][x] = 2.0;
  }
}
