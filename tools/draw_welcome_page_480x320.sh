#!/bin/env bash

size_width=480
size_height=320

game_logo_offset=90
game_logo_height=$(((size_height-$game_logo_offset)/2 * -1))
game_logo_file="logo-goe-32.png"

game_name="GOE"
game_name_fontsize=32
game_name_y_offset=-50

game_fullname="Gaming On Empty"
game_fullname_fontsize=12
game_fullname_y_offset=25

center_line_margin=20
center_line_position="$center_line_margin,$((size_height/2)) $((size_width-center_line_margin)),$((size_height/2))"

social_media_y_offset=40
social_fontsize=6

discord_logo_x_offset=-90
discord_account_x_offset=-40
discord_logo_file="logo-discord-16.png"
discord_account="@goeworks"

twitter_logo_x_offset=10
twitter_account_x_offset=60
twitter_logo_file="logo-twitter-16.png"
twitter_account="@goeworks"

company_logo_x_offset=-60
company_logo_y_offset=10
company_logo_file="logo-matrixworks-16.png"

company_footnote_x_offset=10
company_footnote_y_offset=$company_logo_y_offset
company_footnote_fontsize=5
company_footnote="Powered by Matrixworks"

image_size="${size_width}x${size_height}"
output_file="goe_welcome_$image_size.png"

source ./draw_welcome_page_template.sh

display $output_file
