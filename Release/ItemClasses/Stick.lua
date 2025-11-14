
local ParentClass = Assets.ItemClasses.WeaponBase

local class = {}
setmetatable(class, ParentClass)
class.__index = class

class.Name = "Stick"
class.Description = "How'd this get down here? Deals %s melee damage. Basic crafting material."

class.BookEntry = "The STICK is obtained by finding them or in shops. They do %s melee damage and serve as a basic crafting material."

class.Type = Enums.ItemTypes.Melee

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