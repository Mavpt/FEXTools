/* Interpolator */

#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <TSpline.h>
#include <TF1.h>

#include "DataSet.h"

class Interpolator : public DataSet
{
public:
    Interpolator() = delete;
    Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath);
    virtual ~Interpolator();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties) override;

    virtual void Draw(const char* FilePath, const bool Flush = 1) const;

    Interpolator operator=(const Interpolator& i_DataStack) = delete;

protected:
    void   PrintResult(const char* ResultPath);
    double Calculate(double* fx, double*) { return m_Spline3->Eval(fx[0]); }

protected:
    TSpline3* m_Spline3;
    TF1*      m_OverlayFunction;
};

#endif
