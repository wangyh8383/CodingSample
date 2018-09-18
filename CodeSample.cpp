/*
 * Reasons why choose this block of codes:
 * 1. I like graph problems.
 * 2. This is an interview question I met before.
 * 3. I think it looks not bad.
*/

/*
 * Problem Description:
 * Inside of a rectangle area, there are many checkin points (i.e. '*'). A restaurant is defined as an area no larger than 20m x 20m and there are at least 20 checkin points inside.
 * Output: find all the possible restaurants.
 * -------------------
 * |    *        * * |
 * |              *  |
 * |                 |
 * |  *        *     |
 * |   *       *     |
 * |*                |
 * -------------------  13000m x 13000m
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

// represent a check-in place by Point
class Point {
public:
    double x;
    double y;
    double t;
    Point(double x, double y){
        this->x = x;
        this->y = y;
        this->t = 0;
    };

    Point(double x, double y, double t){
        this->x = x;
        this->y = y;
        this->t = t;
    };
};

// represent a rectangle by 2 points: 1. the left-up point, 2. the right-down point
class Rectangle{
public:
	Point leftup;
	Point rightdown;
	Rectangle(Point lu, Point rd): leftup(lu), rightdown(rd) {};
};

/*
* Solution 1: Divide and Conquer, time complexity: O(nlogn)
*/

const int ACCURACY = 2; // break length and width in half a time, get an ¼ map
const int LENGTH = 13000;
const int WIDTH = 13000;

// O(n), n = points.size()
void PopularPlace(Rectangle map, vector<Point> points, vector<Rectangle> & resturants){ 	if(points.size() < 20)	return; // no need to recurse
    if(abs(map.leftup.x - map.rightdown.x) <= 20 && abs(map.leftup.y - map.rightdown.y)){ // if the current map is no larger than 20m x 20m
        resturants.push_back(map); // take down this answer
        return;
    }

    // otherwise, means the current map is larger than 20m x 20m and has more than 20 check-ins inside. We will evenly break into ACCURACY^2 ones. Each smaller map will keep a list of points that are inside of it.

    // 1. first create these smaller maps
    double deltaX = LENGTH/ACCURACY;
    double deltaY = WIDTH/ACCURACY;
    vector<Rectangle> smallerMaps;
    for(int i=0; i<ACCURACY; i++){	// O(ACCURACY^2) -> O(1)
        for(int j=0; j<ACCURACY; j++){
            double nx = deltaX * i;
            double ny = deltaY * j;
            smallerMaps.push_back(Rectangle(Point(nx, ny), Point(nx+deltaX, ny+deltaY)));
        }
    }

// 2. put points in its corresponding smallerMap
    vector<vector<Point>> pointsInSmallerMaps(smallerMaps.size(), vector<Point>()); // index corresponding to smallerMaps
    for(int i=0; i<points.size(); i++){	// O(n)
        double relativeX = points[i].x - map.leftup.x;
        double relativeY = points[i].y - map.leftup.y;
// turn 2D indices -> 1D index
        int index = relativeX/deltaX * ACCURACY + relativeY/deltaY;
        pointsInSmallerMaps[index].push_back(points[i]); // use world location here
    }

// 3. recursively test each smallerMap
    for(int i=0; i<smallerMaps.size(); i++){	// O(ACCURACY^2) -> O(1)
        PopularPlace(smallerMaps[i], pointsInSmallerMaps[i], resturants);
    }
}

// O(nlogkn)  k=ACCURACY^2, is the base of the log func -> O(nlogn)
vector<Rectangle> PopularPlaceSolution1(vector<Point> checkins){
// initial map is the whole map
    Rectangle initialMap = Rectangle(Point(0,0,0), Point(LENGTH,WIDTH,0));
    vector<Rectangle> resturants;
    PopularPlace(initialMap, checkins, resturants);
    return resturants;
}

/*
* Solution 2: BFS/UnionFind, time complexity: O(n)
* Assumption: all x and y of checkins are integers
* Idea: search the 20m x 20m region of each checkin point to see whether >= 20 points exists. If the x and y are all integers or relatively low accuracy float/double variables, we can consider operation for each point is const (e.g. if it is an integer, 400 operations for each point). So overall running time can be O(n), n is the total checkin points in the graph.
*/

// Time complexity: O(n), Space complexity: O(n), n = checkins.size()
vector<Rectangle> PopularPlaceSolution2(vector<Point> checkins){
    vector<Rectangle> resturants;
    // 1. use a hashset to track all checkin points
    unordered_set<Point> graph;
    for(int i=0; i<checkins.size(); i++){
        graph.insert(checkins[i]);
    }

    // 2. search the 20m x 20m region of each checkin point
    for(int i=0; i<checkins.size(); i++){	// O(n * C) -> O(n)
        int leftupX = checkins[i].x - 10;
        int leftupY = checkins[i].y - 10;
        // go through 400 points
        int cnt = 0;
        for(int j=leftupX; j<leftupX+20; j++){
            for(int k=leftupY; k<leftupY+20; k++){
                // check whether index is valid
                if(j >= 0 && j < LENGTH && k >= 0 && k < WIDTH){
                    if(graph.count(Point(j, k)) > 0)	cnt++;
                }
            }
        }
        if(cnt >= 0){
            resturants.push_back(Rectangle(Point(leftupX, leftupY), Point(leftupX+20, leftupY+20)));
        }
    }
    return resturants;
}