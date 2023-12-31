import sys
import math

#print("args: %d, argv: %s" % (len(sys.argv)-1, str(sys.argv[1:])))
if len(sys.argv) != 5:
    print("Error parameters")
    exit(-1)

center_x = int(sys.argv[1])
center_y = int(sys.argv[2])
radius = int(sys.argv[3])
angle = int(sys.argv[4])
#print("center: (%d, %d), radius: %d, angle: %d" % (center_x, center_y, radius,  angle))

point_x = center_x + radius * math.cos(math.radians(angle))
point_y = center_y + radius * math.sin(math.radians(angle))

print("%0.2f,%0.2f" % (point_x, point_y))
exit(0)
