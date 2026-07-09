#ifndef TLEVISION_LOAD_TIMESPAN_H
#define TLEVISION_LOAD_TIMESPAN_H

// Standard libraries

// Local libraries
#include "file-io/internal-products/application_output/application_output.h"
#include "file-io/internal-products/parameter-evolution-file/parameter_evolution_file.h"
#include "utilities/constants/constants.h"
#include "utilities/logging/log/log.h"
#include "tlevision_interfaces.h"
#include "reference-systems/time-systems/time-formats/time_formats.h"
#include "mathematics-library/numerical-methods/interpolation/interpolation.h"

// Variable and Macro definitions

// Function prototypes

/**
 * Loads the timespan for tlevision.
 *
 * @param timespan The timespan to load.
 * @param pev_with_timespan The parameter evolution file to load the timespan from.
 * @param inputs The tlevision inputs.
 * @param execution_settings The execution settings.
 * @return The status code.
 */
StatusCode load_timespan(
        // Outputs
        ParameterEvolution *timespan,
        // Inputs
        const ParameterEvolutionFile *pev_with_timespan,
        const tlevision_inputs_t *inputs,
        const ExecutionSettings *execution_settings);

#endif
