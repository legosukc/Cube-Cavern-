
-- include a __type for all classes

local class = {}
class.__index = class

function class.new()

	local new = {}
	new.Instances = {}
	setmetatable(new, class)
	
	return new
end

function class:Create(Class)

	local NewClass = Class.new()
	table.insert(Instances, NewClass)

	NewClass._InstanceIndex = #Instances

	return NewClass
end

--[[
this is a C function
function class:Remove(Class) -> none

--]]
class.Remove = nil


function class:Update()
	
	for _, Class in ipairs(self.Instances) do
		Class:Update()
	end
end

function class:Draw()
	
	for _, Class in ipairs(self.Instances) do
		Class:Draw()
	end
end

return class