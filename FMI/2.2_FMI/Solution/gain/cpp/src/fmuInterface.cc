
#include "fmuInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
#define GAIN_VALUE 10

//-----------------------------------------------------------------------------------------
// GET INTEGER
//-----------------------------------------------------------------------------------------

fmi2Status fmi2GetInteger( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2Integer * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    size_t i = 0L;
    bool isError = false;
  
    for (i = 0L; i < nvr; i = i + 1L)
    {
        switch ( vr[i] )
        {
            case 0L:
            
                value[i] = comp->data;
            
            break;
            
            // Missing get result port here...
            case 1L:
            	value[i] = comp->result;
            break;

            default:
                break;
        };
        if (comp->logAll == fmi2True)
        {
            comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                     fmi2OK, "logAll", "fmi2GetInteger: Getting Integer value of"
                    " ID: %u = %d", vr[i], value[i] );
        };
    }

    return fmi2OK;
}


//-----------------------------------------------------------------------------------------
// GET BOOLEAN
//-----------------------------------------------------------------------------------------

fmi2Status fmi2GetBoolean( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2Boolean * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    
    size_t i = 0L;
    
 
    for (i = 0L; i < nvr; i = i + 1L)
    {
        switch (vr[i])
        {
            
            case 0L:

                value[i] = comp->data_rdy ;

            break;

            case 1L:

                value[i] = comp->result_rdy;
            break;

            break;
            default:
                break;
        };
        if (comp->logAll == fmi2True)
        {
            comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                     fmi2OK, "logAll", "fmi2GetBoolean: Getting Boolean value of"
                    " ID: %u = %d", vr[i], value[i] );
        };
    }

    return fmi2OK;
}




//-----------------------------------------------------------------------------------------
// SET INTEGER
//-----------------------------------------------------------------------------------------

fmi2Status fmi2SetInteger( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2Integer * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    size_t i = 0L;
 
    for (i = 0L; i < nvr; i = i + 1L)
    {
        switch (vr[i])
        {
            case 0L:

                comp->data = value[i];
                
            break;
            case 1L:
            	comp->result = value[i];
            default:
            break;
        };
        if (comp->logAll == fmi2True)
        {
            comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                     fmi2OK, "logAll", "fmi2SetInteger: Setting Integer value of"
                    " ID: %u = %d", vr[i], value[i] );
        };
    }

    return fmi2OK;
}



//-----------------------------------------------------------------------------------------
// SET BOOLEAN
//-----------------------------------------------------------------------------------------

fmi2Status fmi2SetBoolean( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2Boolean * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    
    size_t i = 0L;

    for (i = 0L; i < nvr; i = i + 1L)
    {
        switch ( vr[i] )
        {
            case 0L:
                
                comp->data_rdy = value[i];
            
            break;

            case 1L:

                comp->result_rdy = value[i];

            break;

            default:
            break;
        };
        if (comp->logAll == fmi2True)
        {
            comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                     fmi2OK, "logAll", "fmi2SetBoolean: Setting Boolean value of"
                    " ID: %u = %d", vr[i], value[i] );
        };
    }

    return fmi2OK;
}





//---------------------------------------------------------------------------------------
// FMI2DOSTEP
//---------------------------------------------------------------------------------------

fmi2Status fmi2DoStep( fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real
     communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );

   
    
    //------------------------//------------------------
    // SIMULATION STEP HERE!
    //------------------------//------------------------
    // If data are ready, gain the data.

    if(comp->data_rdy == fmi2True)
    {
    	comp->result = comp->data * GAIN_VALUE;
    	comp->result_rdy = fmi2True;
    }


    //  Missing Gain Functionality.... 

    // if new data are ready let's gain!  
    // if not.... ZERO

    //------------------------//------------------------
    //------------------------//------------------------

    // LOGGING

    if (comp->logAll == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2OK, "logAll", "fmi2DoStep: starting time=%f; step size=%f.",
                 comp->time, communicationStepSize );
    }
    comp->time = comp->time + communicationStepSize;
    return fmi2OK;
}



