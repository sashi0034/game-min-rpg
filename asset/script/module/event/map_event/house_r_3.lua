MapEventManager:addMapEvents({

    house_r_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[���̑�n�A�o��Ȃ�����������]].."\n"..[[�ǂ�������o�ꂿ�Ⴄ�݂���]])
        while m:isRunning() do Yield() end

        m:close()
    end
})
