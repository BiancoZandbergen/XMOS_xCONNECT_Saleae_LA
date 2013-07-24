#include "Xlink2WSimulationDataGenerator.h"
#include "Xlink2WAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

Xlink2WSimulationDataGenerator::Xlink2WSimulationDataGenerator()
:   mSerialText( "My first analyzer, woo hoo!" ),
    mStringIndex( 0 )
{
}

Xlink2WSimulationDataGenerator::~Xlink2WSimulationDataGenerator()
{
}

void Xlink2WSimulationDataGenerator::Initialize( U32 simulation_sample_rate, Xlink2WAnalyzerSettings* settings )
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    w0SimData = xlink_channels.Add(mSettings->chanW0, simulation_sample_rate, BIT_LOW);
    w1SimData = xlink_channels.Add(mSettings->chanW1, simulation_sample_rate, BIT_LOW);
    
    //w0SimulationData.SetChannel( mSettings->mInputChannel );
    //w0SimulationData.SetSampleRate( simulation_sample_rate );
    //w0SimulationData.SetInitialBitState( BIT_LOW );
    
    //w1SimulationData.SetChannel( mSettings->chanW1 );
    //w1SimulationData.SetSampleRate( simulation_sample_rate );
    //w1SimulationData.SetInitialBitState( BIT_LOW );

}

U32 Xlink2WSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

    while( w0SimData->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
    {
        //CreateSerialByte();
        xlink_channels.AdvanceAll(1000);
        create_token(0x1CD, 20);
        xlink_channels.AdvanceAll(1000);
        create_token(0x88, 20);
    }

    *simulation_channel = xlink_channels.GetArray();
    return 2;
}

#if 0
void Xlink2WSimulationDataGenerator::CreateSerialByte()
{
    U32 samples_per_bit = mSimulationSampleRateHz / mSettings->mBitRate;

    U8 byte = mSerialText[ mStringIndex ];
    mStringIndex++;
    if( mStringIndex == mSerialText.size() )
        mStringIndex = 0;

    //we're currenty high
    //let's move forward a little
    mSerialSimulationData.Advance( samples_per_bit * 10 );

    mSerialSimulationData.Transition();  //low-going edge for start bit
    mSerialSimulationData.Advance( samples_per_bit );  //add start bit time

    U8 mask = 0x1 << 7;
    for( U32 i=0; i<8; i++ )
    {
        if( ( byte & mask ) != 0 )
            mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
        else
            mSerialSimulationData.TransitionIfNeeded( BIT_LOW );

        mSerialSimulationData.Advance( samples_per_bit );
        mask = mask >> 1;
    }

    mSerialSimulationData.TransitionIfNeeded( BIT_HIGH ); //we need to end high

    //lets pad the end a bit for the stop bit:
    mSerialSimulationData.Advance( samples_per_bit );
}
#endif

void Xlink2WSimulationDataGenerator::create_token(unsigned token, unsigned delay)
{
    int i = 0;
    
    for (i=0; i<9; i++) {
        if ( (token & 0x0100) == 0x0100) {
            w1SimData->Transition();
        } else {
            w0SimData->Transition();
        }
        
        token <<= 1;
        xlink_channels.AdvanceAll(delay);    
    }
    
    w1SimData->TransitionIfNeeded(BIT_LOW);
    w0SimData->TransitionIfNeeded(BIT_LOW);    
}
