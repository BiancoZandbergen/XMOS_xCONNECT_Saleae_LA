#ifndef XLINK2W_ANALYZER_RESULTS
#define XLINK2W_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class Xlink2WAnalyzer;
class Xlink2WAnalyzerSettings;

class Xlink2WAnalyzerResults : public AnalyzerResults
{
public:
    Xlink2WAnalyzerResults( Xlink2WAnalyzer* analyzer, Xlink2WAnalyzerSettings* settings );
    virtual ~Xlink2WAnalyzerResults();

    virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
    virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
    virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
    virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
    Xlink2WAnalyzerSettings* mSettings;
    Xlink2WAnalyzer* mAnalyzer;
};

#endif //XLINK2W_ANALYZER_RESULTS
