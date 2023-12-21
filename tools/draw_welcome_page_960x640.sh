#!/bin/env bash

size_width=960
size_height=640

game_logo_offset=180
game_logo_height=$(((size_height-$game_logo_offset)/2 * -1))
game_logo_file="logo-goe-64.png"

game_name="GOE"
game_name_fontsize=64
game_name_y_offset=-80

game_fullname="Gaming On Empty"
game_fullname_fontsize=20
game_fullname_y_offset=40

center_line_margin=40
center_line_position="$center_line_margin,$((size_height/2)) $((size_width-center_line_margin)),$((size_height/2))"

social_media_y_offset=40
social_fontsize=10

discord_logo_x_offset=-120
discord_account_x_offset=-50
discord_logo_file="logo-discord-24.png"
discord_account="@goeworks"

twitter_logo_x_offset=20
twitter_account_x_offset=90
twitter_logo_file="logo-twitter-24.png"
twitter_account="@goeworks"

company_logo_x_offset=-90
company_logo_y_offset=10
company_logo_file="logo-matrixworks-24.png"

company_footnote_x_offset=20
company_footnote_y_offset=$company_logo_y_offset
company_footnote_fontsize=8
company_footnote="Powered by Matrixworks"

image_size="${size_width}x${size_height}"
output_file="goe_welcome_$image_size.png"

source ./draw_welcome_page_template.sh

display $output_file
