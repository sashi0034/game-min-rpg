MapEventManager:addMapEvents({

    tree_on_upstream = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true or FlagManager.getFlag(FlagName.create_wood_sword)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("���C�Ȗ؂�����Ă��")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[����A�}�����낢��]])
        while m:isRunning() do Yield() end


    end,
})
    