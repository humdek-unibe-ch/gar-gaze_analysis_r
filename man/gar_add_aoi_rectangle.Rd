% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/wrapper.R
\name{gar_add_aoi_rectangle}
\alias{gar_add_aoi_rectangle}
\title{Add an AOI rectangle to the gaze analysis handler in order to enable the
AOI analysis.}
\usage{
gar_add_aoi_rectangle(h, x, y, width, height, label = NULL)
}
\arguments{
\item{h}{A pointer to the gaze analysis handler.}

\item{x}{The normalized x coordinate of the upper left corner.}

\item{y}{The normalized y coordinate of the upper left corner.}

\item{width}{The normalized width of the rectangle.}

\item{height}{The normalized height of the rectangle.}

\item{label}{An optional lable describing the AOI.}
}
\description{
Add an AOI rectangle to the gaze analysis handler in order to enable the
AOI analysis.
}
\examples{
 h <- gar_create()
 gar_add_aoi_rectangle( h, 0.1, 0.1, 0.2, 0.2, "myRect" )
}
