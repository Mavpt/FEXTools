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

    /* Fitter */
    {
        Fitter Yellow("../IO_DataSets/Yellow.txt");
        Yellow.Draw("../IO_Plots/Yellow.png");
    }

    /* Interpolator */
    {
        Interpolator Green("../IO_DataSets/Green.txt");
        Green.Draw("../IO_Plots/Green.png");
    }

    /* DataStack */
    {
        DataStack UI("../IO_DataSets/UI_Stack.txt");
        UI.Draw("../IO_Plots/UI.png");

        DataStack RP("../IO_DataSets/RP_Stack.txt");
        RP.Draw("../IO_Plots/RP.png");
    }

    return 0;
}