/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "wrapper.h"
#include <Rdefines.h>

static SEXP gac_type_tag;

#define GAR_MAX_ITEMS 10000
#define CHECK_GAC_HANDLER(h) do { \
    if( TYPEOF( h ) != EXTPTRSXP || R_ExternalPtrTag( h ) != gac_type_tag ) \
        error( "bad gac handler" ); \
} while( 0 )

/******************************************************************************/
SEXP gar_create( SEXP r_params )
{
    void* h;
    SEXP ptr;
    SEXP item;
    SEXP val;
    gac_filter_parameter_t params;

    gac_get_filter_parameter_default( &params );

    if( TYPEOF( r_params ) == VECSXP )
    {
        // gap
        item = VECTOR_ELT( r_params, 0 );
        if( TYPEOF( item ) == VECSXP )
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
        if( TYPEOF( item ) == VECSXP )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.noise.mid_idx = Rf_asInteger( val );
            }
        }
        // saccade
        item = VECTOR_ELT( r_params, 2 );
        if( TYPEOF( item ) == VECSXP )
        {
            val = VECTOR_ELT( item, 0 );
            if( Rf_isNumber( val ) )
            {
                params.saccade.velocity_threshold = Rf_asReal( val );
            }
        }
        // fixation
        item = VECTOR_ELT( r_params, 3 );
        if( TYPEOF( item ) == VECSXP )
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
SEXP gar_filter_parameter_create( gac_filter_parameter_t* params )
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
SEXP gar_fixation_frame_create( uint32_t count )
{
    const char* names[] = { "sx", "sy", "px", "py", "pz", "duration", "timestamp", "trial_id", "trial_onset", "label", "label_onset", "" };
    SEXP df = PROTECT( Rf_mkNamed( VECSXP, names ) );

    SEXP sx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP sy = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP px = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP py = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP pz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP duration = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP timestamp = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP trial_onset = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP label_onset = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP trial_id = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP label = PROTECT( Rf_allocVector( STRSXP, count ) );

    SET_VECTOR_ELT( df, 0, sx );
    SET_VECTOR_ELT( df, 1, sy );
    SET_VECTOR_ELT( df, 2, px );
    SET_VECTOR_ELT( df, 3, py );
    SET_VECTOR_ELT( df, 4, pz );
    SET_VECTOR_ELT( df, 5, duration );
    SET_VECTOR_ELT( df, 6, timestamp );
    SET_VECTOR_ELT( df, 7, trial_id );
    SET_VECTOR_ELT( df, 8, trial_onset );
    SET_VECTOR_ELT( df, 9, label );
    SET_VECTOR_ELT( df, 10, label_onset );
    UNPROTECT( 11 );

    SET_CLASS( df, mkString( "data.frame" ) );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -count );
    setAttrib( df, R_RowNamesSymbol, rownames );
    UNPROTECT( 1 );

    return df;
}

/******************************************************************************/
void gar_fixation_frame_resize( SEXP df, uint32_t new_length )
{
    SETLENGTH( VECTOR_ELT( df, 0 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 1 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 2 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 3 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 4 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 5 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 6 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 7 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 8 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 9 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 10 ), new_length );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -new_length );
    setAttrib( df, R_RowNamesSymbol, rownames );
    UNPROTECT( 1 );
}

/******************************************************************************/
void gar_fixation_frame_unprotect( SEXP df )
{
    UNPROTECT_PTR( df );
}

