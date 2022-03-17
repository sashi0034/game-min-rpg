MapEventManager:addMapEvents({

    house_on_hill = function (self, e)
        local m = MessageWindow.open()

        if FlagManager.getFlag(FlagName.obtain_solt) then
            m:streamText([[‚Í‚¿‚İ‚Â”ü–¡‚µ‚¢‚Ë]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[‚Ø‚ë‚Ø‚ëA‚Ø‚ë‚Ø‚ë]])
            while m:isRunning() do Yield() end

            m:close()
            return
        end

        m:streamText([[‚Í‚¿‚İ‚Â‚ªH‚×‚½‚¢‚Ë]])
        while m:isRunning() do Yield() end

        if FlagManager.getFlag(FlagName.obtain_honey) then
            m:streamText("\n"..[[‚­‚ñ‚­‚ñ.. ]])
            while m:isRunning() do Yield() end

            m:streamText([[‚«‚İ‚Í‚Í‚¿‚İ‚Â‚ğ]].."\n"..[[‚Á‚Ä‚¢‚é‚ñ‚¶‚á‚È‚¢‚©‚È?]])
            while m:isRunning() do Yield() end

            local index=0
            local refuseCount=0

            repeat
                local s = SelectionWindow.open({[[‚ ‚°‚é]], [[‚ ‚°‚È‚¢]]})
                while s:isRunning() do Yield() end
                index=s:selectedIndex()
                s:close()

                if index==1 then
                    m:streamText("\n"..[[‚â‚¾A‚¨Šè‚¢!]])
                    while m:isRunning() do Yield() end
                    refuseCount=refuseCount+1
                end
            until index==0

            if refuseCount==0 then
                m:streamText("\n"..[[‚ ‚è‚ª‚Æ‚¤!]])
                while m:isRunning() do Yield() end
            else
                m:streamText("\n"..[[–³—‚ğŒ¾‚Á‚Ä‚²‚ß‚ñ‚Ë..]])
                while m:isRunning() do Yield() end
            end

            m:streamText("\n"..[[‚¨—ç‚É‹ß‚­‚Å‚Æ‚ê‚½Šâ‰–‚ğ]].."\n"..[[‚ ‚°‚é‚æ]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.obtain_solt, true)
        end

        m:close()
    end
})