fmi2Component fmi2Instantiate( fmi2String instanceName, fmi2Type fmuType, fmi2String
     fmuGUID, fmi2String fmuResourceLocation, const fmi2CallbackFunctions * functions,
     fmi2Boolean visible, fmi2Boolean loggingOn )
{
    ModelInstance * comp = nullptr;
    if (instanceName == nullptr || strlen( instanceName ) == 0L)
    {
        if (fmi2True == loggingOn)
        {
            functions->logger( functions->componentEnvironment, instanceName, fmi2Error,
                     "logStatusError", "fmi2Instantiate: Missing instance name."
                 );
        }
        return nullptr;
    }
    if (fmuGUID == nullptr || strlen( fmuGUID ) == 0L)
    {
        if (fmi2True == loggingOn)
        {
            functions->logger( functions->componentEnvironment, instanceName, fmi2Error,
                     "logStatusError", "fmi2Instantiate: Missing GUID." );
        }
        return nullptr;
    }
    if (strcmp( fmuGUID, MODEL_GUID ) != 0L)
    {
        if (fmi2True == loggingOn)
        {
            functions->logger( functions->componentEnvironment, instanceName, fmi2Error,
                     "logStatusError", "fmi2Instantiate: Wrong GUID." );
        }
        return nullptr;
    }
    comp = reinterpret_cast< ModelInstance * >( functions->allocateMemory( static_cast<
             size_t >( 1L ), sizeof( *comp ) ) );
    if (comp == nullptr)
    {
        if (fmi2True == loggingOn)
        {
            functions->logger( functions->componentEnvironment, instanceName, fmi2Error,
                     "logStatusError", "fmi2Instantiate: Cannot allocate model instance."
                 );
        }
        return nullptr;
    }
    comp->instanceName = reinterpret_cast< char * >( functions->allocateMemory(
         strlen( instanceName ) + 1L, sizeof( '\0' ) ) );
    if (comp->instanceName == nullptr)
    {
        if (fmi2True == loggingOn)
        {
            functions->logger( functions->componentEnvironment, instanceName, fmi2Error,
                     "logStatusError", "fmi2Instantiate: Cannot allocate model instance"
                    " name." );
        }
        freeInstance( comp );
        return nullptr;
    }


    strcpy( comp->instanceName, instanceName );
    comp->logEvents = loggingOn;
    comp->logSingularLinearSystems = loggingOn;
    comp->logNonlinearSystems = loggingOn;
    comp->logDynamicStateSelection = loggingOn;
    comp->logStatusWarning = loggingOn;
    comp->logStatusDiscard = loggingOn;
    comp->logStatusError = loggingOn;
    comp->logStatusFatal = loggingOn;
    comp->logStatusPending = loggingOn;
    comp->logAll = loggingOn;
    comp->functions = functions;
    comp->time = 0.0;
    if (fmi2True == loggingOn)
    {
        functions->logger( functions->componentEnvironment, instanceName, fmi2OK,
                 "logAll", "fmi2Instantiate: component correctly instantiated."
             );
    }
    return static_cast< fmi2Component >( comp );
}



fmi2Status fmi2SetupExperiment( fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real
     tolerance, fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
   

    if (comp->logAll == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2OK, "logAll", "fmi2SetupExperiment: Initializing model." );
    }
    comp->time = startTime;

    
    return fmi2OK;
}



fmi2Status fmi2EnterInitializationMode( fmi2Component c )
{
    return fmi2OK;
}



fmi2Status fmi2ExitInitializationMode( fmi2Component c )
{
    return fmi2OK;
}



fmi2Status fmi2Terminate( fmi2Component c )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
  
    if (comp->logAll == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2OK, "logAll", "fmi2Terminate: terminating model." );
    }
    
    return fmi2OK;
}



fmi2Status fmi2Reset( fmi2Component c )
{

    // Unsupported
    return fmi2Error;
}


void fmi2FreeInstance( fmi2Component c )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );

    if (comp == nullptr)
    {
        return;
    }

    if (comp->logAll == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2OK, "logAll", "fmi2FreeInstance: freeing model." );
    }

    freeInstance( comp );
}


const 
char * fmi2GetVersion()
{
    return fmi2Version;
}


const 
char * fmi2GetTypesPlatform()
{
    return fmi2TypesPlatform;
}



fmi2Status fmi2SetDebugLogging( fmi2Component c, fmi2Boolean loggingOn, size_t nCategories,
     const fmi2String * categories )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    size_t i = 0L;
    if (nCategories == 0L)
    {
        comp->logEvents = loggingOn;
        comp->logSingularLinearSystems = loggingOn;
        comp->logNonlinearSystems = loggingOn;
        comp->logDynamicStateSelection = loggingOn;
        comp->logStatusWarning = loggingOn;
        comp->logStatusDiscard = loggingOn;
        comp->logStatusError = loggingOn;
        comp->logStatusFatal = loggingOn;
        comp->logStatusPending = loggingOn;
        comp->logAll = loggingOn;
        return fmi2OK;
    }
    comp->logEvents = fmi2False;
    comp->logSingularLinearSystems = fmi2False;
    comp->logNonlinearSystems = fmi2False;
    comp->logDynamicStateSelection = fmi2False;
    comp->logStatusWarning = fmi2False;
    comp->logStatusDiscard = fmi2False;
    comp->logStatusError = fmi2False;
    comp->logStatusFatal = fmi2False;
    comp->logStatusPending = fmi2False;
    comp->logAll = fmi2False;
    for (i = 0L; i < nCategories; i = i + 1L)
    {
        if (strcmp( categories[i], "logAll" ) == 0L)
        {
            comp->logAll = loggingOn;
        }
        else if (strcmp( categories[i], "logStatusPending" ) == 0L)
        {
            comp->logStatusPending = loggingOn;
        }
        else if (strcmp( categories[i], "logStatusFatal" ) == 0L)
        {
            comp->logStatusFatal = loggingOn;
        }
        else if (strcmp( categories[i], "logStatusError" ) == 0L)
        {
            comp->logStatusError = loggingOn;
        }
        else if (strcmp( categories[i], "logStatusDiscard" ) == 0L)
        {
            comp->logStatusDiscard = loggingOn;
        }
        else if (strcmp( categories[i], "logStatusWarning" ) == 0L)
        {
            comp->logStatusWarning = loggingOn;
        }
        else if (strcmp( categories[i], "logDynamicStateSelection" ) == 0L)
        {
            comp->logDynamicStateSelection = loggingOn;
        }
        else if (strcmp( categories[i], "logNonlinearSystems" ) == 0L)
        {
            comp->logNonlinearSystems = loggingOn;
        }
        else if (strcmp( categories[i], "logSingularLinearSystems" ) == 0L)
        {
            comp->logSingularLinearSystems = loggingOn;
        }
        else if (strcmp( categories[i], "logEvents" ) == 0L)
        {
            comp->logEvents = loggingOn;
        }
        else
        {
            return fmi2Warning;
        };
    }

    return fmi2OK;
}



