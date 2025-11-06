
local class = {}
class.__index = class

class.Name = "EnemyBase"

class.MaxHealth = 5

function class.new()

	local new = setmetatable({}, class)

	new.Health = new.MaxHealth
	new.Dead = false
	
	return new
end

local BlankClassFunction = function(self)
end


function class:Damage(Attacker, Damage, KnockbackVelocity)
	
	if true then --typeof(Attacker) == "Player" then

	else

	end

	self.Health -= Damage
	self:KillCheck()
end

function class:KillCheck()

	if self.Health <= 0 and not self.Dead then
		self:OnKill()
	end
end

function class:OnKill()
	
	self.Dead = true
end


class.UpdateBase = class.KillCheck
class.Update = BlankClassFunction

class.Draw = BlankClassFunction

return class