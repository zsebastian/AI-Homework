robot_names = {}
robot_names[0] = "red_robot.lua"
robot_names[1] = "blue_robot.lua"
robot_names[2] = "green_robot.lua"

for i = 1, 100 do
	run_script(robot_names[random(0, 2)])
end