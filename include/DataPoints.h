/* DataPoints */

#ifndef DataPoints_H
#define DataPoints_H

#include <string>

class DataPoints
{
public:
    DataPoints() = delete;
    DataPoints(const int N, double *x, double *y);

    virtual ~DataPoints();

    virtual void Draw(const char * = "DataSet.eps");

protected:
    const int m_N;     //Number of Data Points
    double *m_x, *m_y; //Points (x,y)
};

#endif