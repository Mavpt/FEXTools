/* Interpolator */

#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <TSpline.h>
#include <TF1.h>

#include "DataSet.h"

class Interpolator : public DataSet
{
public:
    Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath);
    virtual ~Interpolator();

    virtual void Draw(const char* DrawPath) const override;

    Interpolator()                    = delete;
    Interpolator(const Interpolator&) = delete;
    Interpolator operator=(const Interpolator&) = delete;

protected:
    virtual void FDraw() const override; // For use in DataStack

    void   PrintResult(const char* ResultPath);
    double Calculate(double* fx, double*) { return m_Spline3->Eval(fx[0]); }

    inline double GetXMinimum() const { return m_OverlayFunction->GetMinimumX(); }
    double        GetXMinimumError() const;

    inline double GetXMaximum() const { return m_OverlayFunction->GetMaximumX(); }
    double        GetXMaximumError() const;

    inline double GetYMinimum() const { return m_OverlayFunction->GetMinimum(); }
    double        GetYMinimumError() const;

    inline double GetYMaximum() const { return m_OverlayFunction->GetMaximum(); }
    double        GetYMaximumError() const;

protected:
    TSpline3* m_Spline3;
    TF1*      m_OverlayFunction;
};

#endif
