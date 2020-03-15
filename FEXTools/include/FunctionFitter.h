/* FunctionFitter */

#ifndef __FUNCTION_FITTER_H__
#define __FUNCTION_FITTER_H__

#include <string>
#include <vector>
#include <map>

#include <TF1.h>

#include "DataSet.h"

struct strsize_less
{
    bool operator()(std::string const& l, std::string const& r) const
    {
        if (l.size() > r.size()) return true;

        if (l.size() < r.size()) return false;

        return l < r;
    };
};

class FunctionFitter : public DataSet
{
public:
    FunctionFitter(const char* ConstructionDataPath);
    FunctionFitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath);

    virtual void Draw(const char* DrawPath) const override;

    virtual ~FunctionFitter();

    FunctionFitter()                      = delete;
    FunctionFitter(const FunctionFitter&) = delete;
    FunctionFitter operator=(const FunctionFitter&) = delete;

protected:
    FunctionFitter(const std::string& ConstructionData);
    virtual void Construct(const std::string& ConstructionData) override;

private:
    void    ReadFunction(const char* FunctionPath);
    TString ProcessFormula();

    void Fit();

    void PrintResult(const char* FunctionPath);

private: // For use in DataStack
    virtual void FDraw() const override;

private:
    TF1*        m_Function2Fit;
    std::string m_FormulaStr;

    std::map<std::string, unsigned int, strsize_less> m_VariableMap;
    std::vector<double>                               m_VariableValues;
    std::vector<double>                               m_VariableErrors;
};

#endif
