library(gar)

params <- gar_get_filter_parameter_default()
print(params)

params$gap$max_gap_length <- 0
params$gap$sample_period <- 1000/120
params$noise$mid_idx <- 0
params$saccade$velocity_threshold <- 25
params$fixation$duration_threshold <- 120
params$fixation$dispersion_threshold <- 1.5
h <- gar_create( params )
new_params <- gar_get_filter_parameter( h )
print(new_params)

d <- read.csv('git-gaze_analysis_r/sample/test.csv', colClasses=c('numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric'))
res <- gar_filter( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$timestamp)
print(res)
