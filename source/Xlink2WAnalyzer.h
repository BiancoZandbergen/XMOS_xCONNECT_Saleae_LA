#ifndef XLINK2W_ANALYZER_H
#define XLINK2W_ANALYZER_H

#include <Analyzer.h>
#include "Xlink2WAnalyzerResults.h"
#include "Xlink2WSimulationDataGenerator.h"

class Xlink2WAnalyzerSettings;
class ANALYZER_EXPORT Xlink2WAnalyzer : public Analyzer
{
public:
    Xlink2WAnalyzer();
    virtual ~Xlink2WAnalyzer();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
    virtual U32 GetMinimumSampleRateHz();

    virtual const char* GetAnalyzerName() const;
    virtual bool NeedsRerun();

protected: //vars
    std::auto_ptr< Xlink2WAnalyzerSettings > mSettings;
    std::auto_ptr< Xlink2WAnalyzerResults > mResults;
    AnalyzerChannelData* inputW0;
    AnalyzerChannelData* inputW1;

    

    Xlink2WSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    //Serial analysis vars:
    U32 mSampleRateHz;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //XLINK2W_ANALYZER_H
