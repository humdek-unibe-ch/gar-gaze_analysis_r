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

