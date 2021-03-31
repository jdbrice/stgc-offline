#include "StFttQAMaker.h"

#include "StFttDigiMaker/StFttDigiMaker.h"


//_____________________________________________________________                                                       
StFttQAMaker::StFttQAMaker(const char *name):StMaker("etofCluster",name)
{                                            
    LOG_DEBUG << "StFttQAMaker::ctor"  << endm;
}
//_____________________________________________________________                                                       
StFttQAMaker::~StFttQAMaker()
{ 
}

//_____________________________________________________________                                                       
Int_t StFttQAMaker::Init()
{
    LOG_INFO << "StFttQAMaker::Init" << endm;

    BookHistograms();
    BookTree();
    return kStOk;
}
//_____________________________________________________________                                                       
Int_t StFttQAMaker::InitRun(Int_t runnumber)
{ 
    return kStOk;
}

//_____________________________________________________________                                                       
Int_t StFttQAMaker::FinishRun(Int_t runnumber)
{ 
    return kStOk;
}

//-------------------------------------------------------------                                                       
Int_t StFttQAMaker::Finish()
{ 
    LOG_INFO << "StFttQAMaker::Finish()" << endm;
    TFile *histFile = new TFile( "fttQA.root", "RECREATE" );
    histFile->cd();

    WriteHistograms();

    mVMMTree->Write();

    LOG_INFO << "StFttQAMaker::Finish() - writing *.fttQA.root ..." << endm;

    histFile->Close();
    return kStOk;
}

//_____________________________________________________________                                                       
Int_t StFttQAMaker::Make()
{ 
    LOG_INFO << "StFttQAMaker::Make()" << endm;
    
    MakeDigiQA();
    
    return kStOk;
}

//_____________________________________________________________  
void
StFttQAMaker::MakeDigiQA(){
    StFttDigiMaker * digiMaker = (StFttDigiMaker *)GetMaker( "stgc" );
    LOG_INFO << "GOT: " << digiMaker << endm;
    LOG_INFO << "GOT VMM: " << digiMaker->mRawVMM.size() << endm;


    mVMMData.n = 0;
    mVMMData.Sec = digiMaker->mSec;
    mVMMData.Rdo = digiMaker->mRdo;

    
    for ( auto vmm : digiMaker->mRawVMM ) {
        mH1d[ "ADC" ]->Fill( vmm.adc );
        mH1d[ "BCID" ]->Fill( vmm.bcid );

        u_char feb = vmm.feb_vmm >> 2 ;  // feb [0..5]
        u_char vm = vmm.feb_vmm & 3 ;    // VMM [0..3]

        mH1d[ "FEBVMM" ]->Fill( vmm.feb_vmm );
        mH1d[ "FEB" ]->Fill( feb );
        mH1d[ "VMM" ]->Fill( vm );
        mH1d[ "CH" ]->Fill( vmm.ch );


        mVMMData.ADC[mVMMData.n] = vmm.adc;
        mVMMData.FEB[mVMMData.n] = feb;
        mVMMData.FEBVMM[mVMMData.n] = vmm.feb_vmm;
        mVMMData.VMM[mVMMData.n] = vm;
        mVMMData.CH[mVMMData.n] = vmm.ch;
        mVMMData.BCID[mVMMData.n] = vmm.bcid;

        mVMMData.n++;
        LOG_INFO << "n = " << mVMMData.n << endm;
    }

    mVMMTree->Fill();
}

//_____________________________________________________________  
void
StFttQAMaker::WriteHistograms()
{
    LOG_DEBUG << "StFttQAMaker::WriteHistograms()" << endm;
    for( const auto& kv : mH1d ) {
        if( kv.second->GetEntries() > 0 ) kv.second->Write();
    }
    for( const auto& kv : mH2d ) {
        if( kv.second->GetEntries() > 0 ) kv.second->Write();
    }
}
//_____________________________________________________________  
void
StFttQAMaker::BookHistograms()
{

    mH1d[ "ADC" ] = new TH1F( "ADC", "ADC", 1024, 0, 4096 );
    mH1d[ "BCID" ] = new TH1F( "BCID", "BCID", 1024, 0, 4096 );
    mH1d[ "FEB" ] = new TH1F( "FEB", "FEB", 10, 0, 10 );
    mH1d[ "FEBVMM" ] = new TH1F( "FEBVMM", "FEBVMM", 4096, 0, 4096 );
    mH1d[ "VMM" ] = new TH1F( "VMM", "VMM", 10, 0, 10 );
    mH1d[ "CH" ] = new TH1F( "CH", "CH", 100, 0, 100 );


    for( auto& kv : mH1d ) {
        kv.second->SetDirectory( 0 );
    }

    for( auto& kv : mH2d ) {
        kv.second->SetDirectory( 0 );
    }
}

//_____________________________________________________________  
void
StFttQAMaker::BookTree()
{
    cout << " Booking the event tree " << endl;
    mVMMTree = new TTree("sTGC_DataDst","sTGC_DataDst");
    mVMMTree->SetAutoSave(100000000); // 100 MB

    // Event information
    mVMMTree->Branch("mEvtID", &mVMMData.mEventId, "mEvtID/I");
    mVMMTree->Branch("n", &mVMMData.n, "n/I");

    // Channel information
    mVMMTree->Branch("Channel_Number", mVMMData.Channel_Number, "Channel_Number[n]/I");
    mVMMTree->Branch("Strip_Number", mVMMData.Strip_Number, "Strip_Number[n]/I");
    mVMMTree->Branch("FEB", mVMMData.FEB, "FEB[n]/I");
    mVMMTree->Branch("FEBVMM", mVMMData.FEBVMM, "FEBVMM[n]/I");
    mVMMTree->Branch("VMM", mVMMData.VMM, "VMM[n]/I");
    mVMMTree->Branch("CH", mVMMData.CH, "CH[n]/I");
    mVMMTree->Branch("BCID", mVMMData.BCID, "BCID[n]/I");
    mVMMTree->Branch("ADC", mVMMData.ADC, "ADC[n]/I");
}