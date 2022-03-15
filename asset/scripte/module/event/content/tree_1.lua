MapEventManager:addMapEvents({

    tree_1 = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("���C�Ȗ؂�����Ă�݂���")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[����A�}�����낢��]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[�����Ƃ���]], [[�܂����Ⴄ]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            m:streamText("\n"..[[���̖؂������Ă��邩���]].."\n"..[[�����Ƃ��Ă�����]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[�ۂ���!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�����Ɏg������������Ƃ��Ă�����]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_tree_branch, true)
        end

        m:close()
    end,})
    