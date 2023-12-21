import math

#r = 50
r = 100
x, y = 100, -100
print("r: ", r)

top_right_x = x + r * math.cos(math.radians(45))
top_right_y = y + r * math.sin(math.radians(45))
print(round(top_right_x, 2), round(top_right_y, 2))

bottom_right_x = x + r * math.cos(math.radians(315))
bottom_right_y = y + r * math.sin(math.radians(315))
print(round(bottom_right_x, 2), round(bottom_right_y, 2))

bottom_left_x = x + r * math.cos(math.radians(225))
bottom_left_y = y + r * math.sin(math.radians(225))
print(round(bottom_left_x, 2), round(bottom_left_y, 2))

top_left_x = x + r * math.cos(math.radians(135))
top_left_y = y + r * math.sin(math.radians(135))
print(round(top_left_x, 2), round(top_left_y, 2))
    
########## output ###########
# ('r: ',100)
# (170.71, 29.29)
# (170.71, 170.71)
# (29.29, 170.71)
# (29.29, 29.29)
# 
# ('r: ', 50)
# (135.36, 64.64)
# (135.36, 135.36)
# (64.64, 135.36)
# (64.64, 64.64)

# 绘制一个圆环，分成8段，背景透明，无边缘线条
convert -size 200x200 xc:none -stroke none \
    -fill 'rgba(255, 0, 0, 0.5)'  -draw "path 'M 100,0        A 100,100 0 0,1 170.71,29.29   L 135.36,64.64  A 50,50 0 0,0 100,50 Z'" \
    -fill 'rgba(0, 255, 0, 0.5)'  -draw "path 'M 170.71,29.29 A 100,100 0 0,1 200,100        L 150,100       A 50,50 0 0,0 135.36,64.64 Z'" \
    -fill 'rgba(0, 255, 0, 0.5)'  -draw "path 'M 200,100       A 100,100 0 0,1 170.71,170.71 L 135.36,135.36 A 50,50 0 0,0 150,100 Z'" \
    -fill 'rgba(255, 0, 0, 0.5)'  -draw "path 'M 170.71,170.71 A 100,100 0 0,1 100,200       L 100,150       A 50,50 0 0,0 135.36, 135.36 Z'" \
    -fill 'rgba(0, 0, 255, 0.5)'  -draw "path 'M 100,200      A 100,100 0 0,1 29.29,170.71   L 64.64,135.36  A 50,50 0 0,0 100,150 Z'" \
    -fill 'rgba(0, 255, 0, 0.5)'  -draw "path 'M 29.29,170.71 A 100,100 0 0,1 0,100          L 50,100        A 50,50 0 0,0 64.64,135.36 Z'" \
    -fill 'rgba(0, 0, 255, 0.5)'  -draw "path 'M 0,100        A 100,100 0 0,1 29.29,29.29    L 64.64,64.64   A 50,50 0 0,0 50,100 Z'" \
    -fill 'rgba(0, 255, 0, 0.5)'  -draw "path 'M 29.29,29.29  A 100,100 0 0,1 100,0          L 100,50        A 50,50 0 0,0 64.64,64.64 Z'" \
    piechart_4.png
