/* Fitter */

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

class Fitter : public DataSet
{
public:
    Fitter(const char* ConstructionDataPath);
    Fitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath);

    virtual void Draw(const char* DrawPath) const override;

    virtual ~Fitter();

    Fitter()              = delete;
    Fitter(const Fitter&) = delete;
    Fitter operator=(const Fitter&) = delete;

protected:
    Fitter(const std::string& ConstructionData);
    virtual void Construct(const std::string& ConstructionData) override;

    virtual std::string GetConstructor() const override;
    virtual void        PrintConstructor(const char* ConstructionDataPath) const override;

private:
    void    ReadFunctionPath(const char* FunctionPath);
    void    ReadFunction(const std::string& Function);
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
