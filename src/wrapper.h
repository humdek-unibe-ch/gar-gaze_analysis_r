#include <Rinternals.h>
#include "gac.h"

/**
 *
 */
SEXP gar_init( void );

/**
 *
 */
SEXP gar_create( SEXP r_params );

/**
 *
 */
SEXP gar_destroy( SEXP ptr );

/**
 *
 */
SEXP gar_create_filter_parameter( gac_filter_parameter_t* params );

/**
 *
 */
SEXP gar_get_filter_parameter( SEXP ptr );

/**
 *
 */
SEXP gar_get_filter_parameter_default();
