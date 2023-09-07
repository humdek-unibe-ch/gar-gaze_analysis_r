# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

#' Executed on load of the library.
#'
#' @param lib
#'  The name of the library
#' @param pkg
#'  The name of the package
.onLoad <- function( lib, pkg )
{
    # library.dynam( "gar", pkg, lib )
    .Call( "gar_init" )
}

#' Add an arbitrary AOI to the gaze analysis handler in order to enable the AOI
#' analysis. Use gar_create_aoi() to create the AOI.
#'
#' @param h
#'  A pointer to the gaze analysis handler, holding the filter parameters.
#' @param aoi
#'  A pointer to the AOI to add.
#' @export
#' @examples
#'  h <- gar_create()
#'  x <- c( 0.5, 0.5, 0.6, 0.7, 0.8, 0.8, 0.7, 0.6 )
#'  y <- c( 0.4, 0.3, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5 )
#'  df <- data.frame( x, y )
#'  aoi <- gar_create_aoi( df, "myAOI" )
#'  gar_add_aoi( h, aoi )
gar_add_aoi <- function( h, aoi )
{
    return( .Call( "gar_add_aoi", h, aoi ) )
}

#' Add an AOI rectangle to the gaze analysis handler in order to enable the
#' AOI analysis.
#'
#' @param h
#'  A pointer to the gaze analysis handler, holding the filter parameters.
#' @param x
#'  The normalized x coordinate of the upper left corner.
#' @param y
#'  The normalized y coordinate of the upper left corner.
#' @param width
#'  The normalized width of the rectangle.
#' @param height
#'  The normalized height of the rectangle.
#' @param label
#'  An optional lable describing the AOI.
#' @export
#' @examples
#'  h <- gar_create()
#'  gar_add_aoi_rectangle( h, 0.1, 0.1, 0.2, 0.2, "myRect" )
gar_add_aoi_rectangle <- function( h, x, y, width, height, label )
{
    return( .Call( "gar_add_aoi_rectangle", h, x, y, width, height, label ) )
}

#' Create a gaze analysis handler. If no parameter structure is provided
#' default values are used.
#'
#' @param params
#'  An optional filter parameter structure.
#' @return
#'  A pointer to the allocated structure or NULL on failure.
#' @export
#' @examples
#'  h <- gar_create()
#'
#'  params <- gar_get_filter_parameter_default()
#'  params$gap$max_gap_length <- 0
#'  h <- gar_create( params )
gar_create <- function( params = NULL )
{
    return( .Call( "gar_create", params ) )
}

#' Create an AOI structure given a list of points.
#'
#' @param points
#'  A data frame holding normalized 2d points.
#' @param label
#'  An optional label describing the AOI.
#' @return
#'  A pointer to the allocated structure or NULL on failure.
#' @export
#' @examples
#'  x <- c( 0.5, 0.5, 0.6, 0.7, 0.8, 0.8, 0.7, 0.6 )
#'  y <- c( 0.4, 0.3, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5 )
#'  df <- data.frame( x, y )
#'  aoi <- gar_create_aoi( df, "myAOI" )
gar_create_aoi <- function( points, label )
{
    return( .Call( "gar_create_aoi", points, label ) )
}

#' Get the current filter parameters.
#'
#' @param h
#'  A pointer to the gaze analysis handler
#' @return
#'  The filter parameters on success or NULL on failure.
#'  For a detalied description of teh parameter values use
#'  `help(gar_get_filter_parameter_default)`
#' @export
#' @examples
#'  h <- gar_create()
#'  params <- gar_get_filter_parameter( h )
gar_get_filter_parameter <- function( h )
{
    return( .Call( "gar_get_filter_parameter", h ) )
}

#' Get the default filter parameter values.
#'
#' @return
#'  The default filter parameters on success or NULL on failure.
#'  The parameters are structured as nested named lists:
#'
#'  - `gap`: The gap filter parameter
#'      - `max_gap_length`: The maximal allowed gap length to be filled-in. Set
#'      to zero to disable gap fill-in filter.
#'      - `sample_period`: The sample period to compute the number of required
#'      fill-in samples.
#'  - `noise`: The noise filter parameter
#'      - `mid_idx`: The mid index of the moving average noise filter window.
#'      This is used to compute the length of the window:
#'      `window_length = mid_idx * 2 + 1`. Set to zero to disable noise filter.
#'  - `fixation`: The fixation parser parameters
#'      - `duration_threshold`: The duration threshold in milliseconds.
#'      - `dispersion_threshold`: The dispersion threshold in degrees.
#'  - `saccade`: The saccade parser parameters
#'      - `velocity_threshold`: The velocity threshold in degrees per second.
#' @export
#' @examples
#'  params <- gar_get_filter_parameter_default()
gar_get_filter_parameter_default <- function()
{
    return( .Call( "gar_get_filter_parameter_default" ) )
}

