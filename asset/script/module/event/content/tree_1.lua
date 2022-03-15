MapEventManager:addMapEvents({

    tree_1 = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("Œ³‹C‚È–Ø‚ªˆç‚Á‚Ä‚é‚İ‚½‚¢")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[‚¨‚âA}‚ª‚à‚ë‚¢‚¼]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[‚»‚Á‚Æ‚·‚é]], [[Ü‚Á‚¿‚á‚¤]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            m:streamText("\n"..[[‚±‚Ì–Ø‚à¶‚«‚Ä‚¢‚é‚©‚ç‚Ë]].."\n"..[[‚»‚Á‚Æ‚µ‚Ä‚¨‚±‚¤]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[‚Û‚«‚Á!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[‰½‚©‚Ég‚¦‚»‚¤‚¾‚©‚ç‚Æ‚Á‚Ä‚¨‚±‚¤]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_tree_branch, true)
        end

        m:close()
    end,})
    