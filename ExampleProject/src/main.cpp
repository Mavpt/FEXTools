/* Main */

#include <FEXTools.h>

using namespace std;

int main()
{
    /* FIXED DISTANCE */
    {
        /* P(R) */
        DataStack RPStack("../IO/FD_RP");
        DataStack RPStack20("../IO/FD_RP20");

        /* I(U) */
        DataStack UIStack("../IO/FD_UI");
        DataStack UIStack20("../IO/FD_UI20");
    }

    /* FIXED RESISTANCE */
    {
        /* P(D) */
        DataSet DP("../IO/FR_DP");

        /* P(A) */
        DataSet AP("../IO/FR_AP");

        /* P(cos(theta)) */
        DataSet AP2("../IO/FR_AP2");
    }

    /* FIXED ANGLE */
    {
        /* P(R) */
        DataStack RPStack("../IO/FA_RP");

        /* I(U) */
        DataStack UIStack("../IO/FA_UI");
    }

    /* FILTERS */
    {
        /* P(R) */
        DataStack RPStack("../IO/Filter_RP");

        /* I(U) */
        DataStack UIStack("../IO/Filter_UI");
    }

    /* ROPTIM */
    {
        /* P(R) */
        Fitter RP("../IO/Pmax_RP");

        /* P(D) */
        Fitter DP("../IO/Pmax_DP");
        Fitter DP2("../IO/Pmax_DP2");

        /* P(cos(theta)) */
        Fitter AP("../IO/Pmax_AP");
    }

    return 0;
}