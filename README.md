# R Bindings for `libgac`

This repository holds an R package which implement the bindings to the gaze analysis library [gac](http://phhum-a209-cp.unibe.ch:10012/LIB/LIB-gaze_analysis_c).

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
