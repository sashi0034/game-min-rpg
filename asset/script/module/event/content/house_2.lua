MapEventManager:addMapEvents({

    house_2 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[����Ɍ������Ă���]].."\n"..[[�l�Y�~���������Ƃ������]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[�����Ƃ������Ƃ���]])
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[�����͂Ȃ�����]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
