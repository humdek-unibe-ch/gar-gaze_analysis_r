#include "wrapper.h"

static SEXP gac_type_tag;

#define CHECK_GAC_HANDLER(h) do { \
    if( TYPEOF( h ) != EXTPTRSXP || R_ExternalPtrTag( h ) != gac_type_tag ) \
        error( "bad gac handler" ); \
} while( 0 )

/******************************************************************************/
SEXP gar_init( void )
{
    gac_type_tag = install( "GAC_TYPE_TAG" );
    return R_NilValue;
}

/******************************************************************************/
SEXP gar_create( SEXP r_params )
{
    void* h;
    SEXP ptr;
    SEXP gap;
    gac_filter_parameter_t params;

    gac_get_filter_parameter_default( &params );

    if( Rf_isExpression( r_params ) )
    {
        gap = VECTOR_ELT( r_params, 0 );
        if( Rf_isExpression( gap ) )
        {
            gap = VECTOR_ELT( r_params, 0 );
        }
    }

    h = gac_create( &params );
    if( h == NULL )
    {
        return R_NilValue;
    }

    ptr = R_MakeExternalPtr( h, gac_type_tag, R_NilValue );
    R_RegisterCFinalizer( ptr, ( R_CFinalizer_t )gar_destroy );

    return ptr;
}

/******************************************************************************/
SEXP gar_destroy( SEXP ptr )
{
    void* h;

    CHECK_GAC_HANDLER( ptr );

    h = R_ExternalPtrAddr( ptr );
    gac_destroy( h );

    return R_NilValue;
}

/******************************************************************************/
SEXP gar_create_filter_parameter( gac_filter_parameter_t* params )
{
    SEXP gap, noise, saccade, fixation, list;
    const char* gap_names[] = { "max_gap_length", "sample_period", "" };
    const char* noise_names[] = { "mid_idx", "" };
    const char* saccade_names[] = { "velocity_threshold", "" };
    const char* fixation_names[] = { "duration_threshold", "dispersion_threshold", "" };
    const char* names[] = { "gap", "noise", "saccade", "fixation", "" };

    gap = PROTECT( Rf_mkNamed( VECSXP, gap_names ) );
    SET_VECTOR_ELT( gap, 0, Rf_ScalarReal( params->gap.max_gap_length ) );
    SET_VECTOR_ELT( gap, 1, Rf_ScalarReal( params->gap.sample_period ) );
    noise = PROTECT( Rf_mkNamed( VECSXP, noise_names ) );
    SET_VECTOR_ELT( noise, 0, Rf_ScalarReal( params->noise.mid_idx ) );
    saccade = PROTECT( Rf_mkNamed( VECSXP, saccade_names ) );
    SET_VECTOR_ELT( saccade, 0, Rf_ScalarReal( params->saccade.velocity_threshold ) );
    fixation = PROTECT( Rf_mkNamed( VECSXP, fixation_names ) );
    SET_VECTOR_ELT( fixation, 0, Rf_ScalarReal( params->fixation.duration_threshold ) );
    SET_VECTOR_ELT( fixation, 1, Rf_ScalarReal( params->fixation.dispersion_threshold ) );

    list = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SET_VECTOR_ELT( list, 0, gap );
    SET_VECTOR_ELT( list, 1, noise );
    SET_VECTOR_ELT( list, 2, saccade );
    SET_VECTOR_ELT( list, 3, fixation );
    UNPROTECT(5);

    return list;
}

/******************************************************************************/
SEXP gar_get_filter_parameter( SEXP ptr )
{
    void* h = R_ExternalPtrAddr( ptr );
    gac_filter_parameter_t params;

    gac_get_filter_parameter( h, &params );
    return gar_create_filter_parameter( &params );
}

/******************************************************************************/
SEXP gar_get_filter_parameter_default()
{
    gac_filter_parameter_t params;

    gac_get_filter_parameter_default( &params );
    return gar_create_filter_parameter( &params );
}
