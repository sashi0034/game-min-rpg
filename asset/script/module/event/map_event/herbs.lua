MapEventManager:addMapEvents({

    herbs_near_bush = function (self, e)
        local flag = FlagName.obtain_herbs_near_bush
        if FlagManager.getFlag(flag) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText([[おや、ぽつんと薬草が生えてる]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[とっておこう]])
        while m:isRunning() do Yield() end

        FlagManager.setFlag(flag, true)
        m:close()
    end,

    herbs_under_tree = function (self, e)
        local flag = FlagName.obtain_herbs_under_tree
        if FlagManager.getFlag(flag) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText([[おや、木の根本に薬草が生えてる]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[とっておこう]])
        while m:isRunning() do Yield() end

        FlagManager.setFlag(flag, true)
        m:close()
    end,

    herbs_near_castle = function (self, e)
        local flag = FlagName.obtain_herbs_near_castle
        if FlagManager.getFlag(flag) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText([[おや、茂みの中に薬草が生えてる]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[とっておこう]])
        while m:isRunning() do Yield() end

        FlagManager.setFlag(flag, true)
        m:close()
    end,

})
    