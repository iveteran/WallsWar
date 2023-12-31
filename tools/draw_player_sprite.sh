# Ref: https://juejin.cn/post/6970637003142463519

images_path="images"

# 头像：一个圆
draw_avatar() {
    local center="25,25"
    local radius=25
    local margin=0
    local output_file="$images_path/player_avatar_circle.png"

    convert -size 50x50 xc:none -stroke none  -fill tomato \
        -draw "circle $center $radius, $margin" $output_file

    display $output_file
}

# 头像：从现用的图片剪出一个圆形头像
crop_avator() {
    local uploaded_image=$1
    local file_basename=$(basename ${uploaded_image%.*})
    local image_width=`identify -format "%w" $uploaded_image`
    local image_height=`identify -format "%h" $uploaded_image`
    local to_size=$((image_width < image_height ? image_width : image_height))  # get minimum value
    local center="$((to_size / 2)),$((to_size / 2))"
    local radius=$((to_size / 2))
    local margin=0
    local output_file="$images_path/croped_circle_$file_basename.png"

    convert $uploaded_image -alpha set -background none -fill white \
        \( +clone -channel A -evaluate set 0 +channel -draw "circle $center $radius, $margin" \) \
        -compose dstin -composite $output_file

    convert $output_file -resize 50x50 $output_file

    display $output_file
}

# 血环：一个圆环，分成3/5/7段，使用不同颜色区分
draw_blood_ring() {
    local circle_angle=360
    local slice_count=3
    local cell_angle=$((circle_angle / slice_count))
    local center_x=30
    local center_y=30
    local outside_radius=30
    local inside_radius=25
    local outside_arc_radius="$outside_radius,$outside_radius"
    local inside_arc_radius="$inside_radius,$inside_radius"

    local x_axis_rotation=0
    local large_arc_flag=0
    local outside_sweep_flag=0    # 逆时针绘制
    local inside_sweep_flag=1     # 顺时针绘制

    local angle0=$((cell_angle * 0))
    local angle1=$((cell_angle * -1))  # 角度是逆时针所以乘以-1
    local angle2=$((cell_angle * -2))  # 同上
    local angle3=$((cell_angle * -3))  # 同上

    local outside_arc_start_point=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle0)
    local outside_arc_end_point=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle1)
    local inside_arc_start_point=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle1)
    local inside_arc_end_point=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle0)

    local outside_arc_start_point_2=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle1)
    local outside_arc_end_point_2=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle2)
    local inside_arc_start_point_2=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle2)
    local inside_arc_end_point_2=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle1)

    local outside_arc_start_point_3=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle2)
    local outside_arc_end_point_3=$(python3 circle_angle_to_point.py $center_x $center_y $outside_radius $angle3)
    local inside_arc_start_point_3=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle3)
    local inside_arc_end_point_3=$(python3 circle_angle_to_point.py $center_x $center_y $inside_radius $angle2)

    local output_file="$images_path/player_blood_ring_$slice_count.png"

    convert -size 60x60 xc:none -stroke none \
        -fill 'magenta'  -draw "path '\
        M $outside_arc_start_point \
        A $outside_arc_radius $x_axis_rotation $large_arc_flag $outside_sweep_flag $outside_arc_end_point \
        L $inside_arc_start_point \
        A $inside_arc_radius $x_axis_rotation $large_arc_flag $inside_sweep_flag $inside_arc_end_point \
        Z'" \
        -fill 'blue'  -draw "path '
        M $outside_arc_start_point_2 \
        A $outside_arc_radius $x_axis_rotation $large_arc_flag $outside_sweep_flag $outside_arc_end_point_2 \
        L $inside_arc_start_point_2 \
        A $inside_arc_radius $x_axis_rotation $large_arc_flag $inside_sweep_flag $inside_arc_end_point_2 \
        Z'" \
        -fill 'green' -draw "path '
        M $outside_arc_start_point_3 \
        A $outside_arc_radius $x_axis_rotation $large_arc_flag $outside_sweep_flag $outside_arc_end_point_3 \
        L $inside_arc_start_point_3 \
        A $inside_arc_radius $x_axis_rotation $large_arc_flag $inside_sweep_flag $inside_arc_end_point_3 \
        Z'" \
        $output_file

    display $output_file
}

# 武器(回旋标)：挂在avatar4个角上
draw_boomerang() {
    local x_axis_rotation=0
    local large_arc_flag=0
    local sweep_flag=1
    local arc_start_point="0,0"
    local arc_end_point="30,30"
    local arc_radius="30,30"
    local broken_point="30,0"
    local output_file="$images_path/player_boomerang.png"

    convert -size 30x30 xc:none -stroke none \
        -fill 'purple'  -draw "path '\
        M $arc_start_point \
        A $arc_radius $x_axis_rotation $large_arc_flag $sweep_flag $arc_end_point \
        L $broken_point \
        Z'" \
        $output_file

    display $output_file
}

# 运动方向指示：半透明三角
draw_triangle() {
    local angle1=-90
    local angle2=-210
    local angle3=-330
    local center_x=10
    local center_y=10
    local radius=10
    #local color="rgba(128,0,128,0.5)"
    local color="rgba(255,255,255,0.5)"  # white

    local top_point=$(python3 circle_angle_to_point.py $center_x $center_y $radius $angle1)
    local bottom_left_point=$(python3 circle_angle_to_point.py $center_x $center_y $radius $angle2)
    local bottom_right_point=$(python3 circle_angle_to_point.py $center_x $center_y $radius $angle3)
    local output_file="$images_path/player_direction_triangle.png"

    convert -size 20x100 xc:none -stroke none \
        -fill $color -draw "path 'M $top_point L $bottom_left_point L $bottom_right_point Z'" \
        $output_file

    display $output_file
}

#draw_avatar
#crop_avator "$images_path/my_avatar.jpg"
#crop_avator "$images_path/yusen_avatar.jpg"
#crop_avator "$images_path/avatar-anonymous-64.png"
draw_blood_ring
#draw_boomerang
#draw_triangle
