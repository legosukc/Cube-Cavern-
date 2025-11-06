
local ParentClass = Assets.Modules.ObjectAuthorityClass.new()

local class = setmetatable({}, ParentClass)


local ItemClasses = Assets.ItemClasses

function class:Create(Name)

	local ItemClass = ItemClasses[Name]
	if not ItemClass then
		print("Unable to find ItemClass of name: ", Name)
		return nil
	end

	local NewItemClass = ParentClass.Create(self, ItemClass)
	NewItemClass.__type = "Item"

	return ItemClass
end

return class