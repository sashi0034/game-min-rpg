MapEventManager:addMapEvents({

    herbs_near_bush = function (self, e)
        local flag = FlagName.obtain_herbs_near_bush
        if FlagManager.getFlag(flag) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText([[����A�ۂ�Ɩ򑐂������Ă�]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[�Ƃ��Ă�����]])
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
        
        m:streamText([[����A�؂̍��{�ɖ򑐂������Ă�]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[�Ƃ��Ă�����]])
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
        
        m:streamText([[����A�΂݂̒��ɖ򑐂������Ă�]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[�Ƃ��Ă�����]])
        while m:isRunning() do Yield() end

        FlagManager.setFlag(flag, true)
        m:close()
    end,

})
    