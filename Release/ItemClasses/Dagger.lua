
local ParentClass = Assets.ItemClasses.WeaponBase

local class = {}
setmetatable(class, ParentClass)
class.__index = class

class.Name = "Dagger"
class.Description = "The sword's little brother, half as long, just as deadly! Deals %s melee damage." --"3 damage, pretty fuckin weak."

class.BookEntry = "Daggers are obtained by crafting a rock and a stick together. They do %s melee damage."

class.Type = Enums.ItemTypes.Melee

class.Cooldown = 0.4
class.SwingDuration = 0.14
class.Damage = 3

class.KnockbackAmount = 0.5

function class.new()

	local new = ParentClass.new()
	setmetatable(new, class)
	
	return new
end

function class:LMB()
end

return class