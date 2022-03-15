MapEventManager:addMapEvents({

    house_2 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[‚¨é‚ÉŒü‚©‚Á‚Ä‚¢‚­]].."\n"..[[ƒlƒYƒ~‚ğŒ©‚½‚±‚Æ‚ª‚ ‚é‚í]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[‚«‚Á‚Æ‚¢‚¢‚±‚Æ‚¾‚í]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[ª‹’‚Í‚È‚¢‚¯‚Ç]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
