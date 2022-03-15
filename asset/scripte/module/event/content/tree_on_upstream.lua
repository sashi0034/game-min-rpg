MapEventManager:addMapEvents({

    tree_on_upstream = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true or FlagManager.getFlag(FlagName.create_wood_sword)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("Œ³‹C‚È–Ø‚ªˆç‚Á‚Ä‚é‚È")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[‚¨‚âA}‚ª‚à‚ë‚¢‚¼]])
        while m:isRunning() do Yield() end


    end,
})
    