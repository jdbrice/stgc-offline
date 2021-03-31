/***************************************************************************
 *
 * $Id: StFttDigiMaker.cxx,v 1.4 2019/03/08 18:45:40 fseck Exp $
 *
 * Author: Florian Seck, April 2018
 ***************************************************************************
 *
 * Description: StFttDigiMaker - class to fill the StEvent from DAQ reader:
 * unpack raw data & save StETofHeader & StETofDigis in StETofCollection 
 *
 ***************************************************************************
 *
 * $Log: StFttDigiMaker.cxx,v $
 * Revision 1.4  2019/03/08 18:45:40  fseck
 * save middle value of tot bin as raw tot of the digi
 *
 * Revision 1.3  2019/02/19 20:32:09  fseck
 * update for unpacking year 2019+ daq files
 *
 * Revision 1.2  2018/07/27 13:58:12  fseck
 * small change to compile also in 64bit mode
 *
 * Revision 1.1  2018/07/25 14:39:40  jeromel
 * Peer reviewed Raghav+Jerome - code from Florian Seck
 *
 *
 ***************************************************************************/
#include <vector>
#include <map>
#include <array>
#include <algorithm>    // std::is_sorted

#include "StRTSBaseMaker.h"
#include "StDAQMaker/StDAQReader.h"
#include "StRtsTable.h"

#include "StEvent.h"

#include "StFttDigiMaker.h"



//_____________________________________________________________
StFttDigiMaker::StFttDigiMaker( const char* name )
: StRTSBaseMaker( "stgc", name ),
  // mEvent( 0 ),          /// pointer to StEvent
  mRunYear( 0 ),        /// year in which the data was taken (switch at 1st Oct)
  mDebug( false )       /// print out of all full messages for debugging
{
    LOG_DEBUG << "StFttDigiMaker::ctor"  << endm;
}

//_____________________________________________________________
StFttDigiMaker::~StFttDigiMaker()
{  /* no op */

}

//_____________________________________________________________
Int_t
StFttDigiMaker::Init()
{
    LOG_INFO << "StFttDigiMaker::Init" << endm;

    return kStOk;
}

//_____________________________________________________________
Int_t
StFttDigiMaker::InitRun( Int_t runnumber )
{ 
    mRunYear = ( runnumber + 727000 ) / 1000000 + 1999;

    LOG_INFO << "runnumber: " << runnumber << "  --> year: " << mRunYear << endm;

    return kStOk;
}

//_____________________________________________________________
Int_t
StFttDigiMaker::FinishRun( Int_t runnumber )
{ 
    return kStOk;
}

//_____________________________________________________________
Int_t
StFttDigiMaker::Finish()
{ 
    return kStOk;
}


//_____________________________________________________________
Int_t
StFttDigiMaker::Make()
{ 
    LOG_INFO << "StFttDigiMaker::Make()" << endm;

    mRawVMM.clear();

    StRtsTable* daqdta = GetNext( "vmm" );

    if ( daqdta == nullptr ) {
        LOG_WARN << "StFttDigiMaker::Make() - NO STGC DATA found in event" << endm;
        return kStOk;
    }


    // do unpacking of the raw data
    int inputSizeBytes = daqdta->GetSize();
    LOG_INFO << "StFttDigiMaker::Make() - InputSize (bytes): " << inputSizeBytes << endm;
    LOG_INFO << "Sector: " << daqdta->Sector() << endm;
    LOG_INFO << "Pad: " << daqdta->Pad() << endm;
    LOG_INFO << "Row: " << daqdta->Row() << endm;
    LOG_INFO << "Rdo: " << daqdta->Rdo() << endm;

    mRdo = daqdta->Rdo();
    mSec = daqdta->Sector();
    mPad = daqdta->Pad();

    if( mDebug ) {
        LOG_DEBUG << "StFttDigiMaker::Make() - InputSize (bytes): " << inputSizeBytes << endm;
    }

    LOG_INFO << "ROWS: " << daqdta->GetNRows() << endm;

    
    // struct stgc_vmm_t *vmm = (stgc_vmm_t *)  ( *daqdta->begin() );
    int len = 1;
    // LOG_INFO << "size: " << sizeof(vmm) << ", size: " << sizeof(stgc_vmm_t) << endm;
    
    for (auto it = daqdta->begin(); it != daqdta->end(); ++it) {
        
        struct stgc_vmm_t *vmm = (stgc_vmm_t *)  ( *it );
        u_char feb = vmm[0].feb_vmm >> 2 ;  // feb [0..5]
        u_char vm = vmm[0].feb_vmm & 3 ;    // VMM [0..3]
        
        printf("  FEB %d:%d, ch %02d: ADC %d, BCID %d\n",feb,vm,vmm[0].ch,vmm[0].adc,vmm[0].bcid) ;
        mRawVMM.push_back( *vmm );

    }







    return kStOk;
}