/******************************************************************************/
void gar_fixation_frame_update( SEXP df, uint32_t idx, gac_fixation_t* fixation )
{
    const char* label = "";

    if( fixation->first_sample.label != NULL )
    {
        label = fixation->first_sample.label;
    }

    REAL( VECTOR_ELT( df, 0 ) )[idx] = fixation->screen_point[0];
    REAL( VECTOR_ELT( df, 1 ) )[idx] = fixation->screen_point[1];
    REAL( VECTOR_ELT( df, 2 ) )[idx] = fixation->point[0];
    REAL( VECTOR_ELT( df, 3 ) )[idx] = fixation->point[1];
    REAL( VECTOR_ELT( df, 4 ) )[idx] = fixation->point[2];
    REAL( VECTOR_ELT( df, 5 ) )[idx] = fixation->duration;
    REAL( VECTOR_ELT( df, 6 ) )[idx] = fixation->first_sample.timestamp;
    REAL( VECTOR_ELT( df, 7 ) )[idx] = fixation->first_sample.trial_id;
    REAL( VECTOR_ELT( df, 8 ) )[idx] = fixation->first_sample.trial_onset;
    SET_STRING_ELT( VECTOR_ELT( df, 9 ), idx, Rf_mkChar( label ) );
    REAL( VECTOR_ELT( df, 10 ) )[idx] = fixation->first_sample.label_onset;
}

/******************************************************************************/
SEXP gar_get_filter_parameter( SEXP ptr )
{
    void* h = R_ExternalPtrAddr( ptr );
    gac_filter_parameter_t params;

    gac_get_filter_parameter( h, &params );
    return gar_filter_parameter_create( &params );
}

/******************************************************************************/
SEXP gar_get_filter_parameter_default()
{
    gac_filter_parameter_t params;

    gac_get_filter_parameter_default( &params );
    return gar_filter_parameter_create( &params );
}

/******************************************************************************/
SEXP gar_init( void )
{
    gac_type_tag = install( "GAC_TYPE_TAG" );
    return R_NilValue;
}

/******************************************************************************/
SEXP gar_parse( SEXP ptr, SEXP px, SEXP py, SEXP pz, SEXP ox, SEXP oy, SEXP oz,
        SEXP sx, SEXP sy, SEXP timestamp, SEXP trial_id, SEXP label )
{
    SEXP ret, fixations, saccades, rlabel;
    const char* clabel;
    const char* names[] = { "fixations", "saccades", "" };
    void* h;
    int32_t len, i;
    int *ptrial_id;
    double *psx, *psy, *ppx, *ppy, *ppz, *pox, *poy, *poz, *ptimestamp;
    bool res;
    gac_fixation_t fixation;
    uint32_t fixation_count = 0;
    gac_saccade_t saccade;
    uint32_t saccade_count = 0;

    CHECK_GAC_HANDLER( ptr );

    if( !Rf_isReal( px )
        || !Rf_isReal( py )
        || !Rf_isReal( pz )
        || ( sx != R_NilValue && !Rf_isReal( sx ) )
        || ( sy != R_NilValue && !Rf_isReal( sy ) )
        || !Rf_isReal( ox )
        || !Rf_isReal( oy )
        || !Rf_isReal( oz )
        || !Rf_isReal( timestamp ) )
    {
        error( "all point and timestamp vectors need to be of type double" );
        return R_NilValue;
    }

    if( !Rf_isInteger( trial_id ) )
    {
        error( "trial ID vector needs to be of type integer" );
        return R_NilValue;
    }

    if( !Rf_isString( label ) )
    {
        error( "label vector needs to be of type string" );
        return R_NilValue;
    }

    len = Rf_length( timestamp );

    if( Rf_length( px ) != len
        || Rf_length( py ) != len
        || Rf_length( pz ) != len
        || ( sx != R_NilValue && Rf_length( sx ) != len )
        || ( sx != R_NilValue && Rf_length( sy ) != len )
        || Rf_length( ox ) != len
        || Rf_length( oy ) != len
        || Rf_length( oz ) != len
        || Rf_length( trial_id ) != len
        || Rf_length( label ) != len )
    {
        error( "all vectors need to be of the same length" );
        return R_NilValue;
    }

    h = R_ExternalPtrAddr( ptr );

    psx = NULL;
    if( sx != R_NilValue )
    {
        psx = REAL( sx );
    }
    psy = NULL;
    if( sy != R_NilValue )
    {
        psy = REAL( sy );
    }
    ppx = REAL( px );
    ppy = REAL( py );
    ppz = REAL( pz );
    pox = REAL( ox );
    poy = REAL( oy );
    poz = REAL( oz );
    ptimestamp = REAL( timestamp );
    ptrial_id = INTEGER( trial_id );
    fixations = gar_fixation_frame_create( len );
    saccades = gar_saccade_frame_create( len );
    for( i = 0; i < len; i++ )
    {
        rlabel = STRING_ELT( label, i );
        clabel = NULL;
        if( !Rf_StringBlank( rlabel ) )
        {
            clabel = CHAR( rlabel );
        }
        if( psx != NULL && psy != NULL )
        {
            gac_sample_window_update_screen( h,
                    ( float )pox[i], ( float )poy[i], ( float )poz[i],
                    ( float )ppx[i], ( float )ppy[i], ( float )ppz[i],
                    ( float )psx[i], ( float )psy[i],
                    ptimestamp[i], ptrial_id[i], clabel );
        }
        else
        {
            gac_sample_window_update( h,
                    ( float )pox[i], ( float )poy[i], ( float )poz[i],
                    ( float )ppx[i], ( float )ppy[i], ( float )ppz[i],
                    ptimestamp[i], ptrial_id[i], clabel );
        }
        res = gac_sample_window_fixation_filter( h, &fixation );
        if( res )
        {
            gar_fixation_frame_update( fixations, fixation_count, &fixation );
            gac_fixation_destroy( &fixation );
            fixation_count++;
        }
        res = gac_sample_window_saccade_filter( h, &saccade );
        if( res )
        {
            gar_saccade_frame_update( saccades, saccade_count, &saccade );
            gac_saccade_destroy( &saccade );
            saccade_count++;
        }
        gac_sample_window_cleanup( h );
    }

    gar_fixation_frame_resize( fixations, fixation_count );
    gar_saccade_frame_resize( saccades, saccade_count );

    ret = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SET_VECTOR_ELT( ret, 0, fixations );
    SET_VECTOR_ELT( ret, 1, saccades );
    UNPROTECT( 1 );

    gar_fixation_frame_unprotect( fixations );
    gar_saccade_frame_unprotect( saccades );

    return ret;
}

