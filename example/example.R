# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

library(gar)

# get the default parameters from the library
params <- gar_get_filter_parameter_default()

# set the filter parameters
params$gap$max_gap_length <- 50
params$gap$sample_period <- 1000/60
params$noise$mid_idx <- 1
params$saccade$velocity_threshold <- 20
params$fixation$duration_threshold <- 100
params$fixation$dispersion_threshold <- 0.5

# read the csv sample file and filter the data for all valid gaze samples
d_raw <- read.csv('example/gaze.csv', colClasses=c(
  'numeric', 'numeric',
  'numeric', 'numeric', 'numeric',
  'numeric', 'numeric', 'numeric',
  'numeric', 'integer', 'character',
  'logical', 'logical', 'logical'))
d <- subset(d_raw, svalid == TRUE & pvalid == TRUE & ovalid == TRUE)

# create the gaze analysis handler (use 2d data from csv sample file)
h <- gar_create( params )

# add an arbitrary AOI to the gaze analysis handler
x <- c( 0.5, 0.5, 0.6, 0.7, 0.8, 0.8, 0.7, 0.6 )
y <- c( 0.4, 0.3, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5 )
df <- data.frame( x, y )
aoi <- gar_add_aoi_points( h, df, "aoi0" )
# add rectangle AOIs to the gaze analysis handler
gar_add_aoi_rectangle(h, 0.3, 0.45, 0.1, 0.1, "aoi1")
gar_add_aoi_rectangle(h, 0.5, 0.75, 0.2, 0.2, "aoi2")
gar_add_aoi_rectangle(h, 0.1, 0.3, 0.2, 0.1, "aoi3")
# parse the gaze data by passing 2d data along. Given that AOIs were added to
# the handler, AOI analysis is enabled.
res <- gar_parse( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$sx, d$sy,
                  d$timestamp, d$trial_id, d$label )

# create the gaze analysis handler with screen coordinates (compute 2d data
# based on screen coordinates)
h_screen <- gar_create( params )
gar_set_screen( h_screen,
                -298.64031982421875, 331.7396545410156, 113.90633392333984,
                298.87738037109375, 331.7396545410156, 113.90633392333984,
                -298.64031982421875, 15.905486106872559, -1.0478993654251099 )

# parse the gaze data and compute 2d data based on the configured screen
# coordinates. Given that no AOIs were added to the handler, the AOI analysis
# data frame will be empty in the result res_screen.
res_screen <- gar_parse( h_screen, d$px, d$py, d$pz, d$ox, d$oy, d$oz,
                         NULL, NULL, d$timestamp, d$trial_id, d$label )
