#include "Xlink2WAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "Xlink2WAnalyzer.h"
#include "Xlink2WAnalyzerSettings.h"
#include <iostream>
#include <fstream>
#include <cstring>

Xlink2WAnalyzerResults::Xlink2WAnalyzerResults( Xlink2WAnalyzer* analyzer, Xlink2WAnalyzerSettings* settings )
:   AnalyzerResults(),
    mSettings( settings ),
    mAnalyzer( analyzer )
{
}

Xlink2WAnalyzerResults::~Xlink2WAnalyzerResults()
{
}

void Xlink2WAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
    ClearResultStrings();
    Frame frame = GetFrame( frame_index );
    char bubblestr[128];
    char temp[64];
    
    if (display_base == ASCII) {
        bool control_token;
        U64 data;
        
        if ((frame.mData1 & 0x01) == 0x01) control_token = true;
        else control_token = false;
        
        if (control_token) {
            //AddResultString("C ");
            //bubblestr = "C ";
            strcpy(bubblestr, "C ");
        } else {
            //AddResultString("D ");
            //bubblestr = "D ";
            strcpy(bubblestr, "D ");

        }
        
        data = frame.mData1;
        data >>= 1;

        AnalyzerHelpers::GetNumberString( data, Hexadecimal, 8, temp, 64 );
        AddResultString(bubblestr, temp);

    } else {

        AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 9, bubblestr, 128 );
        AddResultString( bubblestr );
    }

}

void Xlink2WAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
    std::ofstream file_stream( file, std::ios::out );

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    file_stream << "Time [s],Value" << std::endl;

    U64 num_frames = GetNumFrames();
    for( U32 i=0; i < num_frames; i++ )
    {
        Frame frame = GetFrame( i );

        char time_str[128];
        AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

        char number_str[128];
        AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 9, number_str, 128 );

        file_stream << time_str << "," << number_str << std::endl;

        if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
        {
            file_stream.close();
            return;
        }
    }

    file_stream.close();
}

void Xlink2WAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
    Frame frame = GetFrame( frame_index );
    ClearResultStrings();

    char number_str[128];
    AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
    AddResultString( number_str );
}

void Xlink2WAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}

void Xlink2WAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}