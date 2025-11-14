
local ParentClass = require("WeaponBase")

local class = {}
class.__index = class

class.Damage = 1

class.Type = Enum.ItemTypes.Melee

class.new = ParentClass.new

function class:LMB()

end