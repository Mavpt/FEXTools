/* Main */

#include <FEXTools.h>

#define FDRP_AXIS "R (#Omega)", 0.0, 3.2E3, "P (W)", 0.0, 8.0E-03
#define FARP_AXIS "R (#Omega)", 0.0, 3.2E3, "P (W)", 0.0, 2.5E-02
#define CRP_AXIS  "R (#Omega)", 0.0, 3.2E3, "P (W)", 0.0, 4.0E-03

#define FDUI_AXIS "U (V)", 0.0, 2.5, "I (A)", 0.0, 5.0E-03
#define FAUI_AXIS "U (V)", 0.0, 2.5, "I (A)", 0.0, 1.5E-02
#define CUI_AXIS  "U (V)", 0.0, 2.5, "I (A)", 0.0, 2.5E-03

#define FRDP_AXIS "d (m)", 0.0, 3.5E-01, "P (W)", 0.0, 8.0E-03
#define FRAP_AXIS "#theta (deg)", 0.0, 100.0, "P (W)", 0.0, 6.0E-03

#define PmaxRP_AXIS "R (#Omega)", 0.0, 3.5E3, "P (W)", 0.0, 2.5E-02
#define PmaxDP_AXIS "d (m)", 0.0, 0.4, "P (W)", 0.0, 2.5E-02
#define PmaxAP_AXIS "cos(#theta)", 0.0, 1.0, "P (W)", 0.0, 8.E-03

#define RoptimGRAPH()                        \
    {                                        \
        kBlack, kStar, 2, kBlack, kDotted, 2 \
    }

using namespace std;

int main()
{
    /* DATASET TEST */
    DataSet Red("../IO_DataSet/Red_RP.txt");
    Red.Draw("../IO_Plots/Red.eps");

    return 0;
}