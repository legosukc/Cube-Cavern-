
local class = {}
class.__index = class

class.InteractText = "interact"
class.HoverText = "%s to %s."

function class.new()
	return setmetatable({}, class)
end

local BlankClassFunction = function(self)
end

class.OnInteract = BlankClassFunction

class.Update = BlankClassFunction
class.Draw = BlankClassFunction


function class:GetHoverText()
	
	return self.HoverText:format("E", self.InteractText)
end

return class