/******************************************************************************/
SEXP gar_set_screen( SEXP ptr,
        SEXP top_left_x, SEXP top_left_y, SEXP top_left_z,
        SEXP top_right_x, SEXP top_right_y, SEXP top_right_z,
        SEXP bottom_left_x, SEXP bottom_left_y, SEXP bottom_left_z )
{
    void* h = R_ExternalPtrAddr( ptr );

    gac_set_screen( h,
            Rf_asReal( top_left_x ), Rf_asReal( top_left_y ), Rf_asReal( top_left_z ),
            Rf_asReal( top_right_x ), Rf_asReal( top_right_y ), Rf_asReal( top_right_z ),
            Rf_asReal( bottom_left_x ), Rf_asReal( bottom_left_y ), Rf_asReal( bottom_left_z ) );

    return R_NilValue;
}

/******************************************************************************/
SEXP gar_saccade_frame_create( uint32_t count )
{
    const char* names[] = { "start_screen_x", "start_screen_y", "start_x", "start_y", "start_z", "dest_screen_x", "dest_screen_y", "dest_x", "dest_y", "dest_z", "duration", "timestamp", "trial_id", "trial_onset", "label", "label_onset", "" };
    SEXP df = PROTECT( Rf_mkNamed( VECSXP, names ) );
    SEXP startscreenx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP startscreeny = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP startx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP starty = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP startz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destscreenx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destscreeny = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destx = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP desty = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP destz = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP duration = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP timestamp = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP trial_id = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP trial_onset = PROTECT( Rf_allocVector( REALSXP, count ) );
    SEXP label = PROTECT( Rf_allocVector( STRSXP, count ) );
    SEXP label_onset = PROTECT( Rf_allocVector( REALSXP, count ) );

    SET_VECTOR_ELT( df, 0, startscreenx );
    SET_VECTOR_ELT( df, 1, startscreeny );
    SET_VECTOR_ELT( df, 2, startx );
    SET_VECTOR_ELT( df, 3, starty );
    SET_VECTOR_ELT( df, 4, startz );
    SET_VECTOR_ELT( df, 5, destscreenx );
    SET_VECTOR_ELT( df, 6, destscreeny );
    SET_VECTOR_ELT( df, 7, destx );
    SET_VECTOR_ELT( df, 8, desty );
    SET_VECTOR_ELT( df, 9, destz );
    SET_VECTOR_ELT( df, 10, duration );
    SET_VECTOR_ELT( df, 11, timestamp );
    SET_VECTOR_ELT( df, 12, trial_id );
    SET_VECTOR_ELT( df, 13, trial_onset );
    SET_VECTOR_ELT( df, 14, label );
    SET_VECTOR_ELT( df, 15, label_onset );

    UNPROTECT( 16 );

    SET_CLASS( df, mkString( "data.frame" ) );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -count );
    setAttrib( df, R_RowNamesSymbol, rownames );
    UNPROTECT( 1 );

    return df;
}

