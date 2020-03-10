/* Interpolator */

#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <TSpline.h>

#include "DataSet.h"

class Interpolator : public DataSet
{
public:
    Interpolator() = delete;
    Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath);
    virtual ~Interpolator();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties) override;

    virtual void Draw(const char* FilePath, const bool Flush = 1) const;

    Interpolator operator=(const Interpolator& i_DataStack) = delete;

protected:
    TSpline5* m_Spline3;
};

#endif
