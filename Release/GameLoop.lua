
local UpdateItems = Game.Items.Update
local UpdateEnemies = Game.Enemies.Update
local UpdateInteractables = Game.Interactables.Update

local DrawItems = Game.Items.Draw
local DrawEnemies = Game.Enemies.Draw
local DrawInteractables = Game.Interactables.Draw


-- UPDATING

return function()

	UpdateItems()
	UpdateEnemies()
	UpdateInteractables()
end,


-- DRAWING

function()
	
	DrawInteractables()
	DrawEnemies()
	DrawItems()
end