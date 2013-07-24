#include "Xlink2WAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


Xlink2WAnalyzerSettings::Xlink2WAnalyzerSettings()
:   chanW0( UNDEFINED_CHANNEL ),
    chanW1( UNDEFINED_CHANNEL )
{
    chanW0Interface.reset( new AnalyzerSettingInterfaceChannel() );
    chanW0Interface->SetTitleAndTooltip( "XLINK Wire 0", "XLINK Wire 0" );
    chanW0Interface->SetChannel( chanW0 );
    
    chanW1Interface.reset( new AnalyzerSettingInterfaceChannel() );
    chanW1Interface->SetTitleAndTooltip( "XLINK Wire 1", "XLINK Wire 1" );
    chanW1Interface->SetChannel( chanW1 );

    AddInterface( chanW0Interface.get() );
    AddInterface( chanW1Interface.get() );

    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    ClearChannels();
    AddChannel( chanW0, "XLINK Wire 0", false );
    AddChannel( chanW1, "XLINK Wire 1", false );

}

Xlink2WAnalyzerSettings::~Xlink2WAnalyzerSettings()
{
}

bool Xlink2WAnalyzerSettings::SetSettingsFromInterfaces()
{
    chanW0 = chanW0Interface->GetChannel();
    chanW1 = chanW1Interface->GetChannel();

    ClearChannels();
    AddChannel( chanW0, "XLINK Wire 0", true );
    AddChannel( chanW1, "XLINK Wire 1", true );

    return true;
}

void Xlink2WAnalyzerSettings::UpdateInterfacesFromSettings()
{
    chanW0Interface->SetChannel( chanW0 );
    chanW1Interface->SetChannel( chanW1 );
}

void Xlink2WAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    text_archive >> chanW0;
    text_archive >> chanW1;

    ClearChannels();
    AddChannel( chanW0, "XLINK Wire 0", true );
    AddChannel( chanW1, "XLINK Wire 1", true );

    UpdateInterfacesFromSettings();
}

const char* Xlink2WAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << chanW0;
    text_archive << chanW1;

    return SetReturnString( text_archive.GetString() );
}