/******************************************************************************/
void gar_saccade_frame_resize( SEXP df, uint32_t new_length )
{
    SETLENGTH( VECTOR_ELT( df, 0 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 1 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 2 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 3 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 4 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 5 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 6 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 7 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 8 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 9 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 10 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 11 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 12 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 13 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 14 ), new_length );
    SETLENGTH( VECTOR_ELT( df, 15 ), new_length );

    SEXP rownames = PROTECT( allocVector( INTSXP, 2 ) );
    SET_INTEGER_ELT( rownames, 0, NA_INTEGER );
    SET_INTEGER_ELT( rownames, 1, -new_length );
    setAttrib( df, R_RowNamesSymbol, rownames );
    UNPROTECT( 1 );
}

/******************************************************************************/
void gar_saccade_frame_unprotect( SEXP df )
{
    UNPROTECT_PTR( df );
}

/******************************************************************************/
void gar_saccade_frame_update( SEXP df, uint32_t idx, gac_saccade_t* saccade )
{
    const char* label = "";

    if( saccade->first_sample.label != NULL )
    {
        label = saccade->first_sample.label;
    }

    REAL( VECTOR_ELT( df, 0 ) )[idx] = saccade->first_sample.screen_point[0];
    REAL( VECTOR_ELT( df, 1 ) )[idx] = saccade->first_sample.screen_point[1];
    REAL( VECTOR_ELT( df, 2 ) )[idx] = saccade->first_sample.point[0];
    REAL( VECTOR_ELT( df, 3 ) )[idx] = saccade->first_sample.point[1];
    REAL( VECTOR_ELT( df, 4 ) )[idx] = saccade->first_sample.point[2];
    REAL( VECTOR_ELT( df, 5 ) )[idx] = saccade->last_sample.screen_point[0];
    REAL( VECTOR_ELT( df, 6 ) )[idx] = saccade->last_sample.screen_point[1];
    REAL( VECTOR_ELT( df, 7 ) )[idx] = saccade->last_sample.point[0];
    REAL( VECTOR_ELT( df, 8 ) )[idx] = saccade->last_sample.point[1];
    REAL( VECTOR_ELT( df, 9 ) )[idx] = saccade->last_sample.point[2];
    REAL( VECTOR_ELT( df, 10 ) )[idx] = saccade->last_sample.timestamp - saccade->first_sample.timestamp;
    REAL( VECTOR_ELT( df, 11 ) )[idx] = saccade->first_sample.timestamp;
    REAL( VECTOR_ELT( df, 12 ) )[idx] = saccade->first_sample.trial_id;
    REAL( VECTOR_ELT( df, 13 ) )[idx] = saccade->first_sample.trial_onset;
    SET_STRING_ELT( VECTOR_ELT( df, 14 ), idx, Rf_mkChar( label ) );
    REAL( VECTOR_ELT( df, 15 ) )[idx] = saccade->first_sample.label_onset;
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
