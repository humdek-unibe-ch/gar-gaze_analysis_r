#!/bin/bash

rm -rf build
mkdir -p build

version=$(gawk 'match($0, /Version: ([0-9]+\.[0-9]+\.[0-9]+)/, a) {print a[1]}' DESCRIPTION)
package=gar_${version}_R_x86_64-pc-linux-gnu.tar.gz

R -e 'devtools::document()'
R CMD INSTALL -l build --build .

mv ${package} build/.
