MapEventManager:addMapEvents({

    house_2 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[お城に向かっていく]].."\n"..[[ネズミを見たことがあるわ]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[きっとおいしいに決まってるわ]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[根拠はないけど]])
        while m:isRunning() do Yield() end

        m:close()
        FlagManager.setFlag(FlagName.heard_mouse_rumor, true)
    end
})
