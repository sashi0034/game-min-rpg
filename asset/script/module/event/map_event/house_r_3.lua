MapEventManager:addMapEvents({

    house_r_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[�������瓌�̑�n����]]..[[���̋C�z�������܂�]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
