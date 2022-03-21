MapEventManager:addMapEvents({

    house_r_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[“Œ‚Ì‘ä’nA“o‚ê‚È‚³‚»‚¤‚¾‚¯‚Ç]].."\n"..[[‚Ç‚±‚©‚©‚ç“o‚ê‚¿‚á‚¤‚İ‚½‚¢]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
