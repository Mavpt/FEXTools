/* FunctionPlotter */

#ifndef __FUNCTION_PLOTTER_H__
#define __FUNCTION_PLOTTER_H__

#include <string>
#include <vector>
#include <map>

#include <TF1.h>

#include "FToolsClasses/Fitter.h"

class FunctionPlotter : public FToolsObject
{
public: // Functions
    FunctionPlotter(const char* ConstructionDataPath);

protected: // Functions
    FunctionPlotter(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    // Virtual
    inline virtual const TObject* GetGraph() const override { return m_Function; };

    virtual void Draw(const char* DrawPath) const override;

    virtual std::string GetConstructor() const override;

    virtual void PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void PrintConstructor(std::ofstream& OutputStream) const override;

private: // Function
    // For own use
    void    ReadFunction(const std::string& Function);
    TString ProcessFormula();

    // For use in DataStack
    friend class DataStack;

    virtual void FDraw() const override;

private: // Data
    TF1*        m_Function;
    std::string m_FormulaStr;

    std::map<std::string, double, strsize_less> m_VariableMap;

public: // Functions
    virtual ~FunctionPlotter();

    FunctionPlotter()                       = delete;
    FunctionPlotter(const FunctionPlotter&) = delete;
    FunctionPlotter operator=(const FunctionPlotter&) = delete;
};

#endif
