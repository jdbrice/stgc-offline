# STGC Offline Data Chain

Provides raw data reader and offline analysis of low level sTGC data

## How to Run
You can run the code as follows:
```sh
root4star -b -q -l 'bfc.C( 5000, "in, db, FttDat, FttQA", "stgc_2021_03_31_05_34.daq" )'
```
this runs the `StFttDigiMaker` (`FttDat`) and the `StFttQAMaker` (`FttQA`).


## Modules

The following modules are included

### StFttDigiMaker
The digi maker simply reads the low level information from the DAQ files and stores the info.
For each event there is:
- Sector
- RDO

For each hit there is:
- FEB
- VMM
- CH
- BCID
- ADC


### StFttQAMaker
Produces QA histograms and TTree output for STGC data.
Currently only provides low level QA (level of online plots)

In the future it should contain the higher level QA as well based on later stages in the offline chain.

Currently produces `fttQA.root`
it contains histograms and a ttree `sTGC_DataDst`

```cpp
struct STGCVMMData
{
    // event information
    Int_t    mEventId;
    Int_t    Sec;
    Int_t    Rdo;
    Int_t    n;

    //channel information
    Int_t    Channel_Number[mMax]; // mapped
    Int_t    Strip_Number[mMax]; // mapped
    // RAW level
    Int_t    FEB[mMax];
    Int_t    FEBVMM[mMax];
    Int_t    VMM[mMax];
    Int_t    CH[mMax];
    Int_t    BCID[mMax];
    Int_t    ADC[mMax];

};
```

## StBFChain
The StBFChain is included in order to provide new bfc string options. We have added:
- `FttDat` : runs the StFttDigiMaker
- `FttQA` : runs the StFttQAMaker


# TODO
- Add StEvent and collections
- Add StFttClusterMaker : makes clusters from the raw data
- Add StFttHitMaker : makes 2D hit points
- Update the BFC options
- Update QAMaker with cluster level
- Update QAMaker with hit level
