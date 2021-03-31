#ifndef STAR_StFttQAMaker_H
#define STAR_StFttQAMaker_H


/***************************************************************************                                          
 *                                                                                                                    
 * $Id: StFttQAMaker.h,v 0.1 2017/02/21 17:50:32 tlusty Exp $                                                    
 * StFttQAMaker - class to fille the StEvent from DAQ reader                                                        
 *--------------------------------------------------------------------------                                          
 *                                                                                                                    
 ***************************************************************************/
#include "StMaker.h"

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

// STL
#include <vector>

class StEvent;

const Int_t mMax = 100000;
struct STGCVMMData
{
    // event information
    Int_t    mEventId;
    Int_t    Sec;
    Int_t    Rdo;
    Int_t    n;

    //channel information
    Int_t    Channel_Number[mMax];
    Int_t    Strip_Number[mMax];
    Int_t    FEB[mMax];
    Int_t    FEBVMM[mMax];
    Int_t    VMM[mMax];
    Int_t    CH[mMax];
    Int_t    BCID[mMax];
    Int_t    ADC[mMax];

};


class StFttQAMaker: public StMaker
{
private:

public:

/// Default constructor                                                                                          
    StFttQAMaker(const char *name="fttQA");

    ~StFttQAMaker();


    Int_t  Init();
    Int_t  InitRun(Int_t);
    Int_t  FinishRun(Int_t);
    Int_t  Finish();
    Int_t  Make();

    void MakeDigiQA();

    void WriteHistograms();
    void BookHistograms();
    void BookTree();

    std::map< string, TH1* >    mH1d;
    std::map< string, TH2* >    mH2d;
    TTree * mVMMTree;
    STGCVMMData mVMMData;

    ClassDef(StFttQAMaker, 1)

};

#endif