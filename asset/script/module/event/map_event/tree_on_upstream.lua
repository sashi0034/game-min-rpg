MapEventManager:addMapEvents({
    tree_on_upstream = function (self, e)
        if (FlagManager.getFlag(FlagName.kill_bee)==true) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText("‚¨‚âAƒnƒ`‚Ì‘ƒ‚ª‚ ‚é‚İ‚½‚¢‚¾")
        while m:isRunning() do Yield() end

        local opt = {[[‹C‚É‚µ‚È‚¢]], [[‰£‚é]]}
        if FlagManager.getFlag(FlagName.have_tree_branch) then
            opt[#opt+1] = [[–Ø‚Ì}]]
        end
        if FlagManager.getFlag(FlagName.have_wood_sword) then
            opt[#opt+1] = [[–Ø“]]
        end

        local s = SelectionWindow.open(opt)
        while s:isRunning() do Yield() end
        local word=s:selectedWord()
        s:close()

        if (word=="‹C‚É‚µ‚È‚¢") then
            m:streamText("\n"..[[Šë‚È‚»‚¤‚¾‚©‚ç]].."\n"..[[Œ©‚È‚©‚Á‚½‚±‚Æ‚É‚µ‚æ‚¤]])
            while m:isRunning() do Yield() end
        elseif word=="‰£‚é" then
            m:streamText("\n"..[[‚¦‚¢A]])
            m:animShake(1.0)
            while m:isRunning() do Yield() end

            m:streamText([[‚¦‚¢!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[‚¤‚íA–I‚³‚ñ‚ªo‚Ä‚«‚½!!]])
            while m:isRunning() do Yield() end

            m:animFlash()
            while m:isRunning() do Yield() end
        elseif word=="–Ø‚Ì}" then
            m:streamText("\n"..[[‚³‚Á‚«ƒQƒbƒg‚µ‚½}‚ÅUŒ‚‚µ‚Ä‚İ‚æ‚¤]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[‚Æ‚è‚áA]])
            while m:isRunning() do Yield() end

            m:streamText([[‚¦‚¢!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[‚¤‚íA–I‚³‚ñ‚ª“{‚Á‚Ä‚é!!]])
            while m:isRunning() do Yield() end
        elseif word=="–Ø“" then
            m:streamText("\n"..[[‚³‚Á‚«ì‚Á‚Ä‚à‚ç‚Á‚½Œ•‚ÅUŒ‚‚µ‚Ä‚İ‚æ‚¤]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[‚Æ‚è‚áA]])
            while m:isRunning() do Yield() end

            m:streamText([[‚¤‚è‚á!]])
            m:animShake(2.0)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[‚æ‚µA–I‚³‚ñ‚ğ‚â‚Á‚Â‚¯‚½‚æ!!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[‚Í‚¿‚İ‚Â‚ğè‚É“ü‚ê‚½]])
            while m:isRunning() do Yield() end
            FlagManager.setFlag(FlagName.kill_bee, true)
            FlagManager.setFlag(FlagName.obtain_honey, true)
        end

        m:close()
    end,
})
    