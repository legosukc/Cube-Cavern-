
local class = {}
class.__index = class

class.Name = "ItemBase"
class.Description = "oopsies! you forgot to add a description!"
class.BookEntry = "oopsies! you forgot to add a book entry!"

class.HoverText = ""

class.Type = Enums.ItemTypes.Item

class.Price = 0
class.Rarity = 0
class.PickupTier = 0

class.Cooldown = 0

function class.new()
	
	local new = setmetatable({}, class)
	new.Equipped = false
	new.Physics = false

	return new
end

local BlankClassFunction = function(self)
end


class.LMB = BlankClassFunction
class.RMB = BlankClassFunction


function class:CanPickUp()
	return not self.Physics and not self.Owner
end

class.OnPickup = BlankClassFunction
class.OnDrop = BlankClassFunction


function class:OnEquip()
	self.Equipped = true
end

function class:OnUnequip()
	self.Equipped = false
end


class.Update = BlankClassFunction
class.Draw = BlankClassFunction


function class:GetHoverText()
	
	local InteractionText
	if self.MustBePurchased then
		InteractionText = "inspect"
	else
		InteractionText = (self.Type == Enums.ItemTypes.Pickup or self.Type == Enums.ItemTypes.Item) and "take" or "wear"
	end
	
	return "E to "..InteractionText.." "..self.Name:lower().."."
end

function class:GetDescriptionText()
	return self.Description
end

function class:GetBookEntryText()
	return self.BookEntry
end

return class