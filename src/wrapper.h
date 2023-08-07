#include <Rinternals.h>
#include "gac.h"

/**
 * Allocate the gac handler.
 *
 * @param r_params
 *  An R named list holding the parameters for the gac handler.
 *  If no parameter is privided, default parameters are used.
 * @return
 *  An external pointer structure which points to the gac handler.
 */
SEXP gar_create( SEXP r_params );

/**
 * Allocate the filter parameter R structure.
 *
 * @param params
 *  A pointer to a gac filter parameter structure holding the values to assign
 *  to the R structure.
 * @return
 *  An R named list holding the assigned gac filter parameters.
 */
SEXP gar_create_filter_parameter( gac_filter_parameter_t* params );

/**
 * Given a list of fixations, create a data frame where each row represents a
 * fixation and each column a property of all fixations.
 *
 * @param fixations
 *  A pointer to the fixation list.
 * @param count
 *  The number of fixations.
 * @return
 *  The allocated fixation data frame.
 */
SEXP gar_create_fixation_frame( gac_fixation_t* fixations, uint32_t count );

/**
 * Given a list of saccades, create a data frame where each row represents a
 * saccade and each column a property of all saccades.
 *
 * @param saccades
 *  A pointer to the saccade list.
 * @param count
 *  The number of saccades.
 * @return
 *  The allocated saccade data frame.
 */
SEXP gar_create_saccade_frame( gac_saccade_t* saccades, uint32_t count );

/**
 * Destroy the gac handler.
 *
 * @param ptr
 *  The external pointer structure pointing to the gac handler.
 * @return
 *  R_NilValue
 */
SEXP gar_destroy( SEXP ptr );

/**
 * Return the current parameter of the gac handler.
 *
 * @param ptr
 *  An external pointer structure pointing to the gac handler.
 * @return
 *  A named list holding the filter parameter values.
 */
SEXP gar_get_filter_parameter( SEXP ptr );

/**
 * Return the default filter parameters.
 *
 * @return
 *  A named list holding the default filter parameter values.
 */
SEXP gar_get_filter_parameter_default();

/**
 * Initialize the gac type tag. It is initialized by calling the C level
 * initialization function in the package `.First.lib` function.
 *
 * @return
 *  R_NilValue
 */
SEXP gar_init( void );

/**
 * Search for fixations and saccades in a set of data samples.
 * The sample data is passed as several vectors where each must have the same
 * length and must be of of type double.
 *
 * @param ptr
 *  An external pointer structure pointing to the gac handler.
 * @param px
 *  A vector holding the x coordinate of the gaze position.
 * @param py
 *  A vector holding the y coordinate of the gaze position.
 * @param pz
 *  A vector holding the z coordinate of the gaze position.
 * @param ox
 *  A vector holding the x coordinate of the gaze origin.
 * @param oy
 *  A vector holding the z coordinate of the gaze origin.
 * @param oy
 *  A vector holding the z coordinate of the gaze origin.
 * @param timestamp
 *  A vector holding the relative timestamps of the samples.
 * @return
 *  A named list holding two data frames, one holding fixations and one holding
 *  saccades.
 */
SEXP gar_parse( SEXP ptr, SEXP px, SEXP py, SEXP pz, SEXP ox,
        SEXP oy, SEXP oz, SEXP timestamp );
