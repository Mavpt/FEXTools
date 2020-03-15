/* Main */

#include <FEXTools.h>

using namespace std;

int main()
{
    /* DataSet*/
    {
        DataSet Red("../IO_DataSets/Red.txt");
        Red.Draw("../IO_Plots/Red.png");
    }

    /* FunctionFitter */
    {
        FunctionFitter Yellow("../IO_DataSets/Yellow.txt");
        // FunctionFitter Yellow({ "Yellow", "U(V)", 0, 2.5, " I(A)", 0.0, 2.5E-03 }, { kYellow, kFullCircle, 1, kYellow, kSolid, 1 }, "../IO_Input/UI_Amarelo.txt", "../IO_Functions/UI_Amarelo.txt");
        Yellow.Draw("../IO_Plots/Yellow.png");
    }

    return 0;
}