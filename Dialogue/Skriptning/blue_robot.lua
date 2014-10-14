-- move randomly

new_robot = register_robot("blue_robot"..robot_count, 
	function(this)
		this.x = this.x + random(-1, 1);
		this.y = this.y + random(-1, 1);
		
		if this.x < 0 then this.x = 0 end
		if this.y < 0 then this.y = 0 end
		if this.x >= level.width then this.x = level.width - 1 end
		if this.y >= level.height then this.y = level.height - 1 end
	end
);

new_robot.b = 1
new_robot.health = 50