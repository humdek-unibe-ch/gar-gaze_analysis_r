# `gar`: R Bindings for `libgac`

This repository holds an R package which implements the bindings to the gaze analysis library [gac](http://phhum-a209-cp.unibe.ch:10012/LIB/LIB-gaze_analysis_c).

Note that bindings were only created for high level functions to parse for fixations and saccades.
Filters can be controlled through the filter parameter structure.

## Quick Start

Install and load the library (where `<version and os>` stands for the appropriate file postfix depending on the library version and the target operating system):

```R
install.packages('release/gar_<version and os>', repos = NULL)
library(gar)
```

Get the default filter parameters (use `help(gar_get_filter_parameter_default)` for a parameter documentation):

```R
params <- gar_get_filter_parameter_default()
```

Adjust the filter parameters to your liking (e.g. disable noise and gap filter and increase the velocity threshold for saccade detection ):

```R
params$gap$max_gap_length <- 0
params$noise$mid_idx <- 0
params$saccade$velocity_threshold <- 25
```

Initialise the gaze analysis handler with the modified parameter structure (if no parameter structure is passed to `gar_create()` the default filter parameters are used):

```R
h <- gar_create( params )
```

Load some sample data from a csv file (make sure that coordinate and timestamp data is parsed as a `numeric` and not as `integer`) and filter the data for valid gaze samples:

```R
d_raw <- read.csv('example/sample.csv', colClasses=c('numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'integer', 'character', 'logical', 'logical', 'logical'))
d <- subset(d_raw, svalid == TRUE & pvalid == TRUE & ovalid == TRUE)
```

An AOI analysis can be enabled by adding AOIs to the gaze analysis handler.
This can be an arbitrary AOI defined by a sequence of normalized 2d points:

```R
x <- c( 0.5, 0.5, 0.6, 0.7, 0.8, 0.8, 0.7, 0.6 )
y <- c( 0.4, 0.3, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5 )
df <- data.frame( x, y )
aoi <- gar_add_aoi_points( h, df, "my_circular_aoi" )
```

A simpler, rectangular AOI can be added as follows:

```R
gar_add_aoi_rectangle(h, 0.3, 0.45, 0.1, 0.1, "my_rectangular_aoi")
```

Finally, pass the sample data to the parser (use `help(gar_parse)` )
```R
res <- gar_parse( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$sx, d$sy, d$timestamp, d$trial_id, d$label )
```

The result holds three data tables:
- `fixations`: fixations detected with the I-DT algorithm (Salvucci & Goldberg 2000)
- `saccades`: saccades detected with the I-VT algorithm (Salvucci & Goldberg 2000)
- `aoi`: area of interest analysis based on the detected fixations and saccades

## Basic Concept

The library mainly provides the function `gar_parse` to parse gaze data for fixations and saccades.
The function accepts vectors which must be all of the same length.
Note that some arguments are optional and also accept `NULL`.

The function expects a pointer to a gaze data handler which can be created with `gar_create()`.
`gar_create()` accepts an optional filter parameter structure to configure filters.
Use `gar_get_filter_parameter_default()` to create a filter structure.
Refer to the documentation (`help(gar_get_filter_parameter_default)`) for more information one each parameter value.

The function `gar_get_filter_parameter()` returns the currently used parameter structure.

### Detection Algorithm

Fixations are detected with the I-DT algorithm (Salvucci & Goldberg 2000).
Saccades are detected with the I-VT algorithm (Salvucci & Goldberg 2000).

Note that the resulting fixations and saccades will **not** fit together perfectly (e.g. a saccade follows a fixation and vice versa) because
1. both algorithms work with their own parameters which will most likely lead to gaps (data which is neither classified as part of a fixation nor saccade)
2. gaze data may be a recording of a smooth pursuit
3. gaps in the gaze data because of blinks or other data loss

Refer to the documentation (`help(gar_get_filter_parameter_default)`) for more information one each parameter value.

### Filters

Optionally the gaze data is processed by
1. a moving average filter which computes the average of all samples in the filters own sliding window. Sample annotations (e.g. the label, trial ID, and timestamps) are copied from the data sample in the middle of the sliding window.
2. a gap fill-in filter where data samples are filled into gaps using linear interpolation.

Refer to the documentation (`help(gar_get_filter_parameter_default)`) for more information one each parameter value.

### 3d vs 2d Data

All calculations are performed on 3d data.
If only 2d data is available this library cannot be used (yet).
The reason for this is that with 3d data it is possible to compute an accurate dispersion and velocity threshold based on the distance of the gaze origin to the gaze point.
For 2d data the dispersion and velocity threshold would need to be estimated based on the measured data which is not (yet) supported by the library.

However, it is possible to provide 2d data alongside 3d data for each data sample which will propagated to fixation and saccade result structures.

If 2d data is not available it is possible to compute the 2d data automatically.
To do this it is necessary to provide the gaze analysis handler with the exact position of the screen in 3d space.
Use the function `gar_set_screen()` to set the screen coordinates.
Note that this has no effect if 2d data are passed to `gar_parse()` as in this case the 2d data from the source are used.

### Sample annotations

Each sample has two fields available for custom data annotation:
 - `trial_id`: expects an integer number and can be used to e.g. associate a data point to a trial.
 - `label`: expects a string and can be used to e.g. describe the currently displayed stimuli.

The annotations are propagated to the fixation and saccade result structures.

Further, each sample has two additional timestamp fields for onset information of the annotations:
 - `trial_onset`: the amount of milliseconds since the last change in the field `trial_id`.
 - `label_onset`: the amount of milliseconds since the last change in the field `label`.

### Area of Interest (AOI) Analysis

The area of interest (AOI) analysis is performed based on fixations.
Saccade information can also be used to extend the analysis but fixations are always required.
For each distinct trial ID block an analysis of each AOI is performed.

The `trial_timestamp` in the AOI analysis result is the timestamp of the last trial ID change.
All other time information of the AOI analysis is an onset with the `trial_timestamp` as reference.
The `label_timestamp` is not directly reported in the analysis result but it is used to compute the `label_onset` and the `first_saccade_latency`.
It is the timestamp of the last sample annotation label change (it has nothing to do with the AOI name).
Use the sample annotation label to mark an interesting point in the trial progression (e.g. change the label when an image is shown to the participant).

To decide whether a sample point is inside an AOI a ray casting method is used where a virtual ray is drawn from an arbitrary point outside the AOI to the sample point.
Then, every intersection with segments of the AOI contour is counted.
If an even number of intersection is detected, the point lies outside of the AOI, otherwise the point lies inside the AOI.
To improve performance, a coarse detection using a rectangular a bounding box is performed (if the sample point lies outside the bounding box it also lies outside the AOI).


## Create an R Package

The easiest way to create an R package is through [RStudio](https://posit.co/download/rstudio-desktop/).

### Ubuntu

Before installing RStudio the following tools need to be installed in order to compile the C library which is contained withing the R package `gar`.

```sh
sudo apt install build-essential
sudo apt install autoconf autogen libtool
```

In order to allow for automated document generation install

```sh
sudo apt install texlive-latex-base texlive-fonts-recommended
```

To install RStudio, first, install R with

```sh
sudo apt install r-base
```

Then, download and install the appropriate [RStudio file](https://posit.co/download/rstudio-desktop/).

To build the `gar` package install the following dependencies in R:

```R
install.packages(c('roxygen2', 'pkgbuild'))
```

The R `devtools` are not strictly necessary but handy to improve the package building process.
They to, however, rely on some headers that need to be installed:

```sh
sudo apt install libcurl4-gnutls-dev libtiff-dev
```

```R
install.packages('devtools')
```

Now everything should be ready to build the package.
Open the RStudio project file `gar.Rproj` navigate to the build tab (top left corner) and run the *Check* utility.
If no Errors or Warnings are listed, the package can be build through *More->Build Binary Package* and installed through *Install*.

### Windwos

The simplest way to build the package under windows is by first installing the following packages:

- [R](https://cran.r-project.org/bin/windows/base/)
- [RStudio](https://posit.co/download/rstudio-desktop/)
- [RTools43](https://cran.r-project.org/bin/windows/Rtools/) (make sure to download the matching version your installed R)


Then, open Rtools43 Bash and type the following commands to install the necessary packages to compile the c library included in the gaze analysis package

```sh
pacman -Syyu
pacman -Sy autogen autoconf automake libtool
```

Now everything should be ready to build the package.
Open the RStudio project file `gar.Rproj` navigate to the build tab (top left corner) and run the *Check* utility.
If no Errors or Warnings are listed, the package can be build through *More->Build Binary Package* and installed through *Install*.
