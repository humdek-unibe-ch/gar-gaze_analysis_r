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

Finally, pass the sample data to the parser (use `help(gar_parse)` )
```R
res <- gar_parse( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$sx, d$sy, d$timestamp, d$trial_id, d$label )
```

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

## Create an R Package

To create the package bundle with command line run the script `build_linux.sh`

### Ubuntu

In order for the compilation of the C library to work several packages need to be available.

```sh
sudo apt install build-essential
sudo apt install autoconf autogen libtool
```

Install R

```sh
sudo apt install r-base r-base-dev
```

Run the build script. This will update the documentation, build the package,
and move the package to the release folder.

```sh
./build_linux.sh
```

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

Open The package in RStudio (open the file `gar.Rproj`) and select the tab **Build** in the top left corner.
Then, select **More** and **Build Binary Package**.

This will produce a file `gar_<version>.zip` in the parent folder.
Copy this file into the folder **release** of the repository and rename it to `gar_<version>_R_x86_64-pc-windows-gnu.zip`
