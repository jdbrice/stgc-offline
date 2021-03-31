/***************************************************************************
 *
 * $Id: StFttRawHitMaker.h,v 1.3 2019/02/19 20:32:05 fseck Exp $
 *
 * Author: Florian Seck, April 2018
 ***************************************************************************
 *
 * Description: StFttRawHitMaker - class to fill the StEvent from DAQ reader:
 * unpack raw data & save StETofHeader & StETofDigis in StETofCollection 
 *
 ***************************************************************************
 *
 * $Log: StFttRawHitMaker.h,v $
 * Revision 1.3  2019/02/19 20:32:05  fseck
 * update for unpacking year 2019+ daq files
 *
 * Revision 1.2  2018/07/27 13:58:08  fseck
 * small change to compile also in 64bit mode
 *
 * Revision 1.1  2018/07/25 14:39:40  jeromel
 * Peer reviewed Raghav+Jerome - code from Florian Seck
 *
 *
 ***************************************************************************/
#ifndef STFTTRAWHITMAKER_H
#define STFTTRAWHITMAKER_H

#include <stdint.h>
#include "StRTSBaseMaker.h"

#include <vector>

struct stgc_vmm_t {
    unsigned char feb_vmm;
    unsigned char ch;
    unsigned short adc;
    unsigned short bcid;
};

class StFttDigiMaker: public StRTSBaseMaker {

public:
    StFttDigiMaker( const char* name = "stgc" );

    ~StFttDigiMaker();


    Int_t  Init();
    Int_t  InitRun( Int_t );
    Int_t  FinishRun( Int_t );
    Int_t  Finish();
    Int_t  Make();

    std::vector<stgc_vmm_t> mRawVMM;
    Int_t mRdo, mSec, mPad;

private:
    // StEvent*             mEvent;
    // StETofCollection*    mETofCollection;
    Int_t                mRunYear;
    Bool_t               mDebug;

    virtual const Char_t *GetCVS() const { static const char cvs[]="Tag $Name:  $Id: built " __DATE__ " " __TIME__ ; return cvs; }
    
    ClassDef( StFttDigiMaker, 0 )
};

// #include "StRTSBaseMaker.h"
// class StRTSBaseMaker;
// class StEvent;

// class StFttRawHitMaker: public StRTSBaseMaker {
// public:
//     /// Default constructor
//     StFttRawHitMaker( const char* name = "fttRawHit" );

//     ~StFttRawHitMaker();


//     Int_t  Init();
//     Int_t  InitRun( Int_t );
//     Int_t  FinishRun( Int_t );
//     Int_t  Finish();
//     Int_t  Make();

//     // StETofCollection* getETofCollection();

//     void   setDebug( const bool debug );



// private:
//     StEvent*             mEvent;
//     // StETofCollection*    mETofCollection;
//     Int_t                mRunYear;
//     Bool_t               mDebug;

//     virtual const Char_t *GetCVS() const { static const char cvs[]="Tag $Name:  $Id: built " __DATE__ " " __TIME__ ; return cvs; }

//     ClassDef( StFttRawHitMaker, 0 )
// };

// inline void StFttRawHitMaker::setDebug( const bool debug )  { mDebug = debug; }

#endif // STFTTRAWHITMAKER_H