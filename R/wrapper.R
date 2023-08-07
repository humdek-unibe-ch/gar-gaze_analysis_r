.First.lib <- function( lib, pkg )
{
    library.dynam( "gar", pkg, lib )
    .Call( "gar_init" )
}

#' Allocate the gaze analysis structure on the heap.
#' If no parameter structure is provided default values are used.
#' Refer to gac_init() for more information.
#'
#' @param parameter
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

#' Get the current filter parameters.
#'
#' @param h
#'  A pointer to the gaze analysis handler
#' @return
#'  The filter parameters on success or NULL on failure
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
#'  The default filter parameters on success or NULL on failure
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
#' @param timestamp
#'  A double vector of the relative timestamp in milliseconds
#' @return
#'  The identified fixations and saccades as a named list:
#'  - fixations[]:
#'    - px: The x-coordinate of the average gaze point during the fixation
#'    - py: The y-coordinate of the average gaze point during the fixation
#'    - pz: The z-coordinate of the average gaze point during the fixation
#'    - duration: The fixation duration in milliseconds
#'    - timestamp: The relative timestamp of the first gaze point in the
#'      fixation in milliseconds
#'  - saccades[]:
#'    - startx: The x-coordinate of the first gaze point in the saccade
#'    - starty: The y-coordinate of the first gaze point in the saccade
#'    - startz: The z-coordinate of the first gaze point in the saccade
#'    - destx: The x-coordinate of the last gaze point in the saccade
#'    - desty: The y-coordinate of the last gaze point in the saccade
#'    - destz: The z-coordinate of the last gaze point in the saccade
#'    - duration: The saccade duration in milliseconds
#'    - timestamp: The relative timestamp of the first gaze point in the
#'      saccade in milliseconds
#' @export
#' @examples
#'  h <- gar_create()
#'  d <- read.csv( 'sample.csv', colClasses=c('numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric') )
#'  res <- gar_filter( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$timestamp )
gar_parse <- function( h, px, py, pz, ox, oy, oz, timestamp )
{
    return( .Call( "gar_parse", h, px, py, pz, ox, oy, oz, timestamp ) )
}
