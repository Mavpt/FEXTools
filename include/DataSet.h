/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>

class DataSet
{
public:
    DataSet() = delete;
    DataSet(const char* Title, const Int_t N, const char* xTitle, const Double_t* x, const Double_t* e_x, const char* yTitle, const Double_t* y, const Double_t* e_y);
    DataSet(const char* Title, const char* xTitle, const char* yTitle, const char* FilePath);

    virtual ~DataSet();

    virtual void Draw(const char* = "DataSet.eps") const;

    inline void Print() const { m_Graph->Print(); }

protected:
    TGraphErrors* m_Graph;
};

#endif