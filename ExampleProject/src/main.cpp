/* Main */

#include <FEXTools.h>

using namespace std;

int main()
{
    /* FIXED DISTANCE */
    {
        /* P(R) */
        DataStack RPStack("../IO_FixedDistance/RP");
        DataStack RPStack20("../IO_FixedDistance/RP20");

        /* I(U) */
        DataStack UIStack("../IO_FixedDistance/UI");
        DataStack UIStack20("../IO_FixedDistance/UI20");
    }

    /* FIXED RESISTANCE */
    {
        /* P(D) */
        DataSet DP("../IO_FixedResistance/DP");

        /* P(A) */
        DataSet AP("../IO_FixedResistance/AP");

        /* P(cos(theta)) */
        DataSet AP2("../IO_FixedResistance/AP2");
    }

    /* FIXED ANGLE */
    {
        /* P(R) */
        DataStack RPStack("../IO_FixedAngle/RP");

        /* I(U) */
        DataStack UIStack("../IO_FixedAngle/UI");
    }

    /* FILTERS */
    {
        /* P(R) */
        DataStack RPStack("../IO_Filters/RP");

        /* I(U) */
        DataStack UIStack("../IO_Filters/UI");
    }

    /* ROPTIM */
    {
        /* P(R) */
        Fitter RP("../IO_Pmax/RP");

        /* P(D) */
        Fitter DP("../IO_Pmax/DP");
        Fitter DP2("../IO_Pmax/DP2");

        /* P(cos(theta)) */
        Fitter AP("../IO_Pmax/AP");
    }

    return 0;
}