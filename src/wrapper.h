#include <Rinternals.h>
#include "gac.h"

/**
 *
 */
SEXP gar_create( SEXP r_params );

/**
 *
 */
SEXP gar_create_filter_parameter( gac_filter_parameter_t* params );

/**
 *
 */
SEXP gar_create_fixation_frame( gac_fixation_t* fixations, int count );

/**
 *
 */
SEXP gar_create_saccade_frame( gac_saccade_t* saccades, int count );

/**
 *
 */
SEXP gar_destroy( SEXP ptr );

/**
 *
 */
SEXP gar_get_filter_parameter( SEXP ptr );

/**
 *
 */
SEXP gar_get_filter_parameter_default();

/**
 *
 */
SEXP gar_init( void );

/**
 *
 */
SEXP gar_parse( SEXP ptr, SEXP px, SEXP py, SEXP pz, SEXP ox,
        SEXP oy, SEXP oz, SEXP timestamp );
