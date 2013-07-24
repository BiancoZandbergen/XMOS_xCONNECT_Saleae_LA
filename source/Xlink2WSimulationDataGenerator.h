#ifndef XLINK2W_SIMULATION_DATA_GENERATOR
#define XLINK2W_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class Xlink2WAnalyzerSettings;

class Xlink2WSimulationDataGenerator
{
public:
    Xlink2WSimulationDataGenerator();
    ~Xlink2WSimulationDataGenerator();

    void Initialize( U32 simulation_sample_rate, Xlink2WAnalyzerSettings* settings );
    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
    Xlink2WAnalyzerSettings* mSettings;
    U32 mSimulationSampleRateHz;

protected:
    //void CreateSerialByte();
    void create_token(unsigned token, unsigned delay);
    std::string mSerialText;
    U32 mStringIndex;
    
    SimulationChannelDescriptorGroup xlink_channels;
    SimulationChannelDescriptor *w0SimData;
    SimulationChannelDescriptor *w1SimData;

};
#endif //XLINK2W_SIMULATION_DATA_GENERATOR