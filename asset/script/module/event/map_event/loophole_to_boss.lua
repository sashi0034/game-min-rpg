MapEventManager:addMapEvents({

    loophole_to_boss = function (self, e)
        if not FlagManager.getFlag(FlagName.find_loophole_to_boss) then
            FlagManager.setFlag(FlagName.find_loophole_to_boss, true)
        end
    end
})
