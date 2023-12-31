#!/bin/sh
# Ref: https://imagemagick.org/script/gradient.php

convert -size 40x20 -define gradient:extent=ellipse radial-gradient:black-none player-shadow.png

convert -size 40x20 -define gradient:extent=diagonal radial-gradient:black-none block-shadow.png
