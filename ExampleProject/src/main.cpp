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

    return 0;
}