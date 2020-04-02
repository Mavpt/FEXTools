/* Interpolator */

#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <TSpline.h>
#include <TF1.h>

#include "DataSet.h"

class Interpolator : public DataSet
{
public: // Functions
    Interpolator(const char* ConstructionDataPath);

    double Eval(const double x) { return m_Spline5->Eval(x); }

protected: // Functions
    Interpolator(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    virtual void Draw(const char* DrawPath) const override;

    virtual std::string GetConstructor() const override;

    virtual void PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void PrintConstructor(std::ofstream& OutputStream) const override;

private: // Functions
    // For own use
    double Calculate(double* fx, double*) { return m_Spline5->Eval(fx[0]); }

    const double* GetMinimum() const;
    const double* GetMaximum() const;

    inline double GetXMinimum() const { return m_OverlayFunction->GetMinimumX(); }
    inline double GetXMaximum() const { return m_OverlayFunction->GetMaximumX(); }

    inline double GetYMinimum() const { return m_OverlayFunction->GetMinimum(); }
    inline double GetYMaximum() const { return m_OverlayFunction->GetMaximum(); }

    // For use in DataStack
    friend class DataStack;

    virtual void FDraw() const override;

private: // Data
    TSpline5* m_Spline5;
    TF1*      m_OverlayFunction;

public: // Functions
    virtual ~Interpolator();

    Interpolator()                    = delete;
    Interpolator(const Interpolator&) = delete;
    Interpolator operator=(const Interpolator&) = delete;
};

#endif
