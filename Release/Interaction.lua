
local ParentClass = Assets.Modules.ObjectAuthorityClass.new()

local class = setmetatable({}, ParentClass)


local InteractClasses = Assets.InteractClasses

function class:Create(Name)

	local InteractClass = InteractClasses[Name]
	if not InteractClass then
		print("Unable to find InteractClass of name: ", Name)
		return nil
	end
	
	local NewInteractable = ParentClass.Create(InteractClass)
	NewInteractable.__type = "Interactable"

	return NewInteractable
end

return class