#ifndef XLINK2W_ANALYZER_SETTINGS
#define XLINK2W_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class Xlink2WAnalyzerSettings : public AnalyzerSettings
{
public:
    Xlink2WAnalyzerSettings();
    virtual ~Xlink2WAnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings( const char* settings );
    virtual const char* SaveSettings();

    
    Channel chanW0;
    Channel chanW1;

protected:
    std::auto_ptr< AnalyzerSettingInterfaceChannel >    chanW0Interface;
    std::auto_ptr< AnalyzerSettingInterfaceChannel >    chanW1Interface;
    //std::auto_ptr< AnalyzerSettingInterfaceInteger >    mBitRateInterface;
};

#endif //XLINK2W_ANALYZER_SETTINGS
