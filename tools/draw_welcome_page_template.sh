
convert -size $image_size xc:lightblue -gravity center -fill black \
    -draw "image over 0,$game_logo_height,0,0 '$game_logo_file'" \
    -weight heavy -density 144 -pointsize $game_name_fontsize \
    -draw "text 0,$game_name_y_offset '$game_name'" \
    -stroke black -strokewidth 2 \
    -draw "line ${center_line_position}" \
    -stroke none \
    -weight normal -pointsize $game_fullname_fontsize \
    -draw "text 0,$game_fullname_y_offset '$game_fullname'" \
    -gravity south -pointsize $social_fontsize \
    -draw "image over $discord_logo_x_offset,$social_media_y_offset,0,0 '$discord_logo_file'" \
    -draw "text $discord_account_x_offset,$social_media_y_offset '$discord_account'" \
    -draw "image over $twitter_logo_x_offset,$social_media_y_offset,0,0 '$twitter_logo_file'" \
    -draw "text $twitter_account_x_offset,$social_media_y_offset '$twitter_account'" \
    -draw "image over $company_logo_x_offset,$company_logo_y_offset,0,0 '$company_logo_file'" \
    -pointsize $company_footnote_fontsize \
    -draw "text $company_footnote_x_offset,$company_footnote_y_offset '$company_footnote'" \
    $output_file
