MapEventManager:addMapEvents({

    house_r_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[東の台地、登れなさそうだけど]].."\n"..[[どこかから登れちゃうみたい]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
