MapEventManager:addMapEvents({

    house_r_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[‚±‚±‚©‚ç“Œ‚Ì‘å’n‚©‚ç]]..[[–‚‚Ì‹C”z‚ðŠ´‚¶‚Ü‚·]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