#' Parse a set of input data for fixations and saccades.
#'
#' @param h
#'  A pointer to the gaze analysis handler, holding the filter parameters.
#' @param px
#'  A double vector of x cooridnates of the gaze point
#' @param py
#'  A double vector of y cooridnates of the gaze point
#' @param pz
#'  A double vector of z cooridnates of the gaze point
#' @param ox
#'  A double vector of x cooridnates of the gaze origin
#' @param oy
#'  A double vector of y cooridnates of the gaze origin
#' @param oz
#'  A double vector of z cooridnates of the gaze origin
#' @param sx
#'  An optional vector holding the x coordinates of the gaze screen point
#' @param sy
#'  An optional vector holding the y coordinates of the gaze screen point
#' @param timestamp
#'  A double vector of the relative timestamp in milliseconds
#' @param trial_id
#'  An optional vector holding the ID of the ongoing trial
#' @param label
#'  An optional vector holding an arbitrary label annotating each sample
#' @return
#'  The identified fixations and saccades as a named list:
#'  - `fixations[]`:
#'    - `sx`: The x-coordinate of the average screen gaze point during the fixation
#'    - `sy`: The y-coordinate of the average screen gaze point during the fixation
#'    - `px`: The x-coordinate of the average gaze point during the fixation
#'    - `py`: The y-coordinate of the average gaze point during the fixation
#'    - `pz`: The z-coordinate of the average gaze point during the fixation
#'    - `duration`: The fixation duration in milliseconds
#'    - `timestamp`: The relative timestamp of the first gaze point in the
#'      fixation in milliseconds
#'    - `trial_id`: The rial ID of first sample of the fixation
#'    - `trial_onset`: The timestamp in milliseconds of the first sample of the
#'      fixation since the last trial ID change
#'    - `label`: The annotation of the first sample of the fixation
#'    - `label_onset`: The timestamp in milliseconds of the first sample of the
#'      fixation since the last label change
#'  - `saccades[]`:
#'    - `start_screen_x`: The x-coordinate of the first screen gaze point in the saccade
#'    - `start_screen_y`: The y-coordinate of the first screen gaze point in the saccade
#'    - `start_x`: The x-coordinate of the first gaze point in the saccade
#'    - `start_y`: The y-coordinate of the first gaze point in the saccade
#'    - `start_z`: The z-coordinate of the first gaze point in the saccade
#'    - `dest_screen_x`: The x-coordinate of the last screen point in the saccade
#'    - `dest_screen_y`: The y-coordinate of the last screen point in the saccade
#'    - `dest_x`: The x-coordinate of the last gaze point in the saccade
#'    - `dest_y`: The y-coordinate of the last gaze point in the saccade
#'    - `dest_z`: The z-coordinate of the last gaze point in the saccade
#'    - `duration`: The saccade duration in milliseconds
#'    - `timestamp`: The relative timestamp of the first gaze point in the
#'      saccade in milliseconds
#'    - `trial_id`: The rial ID of first sample of the saccade
#'    - `trial_onset`: The timestamp in milliseconds of the first sample of the
#'      saccade since the last trial ID change
#'    - `label`: The annotation of the first sample of the saccade
#'    - `label_onset`: The timestamp in milliseconds of the first sample of the
#'      saccade since the last label change
#' @export
#' @examples
#'  h <- gar_create()
#'  res <- gar_parse( h, gaze$px, gaze$py, gaze$pz, gaze$ox, gaze$oy, gaze$oz,
#'          gaze$sx, gaze$sy, gaze$timestamp, gaze$trial_id, gaze$label )
gar_parse <- function( h, px, py, pz, ox, oy, oz, sx, sy, timestamp, trial_id, label )
{
    return( .Call( "gar_parse", h, px, py, pz, ox, oy, oz, sx, sy, timestamp, trial_id, label ) )
}

#' Configure the screen position in 3d space. If no 2d gaze coordinates are
#' provided in gar_parse() the screen position will be used to compute 2d gaze
#' coordinates automatically.
#'
#' @param h
#'  A pointer to the gaze analysis handler.
#' @param top_left_x
#'  The x coordinate of the top left screen corner.
#' @param top_left_y
#'  The y coordinate of the top left screen corner.
#' @param top_left_z
#'  The z coordinate of the top left screen corner.
#' @param top_right_x
#'  The x coordinate of the top right screen corner.
#' @param top_right_y
#'  The y coordinate of the top right screen corner.
#' @param top_right_z
#'  The z coordinate of the top right screen corner.
#' @param bottom_left_x
#'  The x coordinate of the bottom left screen corner.
#' @param bottom_left_y
#'  The y coordinate of the bottom left screen corner.
#' @param bottom_left_z
#'  The z coordinate of the bottom left screen corner.
#' @export
#' @examples
#'  h <- gar_create()
#'  gar_set_screen( h, -298.64, 331.74, 113.91, 298.88, 331.74, 113.91, -298.64, 15.91, -1.05 )
gar_set_screen <- function( h,
        top_left_x, top_left_y, top_left_z,
        top_right_x, top_right_y, top_right_z,
        bottom_left_x, bottom_left_y, bottom_left_z )
{
    return( .Call( "gar_set_screen", h,
          top_left_x, top_left_y, top_left_z,
          top_right_x, top_right_y, top_right_z,
          bottom_left_x, bottom_left_y, bottom_left_z ) )
}
