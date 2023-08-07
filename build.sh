#!/bin/bash

R -e 'devtools::document()'

version=$(gawk 'match($0, /Version: ([0-9]+\.[0-9]+\.[0-9]+)/, a) {print a[1]}' DESCRIPTION)
R CMD build .
mv gar_$version.tar.gz release/.
