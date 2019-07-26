


#ifndef GAIN_HH
#define GAIN_HH

#include <cstddef>
#include <cstring>
#include <fmi2Functions.h>
#include <stdint.h>
#include <stdlib.h>



#define MODEL_GUID "affdc61d-653c-4476-8ad3-91b2e55264f3"  // must be the same in the modelDescription.xml


#ifdef __cplusplus
extern "C" {
#endif




struct ModelInstance{


    //----------------------------------------------
    // GAIN INTERFACE
    //----------------------------------------------
    uint8_t data_rdy;   // data flag
    uint8_t result_rdy; //  result is ready flag
    int data;           // from memory
    // Missing result port!
    int result;


    char * instanceName;
    fmi2Real time;
    const fmi2CallbackFunctions * functions;
    fmi2Boolean logEvents;
    fmi2Boolean logSingularLinearSystems;
    fmi2Boolean logNonlinearSystems;
    fmi2Boolean logDynamicStateSelection;
    fmi2Boolean logStatusWarning;
    fmi2Boolean logStatusDiscard;
    fmi2Boolean logStatusError;
    fmi2Boolean logStatusFatal;
    fmi2Boolean logStatusPending;
    fmi2Boolean logAll;

};
static 
void freeInstance( ModelInstance * comp );


fmi2Component fmi2Instantiate( fmi2String instanceName, fmi2Type fmuType, fmi2String
     fmuGUID, fmi2String fmuResourceLocation, const fmi2CallbackFunctions * functions,
     fmi2Boolean visible, fmi2Boolean loggingOn );


fmi2Status fmi2SetupExperiment( fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real
     tolerance, fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime
     );


fmi2Status fmi2EnterInitializationMode( fmi2Component c );


fmi2Status fmi2ExitInitializationMode( fmi2Component c );


fmi2Status fmi2Terminate( fmi2Component c );


fmi2Status fmi2Reset( fmi2Component c );


void fmi2FreeInstance( fmi2Component c );

const 
char * fmi2GetVersion();

const 
char * fmi2GetTypesPlatform();


fmi2Status fmi2SetDebugLogging( fmi2Component c, fmi2Boolean loggingOn, size_t nCategories,
     const fmi2String * categories );


fmi2Status fmi2GetReal( fmi2Component c, const fmi2ValueReference * vr, size_t nvr,
     fmi2Real * value );


fmi2Status fmi2GetInteger( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2Integer * value );


fmi2Status fmi2GetBoolean( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2Boolean * value );


fmi2Status fmi2GetString( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2String * value );


fmi2Status fmi2SetReal( fmi2Component c, const fmi2ValueReference * vr, size_t nvr,
     const fmi2Real * value );


fmi2Status fmi2SetInteger( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2Integer * value );


fmi2Status fmi2SetBoolean( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2Boolean * value );


fmi2Status fmi2SetString( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2String * value );


fmi2Status fmi2CancelStep( fmi2Component c );


fmi2Status fmi2DoStep( fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real
     communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint );


fmi2Status fmi2GetStatus( fmi2Component c, const fmi2StatusKind s, fmi2Status *
     value );


fmi2Status fmi2GetRealStatus( fmi2Component c, const fmi2StatusKind s, fmi2Real
     * value );


fmi2Status fmi2GetIntegerStatus( fmi2Component c, const fmi2StatusKind s, fmi2Integer
     * value );


fmi2Status fmi2GetBooleanStatus( fmi2Component c, const fmi2StatusKind s, fmi2Boolean
     * value );


fmi2Status fmi2GetStringStatus( fmi2Component c, const fmi2StatusKind s, fmi2String
     * value );


fmi2Status fmi2SetRealInputDerivatives( fmi2Component c, const fmi2ValueReference
     * vr, size_t nvr, const fmi2Integer * order, const fmi2Real * value );


fmi2Status fmi2GetRealOutputDerivatives( fmi2Component c, const fmi2ValueReference
     * vr, size_t nvr, const fmi2Integer * order, fmi2Real * value );


#ifdef __cplusplus
} // end extern "C"
#endif


#endif
