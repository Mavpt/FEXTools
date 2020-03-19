/* Interpolator */

#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <TSpline.h>
#include <TF1.h>

#include "DataSet.h"

class Interpolator : public DataSet
{
public:
    Interpolator(const char* ConstructionDataPath);
    Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath);

    virtual void Draw(const char* DrawPath) const override;

    virtual ~Interpolator();

    Interpolator()                    = delete;
    Interpolator(const Interpolator&) = delete;
    Interpolator operator=(const Interpolator&) = delete;

protected:
    virtual void Construct(const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL) override;

    virtual std::string GetConstructor() const override;
    virtual void        PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void        PrintConstructor(std::ofstream& OutputStream) const override;

private:
    // For own use
    double Calculate(double* fx, double*) { return m_Spline3->Eval(fx[0]); }

    const double* GetMinimum() const;
    const double* GetMaximum() const;

    inline double GetXMinimum() const { return m_OverlayFunction->GetMinimumX(); }
    inline double GetXMaximum() const { return m_OverlayFunction->GetMaximumX(); }

    inline double GetYMinimum() const { return m_OverlayFunction->GetMinimum(); }
    inline double GetYMaximum() const { return m_OverlayFunction->GetMaximum(); }

    void PrintResult(const char* ResultPath);

    // For use in DataStack
    friend class DataStack;

    Interpolator(const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);
    virtual void FDraw() const override;

private:
    TSpline3* m_Spline3;
    TF1*      m_OverlayFunction;
};

#endif
