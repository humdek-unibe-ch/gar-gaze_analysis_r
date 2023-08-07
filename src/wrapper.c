#include "wrapper.h"
#include <Rdefines.h>

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
    SEXP item;
    SEXP val;
    gac_filter_parameter_t params;

    gac_get_filter_parameter_default( &params );

    if( Rf_isNewList( r_params ) )
    {
        // gap
        item = VECTOR_ELT( r_params, 0 );
        if( Rf_isNewList( item ) )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.gap.max_gap_length = Rf_asReal( val );
            }
            val = VECTOR_ELT( item, 1 );
            if( Rf_isNumber( val ) )
            {
                params.gap.sample_period = Rf_asReal( val );
            }
        }
        // noise
        item = VECTOR_ELT( r_params, 1 );
        if( Rf_isNewList( item ) )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.noise.mid_idx = Rf_asInteger( val );
            }
        }
        // saccade
        item = VECTOR_ELT( r_params, 2 );
        if( Rf_isNewList( item ) )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.saccade.velocity_threshold = Rf_asReal( val );
            }
        }
        // fixation
        item = VECTOR_ELT( r_params, 3 );
        if( Rf_isNewList( item ) )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.fixation.duration_threshold = Rf_asReal( val );
            }
            val = VECTOR_ELT( item, 1 );
            if( Rf_isNumber( val ) )
            {
                params.fixation.dispersion_threshold = Rf_asReal( val );
            }
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

/******************************************************************************/
SEXP gar_create_fixation_frame( gac_fixation_t* fixations, int count )
{
    int i;
    const char* names[] = { "px", "py", "pz", "duration", "timestamp", "" };
    SEXP px = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP py = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP pz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP duration = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP timestamp = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP df;

    for( i = 0; i < count; i++ )
    {
        REAL( px )[i] = fixations[i].point[0];
        REAL( py )[i] = fixations[i].point[1];
        REAL( pz )[i] = fixations[i].point[2];
        REAL( duration )[i] = fixations[i].duration;
        REAL( timestamp )[i] = fixations[i].timestamp;
    }

    df = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SET_VECTOR_ELT( df, 0, px );
    SET_VECTOR_ELT( df, 1, py );
    SET_VECTOR_ELT( df, 2, pz );
    SET_VECTOR_ELT( df, 3, duration );
    SET_VECTOR_ELT( df, 4, timestamp );

    SET_CLASS( df, mkString( "data.frame" ) );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -count );
    setAttrib( df, R_RowNamesSymbol, rownames );

    UNPROTECT( 7 );

    return df;
}

/******************************************************************************/
SEXP gar_create_saccade_frame( gac_saccade_t* saccades, int count )
{
    int i;
    const char* names[] = { "startx", "starty", "startz", "destx", "desty", "destz", "duration", "timestamp", "" };
    SEXP startx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP starty = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP startz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP desty = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP duration = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP timestamp = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP df;

    for( i = 0; i < count; i++ )
    {
        REAL( startx )[i] = saccades[i].point_start[0];
        REAL( starty )[i] = saccades[i].point_start[1];
        REAL( startz )[i] = saccades[i].point_start[2];
        REAL( destx )[i] = saccades[i].point_dest[0];
        REAL( desty )[i] = saccades[i].point_dest[1];
        REAL( destz )[i] = saccades[i].point_dest[2];
        REAL( duration )[i] = saccades[i].duration;
        REAL( timestamp )[i] = saccades[i].timestamp;
    }

    df = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SET_VECTOR_ELT( df, 0, startx );
    SET_VECTOR_ELT( df, 1, starty );
    SET_VECTOR_ELT( df, 2, startz );
    SET_VECTOR_ELT( df, 3, destx );
    SET_VECTOR_ELT( df, 4, desty );
    SET_VECTOR_ELT( df, 5, destz );
    SET_VECTOR_ELT( df, 6, duration );
    SET_VECTOR_ELT( df, 7, timestamp );

    SET_CLASS( df, mkString( "data.frame" ) );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -count );
    setAttrib( df, R_RowNamesSymbol, rownames );

    UNPROTECT( 10 );

    return df;
}

/******************************************************************************/
SEXP gar_parse( SEXP ptr, SEXP px, SEXP py, SEXP pz, SEXP ox,
        SEXP oy, SEXP oz, SEXP timestamp )
{
    SEXP ret, fixations, saccades;
    const char* names[] = { "fixations", "saccades", "" };
    void* h;
    int len, i;
    double *ppx, *ppy, *ppz, *pox, *poy, *poz, *ptimestamp;
    bool res;
    gac_fixation_t fixation[1000];
    int fixation_count = 0;
    gac_saccade_t saccade[1000];
    int saccade_count = 0;

    CHECK_GAC_HANDLER( ptr );

    if( !Rf_isReal( px )
        || !Rf_isReal( py )
        || !Rf_isReal( pz )
        || !Rf_isReal( ox )
        || !Rf_isReal( oy )
        || !Rf_isReal( oz )
        || !Rf_isReal( timestamp ) )
    {
        error( "all vectors need to be type double" );
        return R_NilValue;
    }

    len = Rf_length( timestamp );

    if( Rf_length( px ) != len
        || Rf_length( py ) != len
        || Rf_length( pz ) != len
        || Rf_length( ox ) != len
        || Rf_length( ox ) != len
        || Rf_length( ox ) != len )
    {
        error( "all vectors need to be of the same length" );
        return R_NilValue;
    }

    h = R_ExternalPtrAddr( ptr );

    ppx = REAL( px );
    ppy = REAL( py );
    ppz = REAL( pz );
    pox = REAL( ox );
    poy = REAL( oy );
    poz = REAL( oz );
    ptimestamp = REAL( timestamp );
    for( i = 0; i < len; i++ )
    {
        gac_sample_window_update( h,
                ( float )pox[i], ( float )poy[i], ( float )poz[i],
                ( float )ppx[i], ( float )ppy[i], ( float )ppz[i],
                ptimestamp[i] );
        res = gac_sample_window_fixation_filter( h, &fixation[fixation_count] );
        if( res )
        {
            fixation_count++;
        }
        res = gac_sample_window_saccade_filter( h, &saccade[saccade_count] );
        if( res )
        {
            saccade_count++;
        }
    }

    fixations = gar_create_fixation_frame( fixation, fixation_count );
    saccades = gar_create_saccade_frame( saccade, saccade_count );

    ret = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SET_VECTOR_ELT( ret, 0, fixations );
    SET_VECTOR_ELT( ret, 1, saccades );
    UNPROTECT( 1 );

    return ret;
}
