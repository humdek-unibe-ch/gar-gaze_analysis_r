# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

#' gar: R bindings for the gaze analysis C library libgac.
#'
#' The library allow to perform basic gaze analysis.
#' - Sample filtering with moving average
#' - Sample gap fill-in through linear interpolation (lerp)
#' - Fixation detection with I-DT algorithm
#' - Saccade detection with I-VT algorithm
#'
#' @docType package
#' @name gar
#' @useDynLib gar
NULL
#> NULL
