gar_create <- function( params = NULL )
{
    return( .Call( "gar_create", params ) )
}

gar_get_filter_parameter <- function( h )
{
    return( .Call( "gar_get_filter_parameter", h ) )
}

gar_get_filter_parameter_default <- function()
{
    return( .Call( "gar_get_filter_parameter_default" ) )
}
