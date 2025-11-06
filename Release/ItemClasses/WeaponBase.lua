
local ParentClass = Assets.ItemClasses.ItemBase

local class = {}
setmetatable(class, ParentClass)
class.__index = class

class.Name = "WeaponBase"

class.Type = Enum.ItemTypes.Weapon

class.Damage = 1

function class.new()
	
	local new = ParentClass.new()
	setmetatable(new, class)

	return new
end

function class:GetDescriptionText()
	return self.Description:format(self.Damage)
end

function class:GetBookEntryText()
	return self.BookEntry:format(self.Damage)
end

return class