fmi2Status fmi2GetReal( fmi2Component c, const fmi2ValueReference * vr, size_t nvr,
     fmi2Real * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusWarning == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Warning, "logStatusWarning", "fmiGetReal: NO Real ports." 
            );
    }
    return fmi2OK;
}




fmi2Status fmi2GetString( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, fmi2String * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusWarning == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Warning, "logStatusWarning", "fmi2GetString: NO String ports."
             );
    }
    return fmi2OK;
}



fmi2Status fmi2SetReal( fmi2Component c, const fmi2ValueReference * vr, size_t nvr,
     const fmi2Real * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusWarning == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Warning, "logStatusWarning", "fmiSetReal: NO Real ports." 
            );
    }
    return fmi2Warning;
}




fmi2Status fmi2SetString( fmi2Component c, const fmi2ValueReference * vr, size_t
     nvr, const fmi2String * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusWarning == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Warning, "logStatusWarning", "fmiSetString: NO String ports."
             );
    }
    return fmi2Warning;
}



fmi2Status fmi2CancelStep( fmi2Component c )
{

    // Unsupported
    return fmi2Error;
}




fmi2Status fmi2GetStatus( fmi2Component c, const fmi2StatusKind s, fmi2Status *
     value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusError == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Error, "logStatusError", "fmi2GetStatus: invalid status request."
             );
    }
    return fmi2Error;
}


fmi2Status fmi2GetRealStatus( fmi2Component c, const fmi2StatusKind s, fmi2Real
     * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (fmi2LastSuccessfulTime == s)
    {
        *value = comp->time;
        return fmi2OK;
    }
    if (comp->logStatusError == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Error, "logStatusError", "fmi2GetRealStatus: invalid status"
                " request." );
    }
    return fmi2Error;
}



fmi2Status fmi2GetIntegerStatus( fmi2Component c, const fmi2StatusKind s, fmi2Integer
     * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusError == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Error, "logStatusError", "fmi2GetIntegerStatus: invalid status"
                " request." );
    }
    return fmi2Error;
}



fmi2Status fmi2GetBooleanStatus( fmi2Component c, const fmi2StatusKind s, fmi2Boolean
     * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusError == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Error, "logStatusError", "fmi2GetBooleanStatus: invalid status"
                " request." );
    }
    return fmi2Error;
}



fmi2Status fmi2GetStringStatus( fmi2Component c, const fmi2StatusKind s, fmi2String
     * value )
{
    ModelInstance * comp = reinterpret_cast< ModelInstance * >( c );
    if (comp->logStatusError == fmi2True)
    {
        comp->functions->logger( comp->functions->componentEnvironment, comp->instanceName,
                 fmi2Error, "logStatusError", "fmi2GetStringStatus: invalid status"
                " request." );
    }
    return fmi2Error;
}



fmi2Status fmi2SetRealInputDerivatives( fmi2Component c, const fmi2ValueReference
     * vr, size_t nvr, const fmi2Integer * order, const fmi2Real * value )
{

    // Unsupported
    return fmi2Error;
}



fmi2Status fmi2GetRealOutputDerivatives( fmi2Component c, const fmi2ValueReference
     * vr, size_t nvr, const fmi2Integer * order, fmi2Real * value )
{

    // Unsupported
    return fmi2Error;
}


#ifdef __cplusplus
} // end extern "C"
#endif

void freeInstance( ModelInstance * comp )
{
    ModelInstance * inst = comp;
    const fmi2CallbackFunctions * functions(nullptr);

    if (inst == nullptr)
    {
        return;
    }
    functions = inst->functions;
    functions->freeMemory( reinterpret_cast< void * >( inst->instanceName ) );
    functions->freeMemory( reinterpret_cast< void * >( inst ) );
}
