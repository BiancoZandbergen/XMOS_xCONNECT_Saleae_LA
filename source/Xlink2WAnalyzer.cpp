#include "Xlink2WAnalyzer.h"
#include "Xlink2WAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

Xlink2WAnalyzer::Xlink2WAnalyzer()
:   Analyzer(),
    mSettings( new Xlink2WAnalyzerSettings() ),
    mSimulationInitilized( false )
{
    SetAnalyzerSettings( mSettings.get() );
}

Xlink2WAnalyzer::~Xlink2WAnalyzer()
{
    KillThread();
}

#define IDLE 0
#define TOKEN 1
void Xlink2WAnalyzer::WorkerThread()
{
    int state = IDLE;      // analyzing state
    int transitions = 0;    // total transitions in token
    int transitions_w0 = 0; // transitions in token on wire 0
    int transitions_w1 = 0; // transitions in token on wire 1
    U32 token = 0;
    U32 adv_trans_w0 = 0; // transitions after advanced on wire 0
    U32 adv_trans_w1 = 0; // transitions after advanced on wire 1
    U64 token_start = 0;
    U64 token_end = 0;
    
    mResults.reset( new Xlink2WAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );
    mResults->AddChannelBubblesWillAppearOn( mSettings->chanW0 );

    mSampleRateHz = GetSampleRate();

    inputW0 = GetAnalyzerChannelData( mSettings->chanW0 );
    inputW1 = GetAnalyzerChannelData( mSettings->chanW1 );

    for ( ; ;) {
        adv_trans_w0 = inputW0->Advance(1);
        adv_trans_w1 = inputW1->Advance(1);
        
        if (adv_trans_w0 > 0 || adv_trans_w1 > 0) {
            if (state == IDLE) {
                state = TOKEN;
                token_start =inputW0->GetSampleNumber();
                mResults->AddMarker(token_start, AnalyzerResults::Start, mSettings->chanW0);
                mResults->AddMarker(token_start, AnalyzerResults::Start, mSettings->chanW1);
            }    
            
            if (transitions < 9) {
                if (adv_trans_w0 == 1) {
                    transitions_w0++;
                    token <<= 1;
                } else if (adv_trans_w1 == 1) {
                    transitions_w1++;
                    token <<= 1;
                    token |= 0x01;
                }

                transitions++;
            } else {
                // last transition in token
                //bool parity_correct = true;
                token_end = inputW0->GetSampleNumber();
                
                mResults->AddMarker(token_end, AnalyzerResults::Stop, mSettings->chanW0);
                mResults->AddMarker(token_end, AnalyzerResults::Stop, mSettings->chanW1);
                
                //we have a token to save. 
                Frame frame;
                frame.mData1 = token;
                frame.mFlags = 0;
                //frame.mFlags |= DISPLAY_AS_ERROR_FLAG;
                frame.mStartingSampleInclusive = token_start;
                frame.mEndingSampleInclusive = token_end;

                mResults->AddFrame( frame );
                mResults->CommitResults();
                ReportProgress( frame.mEndingSampleInclusive );
                
                token = 0;
                state = IDLE;
                transitions = 0;
                transitions_w0 = 0;
                transitions_w1 = 0;
                    
            }
        }
    }
}

bool Xlink2WAnalyzer::NeedsRerun()
{
    return false;
}

U32 Xlink2WAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
    if( mSimulationInitilized == false )
    {
        mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 Xlink2WAnalyzer::GetMinimumSampleRateHz()
{
    return 25000;//mSettings->mBitRate * 4;
}

const char* Xlink2WAnalyzer::GetAnalyzerName() const
{
    return "XMOS Link 2-wire";
}

const char* GetAnalyzerName()
{
    return "XMOS Link 2-wire";
}

Analyzer* CreateAnalyzer()
{
    return new Xlink2WAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}