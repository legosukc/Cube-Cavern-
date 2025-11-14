
local ParentClass = Assets.ItemClasses.WeaponBase

local class = {}
setmetatable(class, ParentClass)
class.__index = class

class.Name = "Rock"
class.Description = "He who throws stones is pretty cool. Deals %s ranged damage. Basic crafting material."

class.BookEntry = "The ROCK is obtained by finding them or pruchasing them in shops. They do %s ranged damage and serve as a basic crafting material."

class.Type = Enums.ItemTypes.Ranged

class.Cooldown = 0.4
class.SwingDuration = 0.14
class.Damage = 2

class.KnockbackAmount = 0.5

function class.new()

	local new = ParentClass.new()
	setmetatable(new, class)
	
	return new
end

function class:LMB()
end

return class