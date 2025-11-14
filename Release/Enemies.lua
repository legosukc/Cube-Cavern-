
local ParentClass = Assets.Modules.ObjectAuthorityClass.new()

local class = ParentClass --setmetatable({}, ParentClass)


local EnemyClasses = Assets.EnemyClasses

function class:Create(Name)

	local EnemyClass = EnemyClasses[Name]
	if not EnemyClass then
		print("Unable to find EnemyClass of name: ", Name)
		return nil
	end

	local NewEnemyClass = ParentClass.Create(self, EnemyClass)
	NewEnemyClass.__type = "Enemy"

	return NewEnemyClass
end

return class