# R Bindings for `libgac`

This repository holds an R package which implement the bindings to the gaze analysis library [gac](http://phhum-a209-cp.unibe.ch:10012/LIB/LIB-gaze_analysis_c).
This R package is called `gar` which stands for Gaze Analysis in R.

Note that bindings were only created for high level functions to parse for fixations and saccades.
Filters must be controlled through the filter parameter structure.

## Quick Start

Install and load the library (where `<version and os> stands for the appropriate postfix`):

```R
install.packages('release/gar_<version and os>', repos = NULL)
library(gar)
```

Get the default filter parameters:

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

Load some sample data from a csv file (make sure that all data parsed as a `numeric` and not as `integer`):

```R
d <- read.csv('sample/test.csv', colClasses=c('numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric', 'numeric'))
```

Finally, pass the sample data to the parser
```R
res <- gar_parse( h, d$px, d$py, d$pz, d$ox, d$oy, d$oz, d$timestamp )
```

The command `print(res)` should show the following output (one fixation and two saccades):

```R
$fixations
        px       py       pz duration timestamp
1 499.8182 499.3636 499.7273 166.6667  1083.333

$saccades
  startx starty startz destx desty destz duration timestamp
1    300    300    500   500   500   500 33.33333      1050
2    501    499    500   600   600   500 16.66667      1250
```

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
