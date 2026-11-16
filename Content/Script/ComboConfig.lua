--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_MainCharacter_C
local Screen = require "Utils.Screen"

local M = UnLua.Class()


function M:Initialize(Initializer)  
	local msg = [[ 
	Hellow 
	Unlua
	]]
	print(msg)
	Screen.Print(msg)
end

-- function M:UserConstructionScript()
-- end

-- function M:ReceiveBeginplay()
	-- local msg = "Unlua beginplay"
	-- print(msg)
	-- Screen.Print(msg)
-- end

-- function M:ReceiveEndPlay()
-- end

-- function M:ReceiveTick(DeltaSeconds)
-- end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return M
