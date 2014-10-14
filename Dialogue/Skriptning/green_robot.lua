-- Always chase the closest robot

new_robot = register_robot("green_robot"..robot_count, 
	function(this)
		robot = nil
		closest_robot = nil
		for i = 1, robot_count do
			robot = robots[i - 1]
			if robot.name ~= this.name then
				if closest_robot == nil then
					closest_robot = robot
				elseif manhattan_distance(this, closest_robot) > manhattan_distance(this, robot) then
					closest_robot = robot
				end
			end
		end

		if closest_robot ~= nil then
			d_x = closest_robot.x - this.x
			d_y = closest_robot.y - this.y

			if d_x < 0 then
				this.x = this.x - 1
			end
			if d_x > 0 then
				this.x = this.x + 1
			end 
			if d_y < 0 then
				this.y = this.y - 1
			end
			if d_y > 0 then
				this.y = this.y + 1
			end
			if d_y == 0 and d_x == 0 then
				this.x = this.x + random(-1, 1)
				this.y = this.y + random(-1, 1)
			end
		end

		if this.x < 0 then this.x = 0 end
		if this.y < 0 then this.y = 0 end
		if this.x >= level.width then this.x = level.width - 1 end
		if this.y >= level.height then this.y = level.height - 1 end
	end
);

new_robot.g = 1
new_robot.health = 200