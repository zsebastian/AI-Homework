-- run away from all other robots

new_robot = register_robot("red_robot"..robot_count, 
	function(this)
		d_x = 0
		d_y = 0
		for i = 1, robot_count do
			robot = robots[i - 1]
			if robot.name ~= this.name and manhattan_distance(this, robot) < 20 then
				d_x = d_x + this.x - robot.x
				d_y = d_y + this.y - robot.y
			end
		end

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

		if this.x < 0 then this.x = 0 end
		if this.y < 0 then this.y = 0 end
		if this.x >= level.width then this.x = level.width - 1 end
		if this.y >= level.height then this.y = level.height - 1 end
	end
);

new_robot.r = 1
new_robot.health = 10