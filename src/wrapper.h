/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <Rinternals.h>
#include "gac.h"
#include "gac_aoi_collection.h"

/**
 * Add an AOI defined by points to the gaze anlysis structure. This enables the
 * AOI analysis on the added AOI.
 *
 * @param ptr
 *  An external pointer structure pointing to the gac handler.
 * @param points
 *  A data frame where each row is a normalized 2d point of the AOI.
 * @param label
 *  An optional label describing the AOI.
 * @return
 *  R_NilValue
 */
SEXP gar_add_aoi_points( SEXP ptr, SEXP points, SEXP label );

/**
 * Add an AOI rectangle to the gaze anlysis structure. This enables the AOI
 * analysis on the added AOI.
 *
 * @param ptr
 *  An external pointer structure pointing to the gac handler.
 * @param x
 *  The normailized x coordinate of the top left corner of the AOI.
 * @param y
 *  The normailized y coordinate of the top left corner of the AOI.
 * @param width
 *  The normalized width of the AOI.
 * @param height
 *  The normalized height of the AOI.
 * @param label
 *  An optional label describing the AOI.
 * @return
 *  R_NilValue
 */
SEXP gar_add_aoi_rectangle( SEXP ptr, SEXP x, SEXP y, SEXP width, SEXP height,
        SEXP label );

/**
 * Create a data frame container to hold fixations.
 *
 * @param count
 *  A preliminary count of items to be added to the data frame.
 * @return
 *  The data frame.
 */
SEXP gar_analysis_frame_create( uint32_t count );

/**
 * Resize the AOI analysis data frame.
 *
 * @param df
 *  The data frame to rezise.
 * @param new_length
 *  The ne length of the data frame
 */
void gar_analysis_frame_resize( SEXP df, uint32_t new_length );

/**
 * Release the protection of the AOI analysis data frame.
 *
 * @param df
 *  The data frame to unprotect.
 */
void gar_analysis_frame_unprotect( SEXP df );

/**
 * Add a new entry to the AOI analysis data frame.
 *
 * @param df
 *  The data frame to update.
 * @param idx
 *  A pointer to the row index of the new entry. This is updated from within
 *  the function.
 * @param analysis
 *  The AOI analysis entry to add.
 */
void gar_analysis_frame_update( SEXP df, uint32_t* idx,
        gac_aoi_collection_analysis_result_t* analysis );

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
 * Prepare a data frame where each row represents a saccade and each column
 * a property of all saccades.
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
 * Destroy an AOI.
 *
 * @param ptr
 *  The external pointer structure pointing to the AOI.
 * @return
 *  R_NilValue
 */
SEXP gar_destroy_aoi( SEXP ptr );

/**
 * Allocate the filter parameter R structure.
 *
 * @param params
 *  A pointer to a gac filter parameter structure holding the values to assign
 *  to the R structure.
 * @return
 *  An R named list holding the assigned gac filter parameters.
 */
SEXP gar_filter_parameter_create( gac_filter_parameter_t* params );

/**
 * Create a data frame container to hold fixations.
 *
 * @param count
 *  A preliminary count of items to be added to the data frame.
 * @return
 *  The data frame.
 */
SEXP gar_fixation_frame_create( uint32_t count );

/**
 * Resize the fixation data frame.
 *
 * @param df
 *  The data frame to rezise.
 * @param new_length
 *  The ne length of the data frame
 */
void gar_fixation_frame_resize( SEXP df, uint32_t new_length );

/**
 * Release the protection of the fixation data frame.
 *
 * @param df
 *  The data frame to unprotect.
 */
void gar_fixation_frame_unprotect( SEXP df );

/**
 * Add a new entry to the fixation data frame.
 *
 * @param df
 *  The data frame to update.
 * @param idx
 *  The row index of the new entry.
 * @param fixation
 *  The fixation entry to add.
 */
void gar_fixation_frame_update( SEXP df, uint32_t idx, gac_fixation_t* fixation );

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
 *  A vector holding the x coordinates of the gaze point.
 * @param py
 *  A vector holding the y coordinates of the gaze point.
 * @param pz
 *  A vector holding the z coordinates of the gaze point.
 * @param ox
 *  A vector holding the x coordinates of the gaze origin.
 * @param oy
 *  A vector holding the y coordinates of the gaze origin.
 * @param oy
 *  A vector holding the z coordinates of the gaze origin.
 * @param sx
 *  A vector holding the x coordinates of the gaze screen point.
 * @param sy
 *  A vector holding the y coordinates of the gaze screen point.
 * @param timestamp
 *  A vector holding the relative timestamps of the samples.
 * @param trial_id
 *  The ID of the current trial.
 * @param label
 *  An arbitary label annotating the data.
 * @return
 *  A named list holding two data frames, one holding fixations and one holding
 *  saccades.
 */
SEXP gar_parse( SEXP ptr, SEXP px, SEXP py, SEXP pz, SEXP ox, SEXP oy, SEXP oz,
        SEXP sx, SEXP sy, SEXP timestamp, SEXP trial_id, SEXP label );

/**
 * Configure the screen position in 3d space. This allows to compute 2d
 * gaze point coordinates.
 *
 * @param h
 *  A pointer to the gaze analysis handler.
 * @param top_left_x
 *  The x coordinate of the top left screen corner.
 * @param top_left_y
 *  The y coordinate of the top left screen corner.
 * @param top_left_z
 *  The z coordinate of the top left screen corner.
 * @param top_right_x
 *  The x coordinate of the top right screen corner.
 * @param top_right_y
 *  The y coordinate of the top right screen corner.
 * @param top_right_z
 *  The z coordinate of the top right screen corner.
 * @param bottom_left_x
 *  The x coordinate of the bottom left screen corner.
 * @param bottom_left_y
 *  The y coordinate of the bottom left screen corner.
 * @param bottom_left_z
 *  The z coordinate of the bottom left screen corner.
 * @return
 *  R_NilValue
 */
SEXP gar_set_screen( SEXP ptr,
        SEXP top_left_x, SEXP top_left_y, SEXP top_left_z,
        SEXP top_right_x, SEXP top_right_y, SEXP top_right_z,
        SEXP bottom_left_x, SEXP bottom_left_y, SEXP bottom_left_z );

/**
 * Create a data frame container to hold saccades.
 *
 * @param count
 *  A preliminary count of items to be added to the data frame.
 * @return
 *  The data frame.
 */
SEXP gar_saccade_frame_create( uint32_t count );

/**
 * Resize the saccade data frame.
 *
 * @param df
 *  The data frame to rezise.
 * @param new_length
 *  The ne length of the data frame
 */
void gar_saccade_frame_resize( SEXP df, uint32_t new_length );

/**
 * Release the protection of the saccade data frame.
 *
 * @param df
 *  The data frame to unprotect.
 */
void gar_saccade_frame_unprotect( SEXP df );

/**
 * Add a new entry to the saccade data frame.
 *
 * @param df
 *  The data frame to update.
 * @param idx
 *  The row index of the new entry.
 * @param saccade
 *  The saccade entry to add.
 */
void gar_saccade_frame_update( SEXP df, uint32_t idx, gac_saccade_t* saccade );
