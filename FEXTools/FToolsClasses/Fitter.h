/* Fitter */

#ifndef __FITTER_H__
#define __FITTER_H__

#include <string>
#include <vector>
#include <map>

#include <TF1.h>

#include "FToolsClasses/DataSet.h"

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
public: // Functions
    Fitter(const char* ConstructionDataPath);

protected: // Functions
    Fitter(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    // Virtual
    virtual void Draw(const char* DrawPath) const override;

    virtual std::string GetConstructor() const override;

    virtual void PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void PrintConstructor(std::ofstream& OutputStream) const override;

private: // Function
    // For own use
    void    ReadFunction(const std::string& Function);
    TString ProcessFormula();

    void Fit();

    // For use in DataStack
    friend class DataStack;

    virtual void FDraw() const override;

private: // Data
    TF1*        m_Function2Fit;
    std::string m_FormulaStr;

    std::map<std::string, unsigned int, strsize_less> m_VariableMap;
    std::vector<double>                               m_VariableValues;
    std::vector<double>                               m_VariableErrors;

    double m_Integral[2];

public: // Functions
    virtual ~Fitter();

    Fitter()              = delete;
    Fitter(const Fitter&) = delete;
    Fitter operator=(const Fitter&) = delete;
};

#endif
